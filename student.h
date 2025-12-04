#pragma once
#include"identity.h"
#include<vector>
#include"computerRoom.h"
#include"orderFile.h"
using namespace std;

class student:public identity
{
public:
	//默认构造
	student();
	//有参构造
	student(string myID, string pswd, string name);
    //学生学号
    string my_studentID;
};

