#pragma once
#include<iostream>
#include<fstream>
#include"globalfile.h"
#include<map>
#include<string>
#include<QVector>
using namespace std;

class orderFile
{
public:
	//构造函数
	orderFile();

	//更新预约记录
	void updateOrder();

	//分离提取键值对
	void parseField(const string& field, map<string, string>& m);
	

	//记录预约条数
    int Ordersize=0;

	//记录所有预约信息的容器 key表示预约条数   value记录预约键值对具体信息
    QVector<map<string, string>> my_orderData;
};

