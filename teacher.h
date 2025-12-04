#pragma once
#include"identity.h"
#include<iostream>
#include<vector>

using namespace std;

class teacher:public identity
{
public:
	teacher();

	teacher(string emID, string name, string pswd);
    string my_employID;
};

