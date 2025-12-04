#ifndef LABROOM_H   // 这三行是防重定义神器！！！
#define LABROOM_H

#include "ComputerRoom.h"

class LabRoom : public ComputerRoom
{
public:
    LabRoom(const QString& name, int machines)
        : ComputerRoom(name, machines) {}

    int freeSeats(int date, int interval) const override;
    bool reserve(int date, int interval,
                 const QString& stuId, const QString& stuName,
                 int persons, int& outMachine) override;
    void cancel(int date, int interval, const QString& stuId) override;
};

#endif
