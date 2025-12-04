#pragma once
#include<iostream>
#include"identity.h"
#include<vector>
#include"teacher.h"
#include"student.h"
#include<algorithm>
#include"computerRoom.h"

using namespace std;

class manager:public identity
{
public:
	manager();

	manager(string name, string pswd);


	string myName; string mypswd;
};

