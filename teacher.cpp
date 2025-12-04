#include "teacher.h"
#include"orderFile.h"
//默认构造
teacher::teacher(){}

teacher::teacher(string emID, string name, string pswd)
{
	this->my_employID = emID;
	this->my_name = name;
	this->my_password = pswd;
}
