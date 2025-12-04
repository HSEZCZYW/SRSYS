#include "teacher_ability_window.h"
#include "ui_teacher_ability_window.h"
#include<QFile>

teacher_ability_window::teacher_ability_window(const teacher& tea,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::teacher_ability_window)
    ,m_teacher(tea)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &teacher_ability_window::updateTime);
    timer->start(1000);        // 每1000毫秒（1秒）触发一次
    updateTime();              // 立刻显示一次当前时间
}
//time
void teacher_ability_window::updateTime()
{
    QString timeStr = QDateTime::currentDateTime().toString("yyyy年MM月dd日 hh:mm:ss dddd");
    ui->label_time->setText(timeStr);
    ui->label_time->setStyleSheet("color: cyan; font: bold 16px; background-color: rgba(0,0,0,100); padding: 5px;");
}

teacher_ability_window::~teacher_ability_window()
{
    delete ui;
}

void teacher_ability_window::on_pushButton_checkAllOrder_clicked()
{
    // 老师查看所有预约记录，必须读取历史记录文件（包含已取消、审核失败等）
    QFile file(ORDER_READ);  // 就是 order_read.txt
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::information(this, "查看所有预约", "暂无任何预约记录");
        return;
    }

    QString content = file.readAll();
    file.close();

    if (content.trimmed().isEmpty()) {
        QMessageBox::information(this, "查看所有预约", "暂无任何预约记录");
        return;
    }

    const QStringList WEEK_DAYS = {"", "周一", "周二", "周三", "周四", "周五"};
    const QStringList INTERVAL_TEXT = {"", "08:00-11:00", "14:00-17:00", "19:00-22:00"};
    QStringList records;
    int index = 1;

    QStringList lines = content.split("\n", Qt::SkipEmptyParts);

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

        if (!m.contains("date") || !m.contains("studentID")) continue;

        int dateNum = m["date"].toInt();
        int intervalNum = m["interval"].toInt();
        int roomNum = m["roomID"].toInt();
        int seatNum = m["seat"].toInt();
        QString stuId = m["studentID"];
        QString stuName = m.value("studentName", "未知");
        QString statusStr = m.value("status", "0");

        QString statusText, statusColor;
        if (statusStr == "1") { statusText = "审核中"; statusColor = "#FFB800"; }
        else if (statusStr == "2") { statusText = "预约成功"; statusColor = "#67C23A"; }
        else if (statusStr == "-1") { statusText = "已取消"; statusColor = "#909399"; }
        else { statusText = "审核失败"; statusColor = "#F56C6C"; }

        QString lineHtml = QString(
                               "<div style='margin:18px 10px; padding:16px; background:#1e1e1e; color:#e0e0e0; "
                               "border-left:5px solid #409EFF; border-radius:8px; font-family:Microsoft YaHei; font-size:15px;'>"
                               "<b style='color:#79ffe1;'>预约编号：</b><span style='color:#ffffff;'>%1</span><br>"
                               "<b style='color:#79ffe1;'>学号　　：</b><span style='color:#ffffff;'>%2</span>　　"
                               "<b style='color:#79ffe1;'>姓名　　：</b><span style='color:#ffffff;'>%3</span><br>"
                               "<b style='color:#79ffe1;'>日期　　：</b><span style='color:#ffffff;'>%4</span>　　"
                               "<b style='color:#79ffe1;'>时间段　：</b><span style='color:#ffffff;'>%5</span><br>"
                               "<b style='color:#79ffe1;'>机房　　：</b><span style='color:#ffffff;'>%6号</span>　　"
                               "<b style='color:#79ffe1;'>机位　　：</b><span style='color:#ffffff;'>%7号</span><br>"
                               "<b style='color:#79ffe1;'>状态　　：</b><b style='color:%8'>%9</b>"
                               "</div>")
                               .arg(index++)
                               .arg(stuId)
                               .arg(stuName)
                               .arg(WEEK_DAYS[dateNum])
                               .arg(INTERVAL_TEXT[intervalNum])
                               .arg(roomNum)
                               .arg(seatNum)
                               .arg(statusColor)
                               .arg(statusText);

        records << lineHtml;
    }

    QString displayContent = records.isEmpty()
                                 ? "<p style='font-size:16pt; color:#999; text-align:center; margin:60px;'>暂无任何预约记录</p>"
                                 : "<div style='font-family:Microsoft YaHei; font-size:14pt;'>" + records.join("") + "</div>";

    QMessageBox msg(this);
    msg.setWindowTitle("所有预约记录");
    msg.setTextFormat(Qt::RichText);
    msg.setText(displayContent);
    msg.setIcon(QMessageBox::Information);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.button(QMessageBox::Ok)->setText("关闭");
    msg.resize(720, 600);
    msg.exec();
}
void teacher_ability_window::on_pushButton_volidOrder_clicked()
{
    teacher_viewOrder *auditDlg = new teacher_viewOrder(this);
    // 模态显示（挡住主窗口，直到审核窗口关闭）
    auditDlg->exec();
}

void teacher_ability_window::on_pushButton_exit_clicked()
{
    int ret = QMessageBox::question(this, "退出系统",
                                    "确定要退出机房预约系统吗？",
                                    QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes)
    {

        qApp->quit();
    }
}

