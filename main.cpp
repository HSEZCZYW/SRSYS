#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include"computerRoom.h"
#include"labroom.h"
#include"publicroom.h"
// 全局机房列表（所有窗口共用！）
QVector<ComputerRoom*> g_allRooms;

void initAllRoomsAndOrders();  // 前置声明

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font("Microsoft YaHei", 10);
    a.setFont(font);

    initAllRoomsAndOrders();

    MainWindow w;
    w.show();

    return a.exec();
}

void initAllRoomsAndOrders()
{
    g_allRooms.clear();

    QFile file(COMPUTER_FILE);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "严重错误：无法打开 computerRoom.txt！";
        return;
    }

    QTextStream in(&file);
    QVector<int> caps;
    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        QStringList parts = line.split(" ", Qt::SkipEmptyParts);
        if (parts.size() >= 2)
            caps << parts[1].toInt();
    }
    file.close();

    while (caps.size() < 5) caps << 50;

    // 创建机房对象
    g_allRooms << new PublicRoom("1号公共机房", caps[0]);
    g_allRooms << new PublicRoom("2号公共机房", caps[1]);
    g_allRooms << new PublicRoom("3号公共机房", caps[2]);
    g_allRooms << new LabRoom   ("数字电路实验室", caps[3]);  // 两人一机
    g_allRooms << new LabRoom   ("计组实验室",     caps[4]);  // 两人一机

    qDebug() << "成功创建5个机房：3公共 + 数字电路 + 计组实验室";

    //恢复历史预约
    QFile orderFile("D:/system/SRSYS/order.txt");  // 改成你的真实路径！
    if (!orderFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "order.txt 不存在（第一次运行正常）";
        return;
    }

    QTextStream inOrder(&orderFile);
    while (!inOrder.atEnd())
    {
        QString line = inOrder.readLine().trimmed();
        if (line.isEmpty()) continue;

        QMap<QString, QString> map;
        for (QString pair : line.split(" "))
        {
            auto kv = pair.split(":");
            if (kv.size() == 2) map[kv[0]] = kv[1];
        }

        if (map["status"] != "1") continue;

        int date = map["date"].toInt();
        int interval = map["interval"].toInt();
        QString stuId = map["studentID"];
        QString stuName = map["studentName"];
        int roomId = map["roomID"].toInt();
        int seat = map["seat"].toInt();

        int index = roomId - 1;
        if (index >= 0 && index < g_allRooms.size())
        {
            ComputerRoom* room = g_allRooms[index];
            int temp;
            room->reserve(date, interval, stuId, stuName, 1, temp);  // 先占位
            if (!room->m_orders.isEmpty())
                room->m_orders.last().machine = seat;  // 强制恢复原始座位号
        }
    }
    orderFile.close();
    qDebug() << "历史预约记录已全部恢复！";
}
