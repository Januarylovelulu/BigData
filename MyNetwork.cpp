#include "MyNetwork.h"

#include <QDebug>

MyNetwork::MyNetwork(QString url)
{
    this->url=url;
}

MyNetwork::~MyNetwork()
{

}

void MyNetwork::on_updateDataReadProgress(qint64 now, qint64 all)
{
    nowData=now;
    allData=all;
    now+=downedData;
    all+=downedData;
    qint64 ratio=now*100/all;
    if((int)((QString("%1").arg(all).toDouble())/pow(2,20))>=10)
    {
        emit returnDownloadStatute(QString("下载进度: %1MB / %2MB  %3%")
                                   .arg((int)(now*100.0/pow(2,20))/100.0)
                                   .arg((int)(all*100/pow(2,20))/100.0)
                                   .arg(ratio));
        emit returnDownloadProgressBarValue((int)ratio);
    }
    else
    {
        emit returnDownloadStatute(QString("下载进度: %1KB / %2KB  %3%")
                                   .arg((int)(now*100.0/pow(2,10))/100.0)
                                   .arg((int)(all*100/pow(2,10))/100.0)
                                   .arg(ratio));
        emit returnDownloadProgressBarValue((int)ratio);
    }

    int timeNow = timeDownloadSpeed.elapsed();
    if (timeNow > timerTime)
    {
        double timerTimeSize=(double)timeNow/1000.0;
        qint64 times=(allData-nowData)/((nowData-lastData)/timerTimeSize);
        QString time;
        if((int)(times/(60*60*24))>=1)
            time="剩余时间: >1天";
        else if((int)(times/(60*60))>=1)
            time=QString("剩余时间: %1时%2分%3秒").arg((int)(times/(60*60))).arg((int)(times%(60*60)/60)).arg((int)(times%60));
        else if((int)(times/(60))>=1)
            time=QString("剩余时间: %1分%2秒").arg((int)(times/(60))).arg((int)(times%60));
        else
            time=QString("剩余时间: %1秒").arg((int)(times));

        QString speed;
        if((int)((nowData-lastData)/pow(2,20)/timerTimeSize)>=1)
            speed=QString("下载速度: %1 MB/s").arg((double)((int)((nowData-lastData)*100/timerTimeSize/pow(10,6)))/100);
        else
            speed=QString("下载速度: %1 KB/s").arg((double)((int)((nowData-lastData)*100/timerTimeSize/pow(10,3)))/100);

        emit returnDownloadSpeedAndTime(speed+"    "+time);
        lastData=nowData;
        timeDownloadSpeed.restart();

        savedData=nowData+downedData;
        if(fileDownload->open(QIODevice::WriteOnly | QIODevice::Append))
        {
            fileDownload->write(replyDownload->readAll());
        }
        fileDownload->close();
    }

}

void MyNetwork::httpFinished()
{
    if(fileDownload->open(QIODevice::WriteOnly | QIODevice::Append))
    {
        fileDownload->write(replyDownload->readAll());
    }
    fileDownload->close();
    stopDownload();
    myFile.rename(downloadFileName,downloadFilePath,downloadFileName+DOWNLOAD_FILE_SUFFIX);
    myFile.deleteFile(downloadFileName+DOWNLOAD_FILE_SUFFIX+DOWNLOAD_FILE_Temporary_SUFFIX,downloadFilePath);
    emit returnDownloadStatute("下载完成！");
    emit returnDownloadFinish();
}

void MyNetwork::stopDownload()
{
    if(isDownloading==false)
        return;

    disconnect(replyDownload, &QNetworkReply::downloadProgress, this, &MyNetwork::on_updateDataReadProgress);
    disconnect(replyDownload, &QNetworkReply::finished, this, &MyNetwork::httpFinished);

    fileDownload->flush();
    fileDownload->close();
    replyDownload->deleteLater();
    replyDownload = nullptr;
    delete fileDownload;
    delete managerDownload;
    fileDownload = nullptr;
    isDownloading=false;

    downedData+=nowData;
    myFile.writeTextFile(QString::number(savedData),downloadFileName+DOWNLOAD_FILE_SUFFIX+DOWNLOAD_FILE_Temporary_SUFFIX,downloadFilePath);
}

void MyNetwork::startRequest(QUrl url)
{
    downedData=0;

    QNetworkRequest qheader;
    qheader.setUrl(url);
    if(myFile.isFileExist(downloadFileName+DOWNLOAD_FILE_SUFFIX,downloadFilePath))
    {
        if(myFile.isFileExist(downloadFileName+DOWNLOAD_FILE_SUFFIX+DOWNLOAD_FILE_Temporary_SUFFIX,downloadFilePath))
        {
            QString downedSize=myFile.readTextFile(downloadFileName+DOWNLOAD_FILE_SUFFIX+DOWNLOAD_FILE_Temporary_SUFFIX,downloadFilePath);
            qheader.setUrl(url);
            QString Range="bytes="+downedSize+"-";//告诉服务器从DownedSize起开始传输
            qheader.setRawHeader("Range",Range.toLatin1());
            downedData=downedSize.toLongLong();
        }
    }
    managerDownload = new QNetworkAccessManager(this);
    replyDownload = managerDownload->get(QNetworkRequest(qheader));
    connect(replyDownload, &QNetworkReply::downloadProgress, this, &MyNetwork::on_updateDataReadProgress);
    connect(replyDownload, &QNetworkReply::finished, this, &MyNetwork::httpFinished);

    lastData=0;
    nowData=0;

    timeDownloadSpeed.start();
}

QStringList MyNetwork::getIp()
{
    QString localHostName=QHostInfo::localHostName();
    QHostInfo info=QHostInfo::fromName(localHostName);
    QStringList strList;
    foreach (QHostAddress address, info.addresses())
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            strList.append(address.toString());
        }
    }
    this->ip=strList;
    return strList;
}

QString MyNetwork::getHtmlContent(QString url)
{
    if(!judgElement("url",url))
    {
        return "";
    }
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
    QByteArray responseData;
    QEventLoop eventLoop;
    connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();       //block until finish
    responseData = reply->readAll();
    return QString(responseData);
}

void MyNetwork::download(QString fileName, QString path, QString url)
{
    if(!judgElement("url",url))
        return ;

    if(isDownloading==true)
        return;

    if(path.contains("\\"))
        path.replace("\\","/");

    if(!myFile.isFileExist(fileName,path))
    {
        downloadFileName=fileName;
    }
    else
    {
        if(myFile.isFileExist(fileName+DOWNLOAD_FILE_SUFFIX,path))
        {
            if(!myFile.isFileExist(fileName+DOWNLOAD_FILE_SUFFIX+DOWNLOAD_FILE_Temporary_SUFFIX,path))
            {
                myFile.deleteFile(fileName+DOWNLOAD_FILE_SUFFIX,path);
                myFile.sleep(1000);
            }
        }
        QStringList nameList=fileName.split(".");
        QString name;
        if(nameList.length()==1)
        {
            name=nameList.at(0);
        }
        else
        {
            for(int x=0;x<nameList.length()-1;x++)
                name+=nameList.at(x);
        }
        for(int x=0;;x++)
        {
            if(nameList.length()==1)
            {
                if(!myFile.isFileExist(name+QString("(%1)").arg(x+1),path))
                {
                    downloadFileName=name+QString("(%1)").arg(x+1);
                    break;
                }
            }
            else
            {
                if(!myFile.isFileExist(name+QString("(%1).").arg(x+1)+nameList.at(nameList.length()-1),path))
                {
                    downloadFileName=name+QString("(%1).").arg(x+1)+nameList.at(nameList.length()-1);
                    break;
                }
            }
        }
    }
    downloadFilePath=path;

    fileDownload = new QFile(path+"/"+downloadFileName+DOWNLOAD_FILE_SUFFIX);

    startRequest(url);
    isDownloading=true;
}

bool MyNetwork::judgElement(QString elementName, QString &element)
{
    if(elementName=="url" && element.isEmpty())
    {
        if(!this->url.isEmpty())
        {
            element=this->url;
            qDebug()<<"url is empty, use the class's attribute";
            return true;
        }
        else
        {
            qDebug()<<"url is empty，return false";
            return false;
        }
    }
    else
    {
        return true;
    }
}
