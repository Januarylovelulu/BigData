# BigData
用Qt实现的大数运算类，包括四则运算和取模运算

制作了许多运算符重载，类和类，类和变量直接用+-*/就可以运算

用例：

BigData b1("100");//初始化为100

BigData b2(10);

BigData b3;//默认初始化为0

qDebug()<<b1+100; //200

qDebug()<<100+b1+"100"; //300

qDebug()<<b1.length(); //3



缺点&注意事项
 * 只处理整数，其他数为非法数
 * 除法会省略余数，获取余数用取余操作
 * 取余操作时，两边的数字必须为正(因为负数取余时没有统一标准)
 * 除法运算效率有待提高，在我的电脑测试486位数÷163位数需要大约7秒钟
