#ifndef PERSONFINDER_H
#define PERSONFINDER_H
#include <QString>

class PersonFinder
{
public:
    virtual ~PersonFinder() = default;
    virtual QString find(const QString& keyword) = 0;
};
#endif // PERSONFINDER_H
