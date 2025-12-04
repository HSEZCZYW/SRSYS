// ComputerRoom.h
#ifndef COMPUTERROOM_H
#define COMPUTERROOM_H

#include <QString>
#include <QVector>

struct Order {
    int date;
    int interval;
    QString stuId;
    QString stuName;
    int machine;   // 机器号
    int persons;   // 几人共用一台机器
};

class ComputerRoom
{
public:
    virtual ~ComputerRoom() = default;
    QVector<Order> m_orders;
    QString name() const { return m_name; }
    // 在 ComputerRoom.h 里直接写（最简单）
    ComputerRoom(const QString& name, int total)
        : m_name(name), m_totalMachines(total)
    {
    }
    //多态实现剩余机位查看、机位预约、取消预约（这里只考虑几人用一台机器的逻辑），为了查询方便做，用vector数组实现存储
    virtual int  freeSeats(int date, int interval) const = 0;
    virtual bool reserve(int date, int interval,
                         const QString& stuId, const QString& stuName,
                         int persons, int& outMachine) = 0;
    virtual void cancel(int date, int interval, const QString& stuId) = 0;

protected:
    QString m_name;
    int m_totalMachines;           // 总机器数

};

#endif
