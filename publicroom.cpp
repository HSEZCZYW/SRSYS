// PublicRoom.cpp
#include "PublicRoom.h"
#include<QSet>

int PublicRoom::freeSeats(int date, int interval) const
{
    int used = 0;
    for (const auto& o : m_orders)
        if (o.date == date && o.interval == interval) ++used;
    return m_totalMachines - used;
}

bool PublicRoom::reserve(int date, int interval,
                         const QString& stuId, const QString& stuName,
                         int persons, int& outMachine)
{
    if (persons != 1 || freeSeats(date, interval) <= 0) return false;

    QSet<int> used;
    for (const auto& o : m_orders)
        if (o.date == date && o.interval == interval)
            used << o.machine;

    for (int i = 1; i <= m_totalMachines; ++i)
        if (!used.contains(i))
        {
            m_orders.append({date, interval, stuId, stuName, i, 1});
            outMachine = i;
            return true;
        }
    return false;
}

void PublicRoom::cancel(int date, int interval, const QString& stuId)
{
    for (int i = 0; i < m_orders.size(); ++i)
        if (m_orders[i].date == date && m_orders[i].interval == interval &&
            m_orders[i].stuId == stuId)
        {
            m_orders.removeAt(i);
            return;
        }
}
