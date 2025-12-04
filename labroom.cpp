#include "LabRoom.h"
#include<QSet>
int LabRoom::freeSeats(int date, int interval) const
{
    int usedPersons = 0;
    for (const auto& o : m_orders)
    {
        if (o.date == date && o.interval == interval)usedPersons += o.persons;
    }
    return m_totalMachines * 2 - usedPersons;
}

bool LabRoom::reserve(int date, int interval,const QString& stuId, const QString& stuName,int persons, int& outMachine)
{
    if (persons != 1 && persons != 2) return false;
    if (freeSeats(date, interval) < persons) return false;

    QSet<int> used;
    int half = -1;
    for (const auto& o : m_orders)
    {
        if (o.date == date && o.interval == interval)
        {
            used << o.machine;
            if (o.persons == 1) half = o.machine;
        }
    }

    int chosen;
    if (persons == 1 && half != -1)// 单人优先拼机
        chosen = half;
    else
    {
        for (int i = 1; i <= m_totalMachines; ++i)
            if (!used.contains(i)) { chosen = i; break; }
        if (used.size() >= m_totalMachines) return false;
    }

    // 如果是拼机，修改原有记录
    if (persons == 1 && half != -1)
    {
        for (int j = 0; j < m_orders.size(); ++j)
        {
            if (m_orders[j].machine == half &&
                m_orders[j].date == date &&
                m_orders[j].interval == interval &&
                m_orders[j].persons == 1)
            {
                m_orders[j].persons = 2;
                break;
            }
        }
    }

    m_orders.append({date, interval, stuId, stuName, chosen, persons});
    outMachine = chosen;
    return true;
}

void LabRoom::cancel(int date, int interval, const QString& stuId)
{
    for (int i = 0; i < m_orders.size(); ++i)
    {
        if (m_orders[i].date == date &&
            m_orders[i].interval == interval &&
            m_orders[i].stuId == stuId)
        {
            m_orders.removeAt(i);
            return;
        }
    }
}
