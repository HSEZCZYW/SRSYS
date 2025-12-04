#include "student.h"
#include<fstream>
#include"globalfile.h"
#include<vector>
#include"computerRoom.h"

using namespace std;


student::student(){}


student::student(string myID, string pswd, string name)
{
	this->my_name = name;
	this->my_studentID = myID;
	this->my_password = pswd;

}
