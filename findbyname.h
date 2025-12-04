// FindByName.h
#pragma once
#include "PersonFinder.h"
#include "globalfile.h"
#include <fstream>
#include <sstream>

class FindByName : public PersonFinder
{
public:
    QString find(const QString& keyword) ;

};
