#include "students_applyorder_window.h"
#include "ui_students_applyorder_window.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QTime>
#include <QRegularExpression>
#include "globalfile.h"

Students_applyOrder_Window::Students_applyOrder_Window(const student& stu, QWidget *parent)
    : QDialog(parent), ui(new Ui::Students_applyOrder_Window), m_student(stu)
{
    ui->setupUi(this);
    setWindowTitle("申请机房预约");

    // 日期
    int todayWeekday = QDate::currentDate().dayOfWeek(); // 1=周一
    QStringList days = {"周一", "周二", "周三", "周四", "周五"};

    if (todayWeekday <= 5) { // 周一到周五
        for (int i = todayWeekday - 1; i < 5; ++i) {
            ui->comboBox_date->addItem(days[i]);
        }
    } else { // 周六周日
        for (int i = 0; i < 5; ++i) {
            ui->comboBox_date->addItem(days[i]); // 下周一到周五
        }
    }

    // 时间段
    ui->comboBox_time->addItem("08:00 - 11:00");
    ui->comboBox_time->addItem("14:00 - 17:00");
    ui->comboBox_time->addItem("19:00 - 22:00");

    // 人数选择框
    ui->comboBox_persons->addItem("1人");
    ui->comboBox_persons->addItem("2人");
    ui->comboBox_persons->hide();

    loadComputerRooms();

    connect(ui->comboBox_room, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Students_applyOrder_Window::on_comboBox_room_currentIndexChanged);
}

Students_applyOrder_Window::~Students_applyOrder_Window()
{
    for (auto room : m_rooms) delete room;
    delete ui;
}

void Students_applyOrder_Window::loadComputerRooms()
{
    for (auto r : m_rooms) delete r;
    m_rooms.clear();
    ui->comboBox_room->clear();

    QFile file(COMPUTER_FILE);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "错误", "无法打开机房文件！");
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        auto parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (parts.size() < 2) continue;

        int id = parts[0].toInt();
        int machines = parts[1].toInt();

        ComputerRoom* room = (id <= 3)
                                 ? static_cast<ComputerRoom*>(new PublicRoom(QString("%1号机房").arg(id), machines))
                                 : static_cast<ComputerRoom*>(new LabRoom(QString("%1号机房").arg(id), machines));

        m_rooms.append(room);
        QString type = (id <= 3) ? "普通机房（一人一机）" : "实验室（支持两人拼机）";
        ui->comboBox_room->addItem(QString("%1 - %2台 - %3").arg(id).arg(machines).arg(type));
    }
    file.close();

    QFile orderFile(ORDER_FILE);
    if (orderFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream orderIn(&orderFile);
        while (!orderIn.atEnd()) {
            QString line = orderIn.readLine().trimmed();
            if (line.isEmpty()) continue;

            QRegularExpression re("(\\w+):([^ |]+)");
            QRegularExpressionMatchIterator it = re.globalMatch(line);

            Order order;
            order.persons = 1;  // 默认1人
            int roomId = -1;
            int status = 0;

            while (it.hasNext()) {
                QRegularExpressionMatch match = it.next();
                QString key = match.captured(1);
                QString value = match.captured(2);
                if (key == "date") order.date = value.toInt();
                else if (key == "interval") order.interval = value.toInt();
                else if (key == "studentID") order.stuId = value;
                else if (key == "studentName") order.stuName = value;
                else if (key == "roomID") roomId = value.toInt();
                else if (key == "seat") order.machine = value.toInt();
                else if (key == "status") status = value.toInt();
            }
            if (line.contains("|persons:2")) {
                order.persons = 2;
            }

            if (status == 1 && roomId > 0 && roomId <= m_rooms.size()) {
                m_rooms[roomId - 1]->m_orders.append(order);
            }
        }
        orderFile.close();
    }

    if (m_rooms.isEmpty()) {
        ui->comboBox_room->addItem("暂无机房");
    } else {
        on_comboBox_room_currentIndexChanged(0);
    }
}

void Students_applyOrder_Window::on_comboBox_room_currentIndexChanged(int index)
{
    if (index < 0 || index >= m_rooms.size()) {
        ui->comboBox_persons->hide();
        return;
    }
    bool isLab = (index >= 3);  // 4、5号机房是实验室
    ui->comboBox_persons->setVisible(isLab);
}

void Students_applyOrder_Window::on_pushButton_sure_clicked()
{
    int dateIdx  = ui->comboBox_date->currentIndex();
    int interval = ui->comboBox_time->currentIndex() + 1;
    int roomIdx  = ui->comboBox_room->currentIndex();

    if (dateIdx == -1 || roomIdx == -1) {
        QMessageBox::warning(this, "提示", "请完整选择日期、时间和机房！");
        return;
    }

    int date = QDate::currentDate().dayOfWeek() + dateIdx;
    if (date > 7) date -= 7;

    QTime now = QTime::currentTime();
    if (dateIdx == 0) {
        if ((interval == 1 && now.hour() >= 11) ||
            (interval == 2 && now.hour() >= 17) ||
            (interval == 3 && now.hour() >= 22)) {
            QMessageBox::warning(this, "失败", "该时间段已过！");
            return;
        }
    }

    ComputerRoom* room = m_rooms[roomIdx];

    QString stuId   = QString::fromStdString(m_student.my_studentID);
    QString stuName = QString::fromStdString(m_student.my_name);

    // 重复预约检查
    for (const auto& o : room->m_orders) {
        if (o.date == date && o.interval == interval && o.stuId == stuId) {
            QMessageBox::warning(this, "失败", "您已预约过该时段该机房！");
            return;
        }
    }

    int persons = (roomIdx >= 3) ? (ui->comboBox_persons->currentIndex() + 1) : 1;

    int machineNo = -1;
    bool success = room->reserve(date, interval, stuId, stuName, persons, machineNo);

    if (!success) {
        QMessageBox::warning(this, "预约失败",
                             roomIdx >= 3 ? "实验室已满（每机最多2人）" : "该机房已满员");
        return;
    }

    // 写入订单文件
    QFile file(ORDER_FILE);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << "date:" << date << " "
            << "interval:" << interval << " "
            << "studentID:" << stuId << " "
            << "studentName:" << stuName << " "
            << "roomID:" << (roomIdx + 1) << " "
            << "seat:" << machineNo << " "
            << "status:1";
        if (persons == 2) {
            out << " |persons:2";
        }
        out << "\n";

        file.close();
    }
    // 写入订单文件
    QFile file2(ORDER_READ);
    if (file2.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file2);
        out << "date:" << date << " "
            << "interval:" << interval << " "
            << "studentID:" << stuId << " "
            << "studentName:" << stuName << " "
            << "roomID:" << (roomIdx + 1) << " "
            << "seat:" << machineNo << " "
            << "status:1";
        if (persons == 2) {
            out << " |persons:2";
        }
        out << "\n";
        file2.close();
    }
    QMessageBox::information(this, "成功",
                             QString("预约成功！\n%1\n机器号：%2\n%3")
                                 .arg(room->name()).arg(machineNo)
                                 .arg(persons == 2 ? "（两人拼机）" : "（一人使用）"));

    accept();
}

void Students_applyOrder_Window::on_pushButton_cancel_clicked()
{
    reject();
}
