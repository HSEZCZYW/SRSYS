#include "manager.h"
#include<fstream>
#include<iostream>
#include"globalfile.h"


using namespace std;
manager::manager(){}

manager::manager(string name, string pswd)
{
    this->myName = name;
    this->mypswd = pswd;


}


