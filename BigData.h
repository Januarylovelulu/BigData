#ifndef BIGDATA_H
#define BIGDATA_H

#include <QString>
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

/*
 * 只处理整数，其他数为非法数
 * 除法会省略余数，获取余数用取余操作
 * 取余操作时，两边的数字必须为正(因为负数取余时没有统一标准)
 * 除法运算效率有待提高，在我的电脑测试486位数÷163位数需要大约7秒钟
*/

class BigData
{
public:
    BigData(QString number="0");
    BigData(int number);
    BigData(long long number);
    void setNumber(QString number);
    QString getNumber();
    int length();

protected:
    BigData add(BigData num1, BigData num2);
    string add(string a,string b);

    BigData subtract(BigData num1, BigData num2);
    string subtract(string a,string b);

    BigData multiple(BigData num1, BigData num2);
    string multiple(string a,string b);

    BigData divide(BigData num1, BigData num2);
    string divide(string a,string b);

    BigData module(BigData num1, BigData num2);
    string module(string a,string b);

    QString cleanFirstZero(QString &number); //除去数字开头的0，+, 计数的,(逗号)，符号后面开头的0，并且检测数字是否合法
    bool checkNumberIsBiggerThanZero(QString &number);
    int compare(BigData a,BigData b); //0:a<b, 1:a==b, 2:a>b
    int compare(string a,string b); //0:a<b, 1:a==b, 2:a>b

    string pow(string num, unsigned long long times);
    string pow(unsigned long long num, unsigned long long times);

private:
    QString number;

    QString legalCharacter="0123456789+-,";

public:
    BigData operator+(BigData number);
    BigData operator+(int number);
    friend BigData operator+(int number2,BigData number);
    BigData operator+(long number);
    friend BigData operator+(long number2,BigData number);
    BigData operator+(long long number);
    friend BigData operator+(long long number2,BigData number);
    BigData operator+(QString number);
    friend BigData operator+(QString number2,BigData number);
    BigData operator++(); //前自增
    BigData operator++(int); //后自增

    BigData operator-(BigData number);
    BigData operator-(int number);
    friend BigData operator-(int number2,BigData number);
    BigData operator-(long number);
    friend BigData operator-(long number2,BigData number);
    BigData operator-(long long number);
    friend BigData operator-(long long number2,BigData number);
    BigData operator-(QString number);
    friend BigData operator-(QString number2,BigData number);
    BigData operator--(); //前自减
    BigData operator--(int); //后自减

    BigData operator*(BigData number);
    BigData operator*(int number);
    friend BigData operator*(int number2,BigData number);
    BigData operator*(long number);
    friend BigData operator*(long number2,BigData number);
    BigData operator*(long long number);
    friend BigData operator*(long long number2,BigData number);
    BigData operator*(QString number);
    friend BigData operator*(QString number2,BigData number);

    BigData operator/(BigData number);
    BigData operator/(int number);
    friend BigData operator/(int number2,BigData number);
    BigData operator/(long number);
    friend BigData operator/(long number2,BigData number);
    BigData operator/(long long number);
    friend BigData operator/(long long number2,BigData number);
    BigData operator/(QString number);
    friend BigData operator/(QString number2,BigData number);

    BigData operator%(BigData number);
    BigData operator%(int number);
    friend BigData operator%(int number2,BigData number);
    BigData operator%(long number);
    friend BigData operator%(long number2,BigData number);
    BigData operator%(long long number);
    friend BigData operator%(long long number2,BigData number);
    BigData operator%(QString number);
    friend BigData operator%(QString number2,BigData number);

    BigData operator-(); //相反数
    bool operator==(BigData number);
    bool operator!=(BigData number);
    bool operator>(BigData number);
    bool operator<(BigData number);
    bool operator>=(BigData number);
    bool operator<=(BigData number);

    friend QDebug operator<<(QDebug dbg,BigData number);
};

#endif // BIGDATA_H
