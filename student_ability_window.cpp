#include "student_ability_window.h"
#include "ui_student_ability_window.h"
#include<QFile>
#include<QRegularExpression>
Student_ability_Window::Student_ability_Window(const student& stu,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Student_ability_Window)
    ,m_student(stu)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Student_ability_Window::updateTime);
    timer->start(1000);        // 每1000毫秒（1秒）触发一次
    updateTime();              // 立刻显示一次当前时间
}

Student_ability_Window::~Student_ability_Window()
{
    delete ui;
}

void Student_ability_Window::updateTime()
{
    QString timeStr = QDateTime::currentDateTime().toString("yyyy年MM月dd日 hh:mm:ss dddd");
    ui->label_time->setText(timeStr);
    ui->label_time->setStyleSheet("color: cyan; font: bold 16px; background-color: rgba(0,0,0,100); padding: 5px;");
}
void Student_ability_Window::on_pushButton_applyOrder_clicked()
{
    // 创建预约填写弹窗，把当前登录的学生传进去
    Students_applyOrder_Window dlg(m_student, this);
    dlg.setWindowTitle("申请机房预约");

    if (dlg.exec() == QDialog::Accepted)
    {
        QMessageBox::information(this, "成功", "预约申请已提交，请等待审核！");
    }
}


void Student_ability_Window::on_pushButton_checkMyOrder_clicked()
{
    // 读取历史记录文件（包含已取消、审核中、成功等所有记录）
    QFile file(ORDER_READ);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::information(this, "我的预约", "暂无任何预约记录");
        return;
    }
    QString content = file.readAll();
    file.close();

    if (content.trimmed().isEmpty()) {
        QMessageBox::information(this, "我的预约", "暂无任何预约记录");
        return;
    }

    const QStringList WEEK_DAYS = {"", "周一", "周二", "周三", "周四", "周五"};
    const QStringList INTERVAL_TEXT = {"", "08:00-11:00", "14:00-17:00", "19:00-22:00"};
    QStringList records;
    bool hasMyOrder = false;

    QStringList lines = content.split("\n");
    int index = 1;

    for (QString line : lines) {
        line = line.trimmed();
        if (line.isEmpty()) continue;

        QRegularExpression re("(\\w+):([^ |]+)");
        QRegularExpressionMatchIterator it = re.globalMatch(line);
        QMap<QString, QString> m;

        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            m[match.captured(1)] = match.captured(2);
        }

        if (!m.contains("studentID") || m["studentID"] != QString::fromStdString(m_student.my_studentID))
            continue;

        hasMyOrder = true;

        int dateNum = m["date"].toInt();
        int intervalNum = m["interval"].toInt();
        int roomNum = m["roomID"].toInt();
        int seatNum = m["seat"].toInt();
        QString statusStr = m.value("status", "0");

        QString statusText = "未知状态";
        QString statusColor = "#F56C6C";
        if (statusStr == "1") { statusText = "审核中"; statusColor = "#FFB800"; }
        else if (statusStr == "2") { statusText = "预约成功"; statusColor = "#67C23A"; }
        else if (statusStr == "-1") { statusText = "已取消"; statusColor = "#909399"; }
        else if (statusStr == "0") { statusText = "审核失败"; statusColor = "#F56C6C"; }

        QString lineHtml = QString(
                               "<div style='margin:18px 10px; padding:16px; background:#1e1e1e; color:#e0e0e0; "
                               "border-left:5px solid #409EFF; border-radius:8px; font-family:Microsoft YaHei; font-size:15px;'>"
                               "<b style='color:#79ffe1;'>预约编号：</b><span style='color:#ffffff;'>%1</span><br>"
                               "<b style='color:#79ffe1;'>日期　　：</b><span style='color:#ffffff;'>%2</span>　　"
                               "<b style='color:#79ffe1;'>时间段　：</b><span style='color:#ffffff;'>%3</span><br>"
                               "<b style='color:#79ffe1;'>机房　　：</b><span style='color:#ffffff;'>%4号</span>　　"
                               "<b style='color:#79ffe1;'>机位　　：</b><span style='color:#ffffff;'>%5号</span><br>"
                               "<b style='color:#79ffe1;'>状态　　：</b><b style='color:%6'>%7</b>"
                               "</div>")
                               .arg(index++)
                               .arg(WEEK_DAYS[dateNum])
                               .arg(INTERVAL_TEXT[intervalNum])
                               .arg(roomNum)
                               .arg(seatNum)
                               .arg(statusColor)
                               .arg(statusText);

        records << lineHtml;
    }

    QString display = hasMyOrder ?
                          "<div style='font-family:Microsoft YaHei; font-size:14pt;'>" + records.join("") + "</div>" :
                          "<p style='font-size:16pt; color:#999; text-align:center; margin:60px;'>您当前没有任何预约记录</p>";

    QMessageBox msg(this);
    msg.setWindowTitle("我的预约记录");
    msg.setTextFormat(Qt::RichText);
    msg.setText(display);
    msg.setIcon(QMessageBox::Information);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.button(QMessageBox::Ok)->setText("知道了");
    msg.resize(680, 560);
    msg.exec();
}
void Student_ability_Window::on_pushButton_CancelOrDer_clicked()
{
    orderFile of;
    if (of.Ordersize == 0)
    {
        QMessageBox::information(this, "取消预约", "暂无任何预约记录");
        return;
    }

    // 收集当前学生所有“可以取消”的预约（审核中 或 已成功）
    struct OrderInfo {
        int index;
        QString desc;
    };
    QVector<OrderInfo> cancelable;

    const QStringList WEEK_DAYS = {"", "周一", "周二", "周三", "周四", "周五"};
    const QStringList INTERVALS = {"", "08:00-11:00", "14:00-17:00", "19:00-22:00"};

    for (int i = 0; i < of.Ordersize; ++i)
    {
        if (QString::fromStdString(of.my_orderData[i]["studentID"]) != QString::fromStdString(m_student.my_studentID))
            continue;

        string status = of.my_orderData[i]["status"];
        if (status != "1" && status != "2")  // 只允许取消“审核中”和“预约成功”的
            continue;

        int date    = of.my_orderData[i]["date"].c_str()[0] - '0';
        int interval= of.my_orderData[i]["interval"].c_str()[0] - '0';
        int room    = std::stoi(of.my_orderData[i]["roomID"]);
        int seat    = std::stoi(of.my_orderData[i]["seat"]);

        QString statusText = (status == "1") ? "审核中" : "预约成功";

        QString desc = QString("编号 %1 | %2 %3 | 机房%4 机位%5号 ")
                           .arg(i+1)
                           .arg(WEEK_DAYS[date])
                           .arg(INTERVALS[interval])
                           .arg(room).arg(seat);


        cancelable.push_back({i, desc});
    }

    if (cancelable.isEmpty())
    {
        QMessageBox::information(this, "取消预约", "您当前没有可以取消的预约\n（已取消或审核失败的记录无法再次取消）");
        return;
    }

    // 构建选择列表
    QStringList items;
    for (const auto& info : cancelable)
        items << info.desc;

    bool ok;
    QString selected = QInputDialog::getItem(
        this,
        "取消预约",
        "<font style='font-size:14px;'>请选择要取消的预约记录：</font>",
        items,
        0,
        false,
        &ok);

    if (!ok || selected.isEmpty()) return;  // 用户点了取消

    // 找到用户选的是哪一条
    int chosenIndex = -1;
    for (int i = 0; i < cancelable.size(); ++i)
    {
        if (items[i] == selected)
        {
            chosenIndex = cancelable[i].index;
            break;
        }
    }

    // 最终确认
    int ret = QMessageBox::question(this,
                                    "确认取消",
                                    "<font style='font-size:15px;'>确定要取消以下预约吗？<br><br>"
                                    "<b>" + selected + "</b><br><br>取消后无法恢复！</font>",
                                    QMessageBox::Yes | QMessageBox::No);

    if (ret != QMessageBox::Yes) return;

    //of.my_orderData[chosenIndex]["status"] = "-1";

    //of.updateOrder();
    of.my_orderData.erase(of.my_orderData.begin() + chosenIndex);
    of.Ordersize = of.my_orderData.size();
    of.updateOrder();

    //order_read中把 status 改为 -1
    QFile historyFile(ORDER_READ);  // ORDER_READ 就是 "order_read.txt"
    if (historyFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QString content = historyFile.readAll();
        content.replace(QRegularExpression("status:1"), "status:-1");
        historyFile.resize(0);
        QTextStream out(&historyFile);
        out << content;
        historyFile.close();
    }
    QMessageBox::information(this, "成功", "预约已成功取消!");
}


void Student_ability_Window::on_pushButton_exit_clicked()
{
    int ret = QMessageBox::question(this, "退出系统",
                                    "确定要退出机房预约系统吗？",
                                    QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes)
    {

        qApp->quit();
    }
}

