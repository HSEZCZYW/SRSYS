#ifndef PUBLICROOM_H
#define PUBLICROOM_H

#include "ComputerRoom.h"

class PublicRoom : public ComputerRoom
{
public:
    PublicRoom(const QString& name, int machines)
        : ComputerRoom(name, machines) {}

    int freeSeats(int date, int interval) const override;
    bool reserve(int date, int interval,
                 const QString& stuId, const QString& stuName,
                 int persons, int& outMachine) override;
    void cancel(int date, int interval, const QString& stuId) override;
};

#endif
