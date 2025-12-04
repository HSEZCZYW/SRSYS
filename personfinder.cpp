#include "personfinder.h"

PersonFinder::PersonFinder() {}
#pragma once
#include <QString>

class PersonFinder
{
public:
    virtual ~PersonFinder() = default;
    virtual QString find(const QString& keyword) = 0;  // 纯虚函数，返回查找结果
};
