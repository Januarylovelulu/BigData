#include "BigData.h"

#include <QDebug>

BigData::BigData(QString number)
{
    cleanFirstZero(number);
    if(!number.isEmpty())
    {
        this->number=number;
    }
}

BigData::BigData(int number)
{
    this->number=QString::number(number);
}

BigData::BigData(long long number)
{
    this->number=QString::number(number);
}

void BigData::setNumber(QString number)
{
    cleanFirstZero(number);
    this->number=number;
}

QString BigData::getNumber()
{
    return this->number;
}

int BigData::length()
{
    return this->number.length();
}

BigData BigData::add(BigData num1, BigData num2)
{
    QString a=num1.getNumber();
    QString b=num2.getNumber();
    string str;

    bool isNum[2]={checkNumberIsBiggerThanZero(a),checkNumberIsBiggerThanZero(b)};
    if(isNum[0] && isNum[1])
        str=add(a.toStdString(),b.toStdString());
    else if(!isNum[0] && !isNum[1])
        str="-"+add(a.toStdString(),b.toStdString());
    else if(!isNum[0] && isNum[1])
        str=subtract(b.toStdString(),a.toStdString());
    else if(isNum[0] && !isNum[1])
        str=subtract(a.toStdString(),b.toStdString());

    return BigData(QString::fromStdString(str));
}

string BigData::add(string a, string b)
{
    unsigned long long lenA=a.length();
    unsigned long long lenB=b.length();
    unsigned long long len=max(lenA,lenB)+2;

    reverse(a.begin(),a.end());
    reverse(b.begin(),b.end());

    string result(len,'0');

    for(unsigned long long  x=0;x<lenA;x++)
        result[x]=a[x];

    for(unsigned long long  x=0;x<len;x++)
    {
        int tmp=0;
        if(lenB>x)
            tmp=result[x]-'0'+b[x]-'0';
        else
            tmp=result[x]-'0';
        if(tmp>=10)
        {
            result[x]=tmp-10+'0';
            result[x+1]=result[x+1]-'0'+'1'-'0'+'0';
        }
        else
        {
            result[x]=tmp+'0';
        }
    }
    reverse(result.begin(),result.end());

    return result.substr(result.find_first_not_of('0'));
}

BigData BigData::subtract(BigData num1, BigData num2)
{
    QString a=num1.getNumber();
    QString b=num2.getNumber();
    string str;

    bool isNum[2]={checkNumberIsBiggerThanZero(a),checkNumberIsBiggerThanZero(b)};
    if(isNum[0] && isNum[1])
        str=subtract(a.toStdString(),b.toStdString());
    else if(!isNum[0] && !isNum[1])
        str=subtract(b.toStdString(),a.toStdString());
    else if(!isNum[0] && isNum[1])
        str="-"+add(a.toStdString(),b.toStdString());
    else if(isNum[0] && !isNum[1])
        str=add(a.toStdString(),b.toStdString());

    return BigData(QString::fromStdString(str));
}

string BigData::subtract(string a, string b)
{
    if(a==b)
        return "0";

    bool isAbigger=false;
    if(a.length()>b.length())
        isAbigger=true;
    else if(a.length()==b.length())
    {
        if(a[0]>=b[0])
            isAbigger=true;
    }
    if(isAbigger==false)
    {
        string temp;
        temp=a;
        a=b;
        b=temp;
    }
    unsigned long long lenA=a.length();
    unsigned long long lenB=b.length();
    unsigned long long len=max(a.length(),b.length())+10;

    reverse(a.begin(),a.end());
    reverse(b.begin(),b.end());

    string result(len,'0');

    for(unsigned long long x=0;x<lenA;x++)
        result[x]=a[x];
    for(unsigned long long x=0;x<len;x++)
    {
        int tmp=0;
        if(x<lenB)
            tmp=result[x]-'0'-(b[x]-'0');
        else
            tmp=result[x]-'0';
        if(tmp<0)
        {
            result[x]=tmp+10+'0';
            result[x+1]=result[x+1]-'0'-1+'0';
        }
        else
        {
            result[x]=tmp+'0';
        }
    }

    reverse(result.begin(),result.end());
    result=result.substr(result.find_first_not_of('0'));

    if(isAbigger==false)
    {
        result="-"+result;
    }
    return result;
}

BigData BigData::multiple(BigData num1, BigData num2)
{
    QString a=num1.getNumber();
    QString b=num2.getNumber();
    string str;

    bool isNum[2]={checkNumberIsBiggerThanZero(a),checkNumberIsBiggerThanZero(b)};
    if((isNum[0] && isNum[1]) || (!isNum[0] && !isNum[1]))
        str=multiple(a.toStdString(),b.toStdString());
    else
        str="-"+multiple(a.toStdString(),b.toStdString());

    return BigData(QString::fromStdString(str));
}

string BigData::multiple(string a, string b)
{
    string res="";
    //两个数的位数
    int m = a.size(), n = b.size();

    //一个i位数乘以一个j位数，结果至少是i+j-1位数
    vector<unsigned long long> tmp(m + n - 1);

    //每一位进行笛卡尔乘法
    for (int i = 0; i < m; i++){
         int ax = a[i] - '0';
        for (int j = 0; j < n; j++){
            int bx = b[j] - '0';
            tmp[i + j] += ax*bx;
        }
    }
    //进行进位处理，注意左侧是大右侧是小
    int carry = 0;
    for (int i = tmp.size() - 1; i >= 0; i--){
        int t = tmp[i] + carry;
        tmp[i] = t % 10;
        carry = t / 10;
    }
    //若遍历完仍然有进位
    while (carry != 0){
        int t = carry % 10;
        carry /= 10;
        tmp.insert(tmp.begin(), t);
    }
    //将结果存入到返回值中
    for (auto a : tmp){
        res = res + to_string(a);
    }
    if(res.size()>0&&res[0]=='0')return "0";
    return res;
}

BigData BigData::divide(BigData num1, BigData num2)
{
    QString a=num1.getNumber();
    QString b=num2.getNumber();
    string str;

    bool isNum[2]={checkNumberIsBiggerThanZero(a),checkNumberIsBiggerThanZero(b)};
    if((isNum[0] && isNum[1]) || (!isNum[0] && !isNum[1]))
        str=divide(a.toStdString(),b.toStdString());
    else
        str="-"+divide(a.toStdString(),b.toStdString());

    return BigData(QString::fromStdString(str));
}

string BigData::divide(string a, string b)
{
    unsigned long long lenA=a.length();
    unsigned long long lenB=b.length();

    string result="0";

    if(compare(a,b)==0)
        return "0";
    if(compare(a,b)==1)
        return "1";

    while(compare(a,b))
    {
        if(a[0]>b[0])
        {
            while(compare(a,multiple(b,pow(10,lenA-lenB))))
            {
                a=subtract(a,multiple(b,pow(10,lenA-lenB)));
                result=add(result,multiple("1",pow(10,lenA-lenB)));
            }
            lenA=a.length();
        }
        else if(a[0]<b[0])//因为a>b,a[0]<b[0],此时lenA一定大于lenB
        {
            while(compare(a,multiple(b,pow(10,lenA-lenB-1))))
            {
                a=subtract(a,multiple(b,pow(10,lenA-lenB-1)));
                result=add(result,multiple("1",pow(10,lenA-lenB-1)));
            }
            lenA=a.length();
        }
        else
        {
            if(compare(a,multiple(b,pow(10,lenA-lenB))))
            {
                while(compare(a,multiple(b,pow(10,lenA-lenB))))
                {
                    a=subtract(a,multiple(b,pow(10,lenA-lenB)));
                    result=add(result,multiple("1",pow(10,lenA-lenB)));
                }
                lenA=a.length();
            }
            else
            {
                while(compare(a,multiple(b,pow(10,lenA-lenB-1))))
                {
                    a=subtract(a,multiple(b,pow(10,lenA-lenB-1)));
                    result=add(result,multiple("1",pow(10,lenA-lenB-1)));
                }
                lenA=a.length();
            }
        }
    }

    result=result.substr(result.find_first_not_of('0'));
    return result;
}

BigData BigData::module(BigData num1, BigData num2)
{
    QString a=num1.getNumber();
    QString b=num2.getNumber();
    string str;

    checkNumberIsBiggerThanZero(a);
    checkNumberIsBiggerThanZero(b);
    str=module(a.toStdString(),b.toStdString());

    return BigData(QString::fromStdString(str));
}

string BigData::module(string a, string b)
{
    return subtract(a,multiple(divide(a,b),b));
}

QString BigData::cleanFirstZero(QString &number)
{
    QString str=number;
    for(int x=0;x<legalCharacter.length();x++)
        str.replace(legalCharacter.at(x),"");
    if(!str.isEmpty())
    {
        qDebug()<<"The number is illegal.";
        number="The number is illegal.";
        return "The number is illegal.";
    }

    bool isLessThanZero=false;
    if(number.contains("-"))
    {
        number.remove(0,1);
        isLessThanZero=true;
    }
    number.replace("+","");
    number.replace(",","");
    while(number.at(0)=="0" && number.length()>1)
    {
        number.remove(0,1);
    }
    if(isLessThanZero==true)
        number="-"+number;
    return number;
}

bool BigData::checkNumberIsBiggerThanZero(QString &number)
{
    if(number.at(0)=="-")
    {
        number.replace("-","");
        return false;
    }
    else
        return true;
}

int BigData::compare(BigData a, BigData b)
{
    int lenA=a.number.length();
    int lenB=b.number.length();
    QString num1=a.number;
    QString num2=b.number;

    if(lenA<lenB)
        return 0;
    else if(lenA==lenB)
    {
        for(int x=0;x<lenA;x++)
        {
            QString s1=num1.at(x);
            QString s2=num2.at(x);
            if(s1.toInt()<s2.toInt())
                return 0;
            if(s1.toInt()>s2.toInt())
                return 2;
        }
    }
    else
        return 2;
    return 1;
}

int BigData::compare(string a, string b)
{
    unsigned long long lenA=a.length();
    unsigned long long lenB=b.length();

    if(lenA<lenB)
        return 0;
    else if(lenA==lenB)
    {
        for(unsigned long long x=0;x<lenA;x++)
        {
            if(a[x]<b[x])
                return 0;
            if(a[x]>b[x])
                return 2;
        }
    }
    else
        return 2;
    return 1;
}

string BigData::pow(string num, unsigned long long times)
{
    string result="1";
    for(unsigned long long x=0;x<times;x++)
    {
        result=multiple(result,num);
    }
    return result;
}

string BigData::pow(unsigned long long num, unsigned long long times)
{
    string result="1";
    for(unsigned long long x=0;x<times;x++)
    {
        result=multiple(result,to_string(num));
    }
    return result;
}




BigData BigData::operator+(BigData number)
{
    if(this->number.contains("illegal") || number.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(add(this->number,number.number));
    return res;
}

BigData BigData::operator+(int number)
{
    if(this->number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(add(this->number,QString::number(number)));
    return res;
}

BigData operator+(int number2,BigData number)
{
    if(number.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(number.add(QString::number(number2),number.number));
    return res;
}

BigData BigData::operator+(long number)
{
    if(this->number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(add(this->number,QString::number(number)));
    return res;
}

BigData operator+(long number2, BigData number)
{
    if(number.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(number.add(QString::number(number2),number.number));
    return res;
}

BigData BigData::operator+(long long number)
{
    if(this->number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(add(this->number,QString::number(number)));
    return res;
}

BigData operator+(long long number2, BigData number)
{
    if(number.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(number.add(QString::number(number2),number.number));
    return res;
}

BigData BigData::operator+(QString number)
{
    BigData b(number);
    if(this->number.contains("illegal") || b.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(add(this->number,b));
    return res;
}

BigData operator+(QString number2, BigData number)
{
    BigData b(number);
    if(number.number.contains("illegal") || b.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(number.add(number2,number.number));
    return res;
}

BigData BigData::operator++()
{
    *this=*this+*this;
    return *this;
}

BigData BigData::operator++(int)
{
    BigData b=*this;
    *this=add(*this,1);
    return b;
}


BigData BigData::operator-(BigData number)
{
    if(this->number.contains("illegal") || number.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(subtract(this->number,number.number));
    return res;
}

BigData BigData::operator-(int number)
{
    if(this->number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(subtract(this->number,QString::number(number)));
    return res;
}

BigData operator-(int number2,BigData number)
{
    if(number.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(number.subtract(QString::number(number2),number.number));
    return res;
}

BigData BigData::operator-(long number)
{
    if(this->number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(subtract(this->number,QString::number(number)));
    return res;
}

BigData operator-(long number2, BigData number)
{
    if(number.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(number.subtract(QString::number(number2),number.number));
    return res;
}

BigData BigData::operator-(long long number)
{
    if(this->number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(subtract(this->number,QString::number(number)));
    return res;
}

BigData operator-(long long number2, BigData number)
{
    if(number.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(number.subtract(QString::number(number2),number.number));
    return res;
}

BigData BigData::operator-(QString number)
{
    BigData b(number);
    if(this->number.contains("illegal") || b.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(subtract(this->number,b));
    return res;
}

BigData operator-(QString number2, BigData number)
{
    BigData b(number);
    if(number.number.contains("illegal") || b.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(number.subtract(number2,number.number));
    return res;
}

BigData BigData::operator--()
{
    *this=*this-*this;
    return *this;
}

BigData BigData::operator--(int)
{
    BigData b=*this;
    *this=subtract(*this,1);
    return b;
}


BigData BigData::operator*(BigData number)
{
    if(this->number.contains("illegal") || number.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(multiple(this->number,number.number));
    return res;
}

BigData BigData::operator*(int number)
{
    if(this->number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(multiple(this->number,QString::number(number)));
    return res;
}

BigData operator*(int number2,BigData number)
{
    if(number.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(number.multiple(QString::number(number2),number.number));
    return res;
}

BigData BigData::operator*(long number)
{
    if(this->number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(multiple(this->number,QString::number(number)));
    return res;
}

BigData operator*(long number2, BigData number)
{
    if(number.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(number.multiple(QString::number(number2),number.number));
    return res;
}

BigData BigData::operator*(long long number)
{
    if(this->number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(multiple(this->number,QString::number(number)));
    return res;
}

BigData operator*(long long number2, BigData number)
{
    if(number.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(number.multiple(QString::number(number2),number.number));
    return res;
}

BigData BigData::operator*(QString number)
{
    BigData b(number);
    if(this->number.contains("illegal") || b.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(multiple(this->number,b));
    return res;
}

BigData operator*(QString number2, BigData number)
{
    BigData b(number);
    if(number.number.contains("illegal") || b.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    BigData res(number.multiple(number2,number.number));
    return res;
}


BigData BigData::operator/(BigData number)
{
    if(this->number.contains("illegal") || number.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    if(number.number=="0")
    {
        qDebug()<<"The number is zero, error.";
        return BigData("The number is zero, error.");
    }
    BigData res(divide(this->number,number.number));
    return res;
}

BigData BigData::operator/(int number)
{
    if(this->number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    if(number==0)
    {
        qDebug()<<"The number is zero, error.";
        return BigData("The number is zero, error.");
    }
    BigData res(divide(this->number,QString::number(number)));
    return res;
}

BigData operator/(int number2,BigData number)
{
    if(number.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    if(number.number=="0")
    {
        qDebug()<<"The number is zero, error.";
        return BigData("The number is zero, error.");
    }
    BigData res(number.divide(QString::number(number2),number.number));
    return res;
}

BigData BigData::operator/(long number)
{
    if(this->number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    if(number==0)
    {
        qDebug()<<"The number is zero, error.";
        return BigData("The number is zero, error.");
    }
    BigData res(divide(this->number,QString::number(number)));
    return res;
}

BigData operator/(long number2, BigData number)
{
    if(number.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    if(number.number=="0")
    {
        qDebug()<<"The number is zero, error.";
        return BigData("The number is zero, error.");
    }
    BigData res(number.divide(QString::number(number2),number.number));
    return res;
}

BigData BigData::operator/(long long number)
{
    if(this->number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    if(number==0)
    {
        qDebug()<<"The number is zero, error.";
        return BigData("The number is zero, error.");
    }
    BigData res(divide(this->number,QString::number(number)));
    return res;
}

BigData operator/(long long number2, BigData number)
{
    if(number.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    if(number.number=="0")
    {
        qDebug()<<"The number is zero, error.";
        return BigData("The number is zero, error.");
    }
    BigData res(number.divide(QString::number(number2),number.number));
    return res;
}

BigData BigData::operator/(QString number)
{
    BigData b(number);
    if(this->number.contains("illegal") || b.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    if(number=="0")
    {
        qDebug()<<"The number is zero, error.";
        return BigData("The number is zero, error.");
    }
    BigData res(divide(this->number,b));
    return res;
}

BigData operator/(QString number2, BigData number)
{
    BigData b(number);
    if(number.number.contains("illegal") || b.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    if(number.number=="0")
    {
        qDebug()<<"The number is zero, error.";
        return BigData("The number is zero, error.");
    }
    BigData res(number.divide(number2,number.number));
    return res;
}


BigData BigData::operator%(BigData number)
{
    if(this->number.contains("illegal") || number.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    bool isNum[2]={checkNumberIsBiggerThanZero(this->number),checkNumberIsBiggerThanZero(number.number)};
    if(!(isNum[0] && isNum[1]))
    {
        qDebug()<<"The number is less than zero, deny culculate.";
        return BigData("The number is zero, deny culculate.");
    }
    BigData res(module(this->number,number.number));
    return res;
}

BigData BigData::operator%(int number)
{
    if(this->number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    QString str=QString::number(number);
    bool isNum[2]={checkNumberIsBiggerThanZero(this->number),checkNumberIsBiggerThanZero(str)};
    if(!(isNum[0] && isNum[1]))
    {
        qDebug()<<"The number is less than zero, deny culculate.";
        return BigData("The number is zero, deny culculate.");
    }
    BigData res(module(this->number,QString::number(number)));
    return res;
}

BigData operator%(int number2,BigData number)
{
    if(number.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    QString str=QString::number(number2);
    bool isNum[2]={number.checkNumberIsBiggerThanZero(number.number)
                   ,number.checkNumberIsBiggerThanZero(str)};
    if(!(isNum[0] && isNum[1]))
    {
        qDebug()<<"The number is less than zero, deny culculate.";
        return BigData("The number is zero, deny culculate.");
    }
    BigData res(number.module(QString::number(number2),number.number));
    return res;
}

BigData BigData::operator%(long number)
{
    if(this->number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    QString str=QString::number(number);
    bool isNum[2]={checkNumberIsBiggerThanZero(this->number),checkNumberIsBiggerThanZero(str)};
    if(!(isNum[0] && isNum[1]))
    {
        qDebug()<<"The number is less than zero, deny culculate.";
        return BigData("The number is zero, deny culculate.");
    }
    BigData res(module(this->number,QString::number(number)));
    return res;
}

BigData operator%(long number2, BigData number)
{
    if(number.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    QString str=QString::number(number2);
    bool isNum[2]={number.checkNumberIsBiggerThanZero(number.number)
                   ,number.checkNumberIsBiggerThanZero(str)};
    if(!(isNum[0] && isNum[1]))
    {
        qDebug()<<"The number is less than zero, deny culculate.";
        return BigData("The number is zero, deny culculate.");
    }
    BigData res(number.module(QString::number(number2),number.number));
    return res;
}

BigData BigData::operator%(long long number)
{
    if(this->number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    QString str=QString::number(number);
    bool isNum[2]={checkNumberIsBiggerThanZero(this->number),checkNumberIsBiggerThanZero(str)};
    if(!(isNum[0] && isNum[1]))
    {
        qDebug()<<"The number is less than zero, deny culculate.";
        return BigData("The number is zero, deny culculate.");
    }
    BigData res(module(this->number,QString::number(number)));
    return res;
}

BigData operator%(long long number2, BigData number)
{
    if(number.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    QString str=QString::number(number2);
    bool isNum[2]={number.checkNumberIsBiggerThanZero(number.number)
                   ,number.checkNumberIsBiggerThanZero(str)};
    if(!(isNum[0] && isNum[1]))
    {
        qDebug()<<"The number is less than zero, deny culculate.";
        return BigData("The number is zero, deny culculate.");
    }
    BigData res(number.module(QString::number(number2),number.number));
    return res;
}

BigData BigData::operator%(QString number)
{
    BigData b(number);
    if(this->number.contains("illegal") || b.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    bool isNum[2]={checkNumberIsBiggerThanZero(this->number),checkNumberIsBiggerThanZero(number)};
    if(!(isNum[0] && isNum[1]))
    {
        qDebug()<<"The number is less than zero, deny culculate.";
        return BigData("The number is zero, deny culculate.");
    }
    BigData res(module(this->number,b));
    return res;
}

BigData operator%(QString number2, BigData number)
{
    BigData b(number);
    if(number.number.contains("illegal") || b.number.contains("illegal"))
    {
        qDebug()<<"The number is illegal.";
        return BigData("The number is illegal.");
    }
    bool isNum[2]={number.checkNumberIsBiggerThanZero(number.number)
                   ,number.checkNumberIsBiggerThanZero(number2)};
    if(!(isNum[0] && isNum[1]))
    {
        qDebug()<<"The number is less than zero, deny culculate.";
        return BigData("The number is zero, deny culculate.");
    }
    BigData res(number.module(number2,number.number));
    return res;
}


BigData BigData::operator-()
{
    if(this->number.contains("-"))
        this->number.replace("-","");
    else
        this->number="-"+this->number;
    BigData res(this->number);
    return res;
}

bool BigData::operator==(BigData number)
{
    if(compare(*this,number)==1)
        return true;
    else
        return false;
}

bool BigData::operator!=(BigData number)
{
    if(compare(*this,number)!=1)
        return true;
    else
        return false;
}

bool BigData::operator>(BigData number)
{
    if(compare(*this,number)==2)
        return true;
    else
        return false;
}

bool BigData::operator<(BigData number)
{
    if(compare(*this,number)==0)
        return true;
    else
        return false;
}

bool BigData::operator>=(BigData number)
{
    if(compare(*this,number)>=1)
        return true;
    else
        return false;
}

bool BigData::operator<=(BigData number)
{
    if(compare(*this,number)<=1)
        return true;
    else
        return false;
}

QDebug operator<<(QDebug dbg, BigData number)
{
    dbg.nospace()<<"BigData("<<number.number<<")";
    return dbg.maybeSpace();
}
