#include "manager_searchforperson.h"
#include "ui_manager_searchforperson.h"
#include<QDir>
#include<QDesktopServices>
manager_searchForPerson::manager_searchForPerson(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::manager_searchForPerson)
{
    ui->setupUi(this);
    ui->comboBox_identity->addItem("学生");
    ui->comboBox_identity->addItem("教师");
    ui->comboBox_identity->setCurrentIndex(0);

    ui->comboBox_way->addItem("按ID查找");
    ui->comboBox_way->addItem("按姓名查找");
    ui->comboBox_way->setCurrentIndex(0);
}

manager_searchForPerson::~manager_searchForPerson()
{
    delete ui;
}

void manager_searchForPerson::on_pushButton_sure_clicked()
{
    //做成多态：身份->查找->way->显示身份信息，且将预约order写进新建txt文件（无order则显示无order)
    QString keyword = ui->lineEdit_content->text().trimmed();
    if (keyword.isEmpty())
    {
        QMessageBox::warning(this, "错误", "请输入要查找的内容！");
        return;
    }

    PersonFinder* finder = nullptr;

    QString findWay = ui->comboBox_way->currentText();  // "按ID查找" 或 "按姓名查找"

    if (findWay == "按ID查找")
        finder = new FindByID();
    else
        finder = new FindByName();

    // 多态调用
    QString result = finder->find(keyword);

    delete finder;

    // 显示结果
    QString displayResult;
    if (result.contains("学生："))
    {
        // 学生：
        QRegularExpression re("学生：(.+) \\((.+)\\)");
        QRegularExpressionMatch match = re.match(result);
        if (match.hasMatch())
        {
            QString stuName = match.captured(1);
            QString stuID   = match.captured(2);
            displayResult = QString("学生：%1\n学号：%2").arg(stuName, stuID);

            // 生成预约记录文件
            QString folderPath = "D:/system/SRSYS/research_order_record";
            QDir dir(folderPath);
            if (!dir.exists()) dir.mkpath(folderPath);

            QString filename = folderPath + "/" +
                               QString("%1_%2_预约记录_%3.txt")
                                   .arg(stuID)
                                   .arg(stuName)
                                   .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss"));

            QFile file(filename);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QTextStream out(&file);

                out << "=== 学生预约记录 ===\n";
                out << "姓名：" << stuName << "\n";
                out << "学号：" << stuID << "\n";
                out << "生成时间：" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "\n";
                out << "==================\n\n";

                orderFile of;
                qDebug() << "读取到" << of.Ordersize << "条预约记录";  //调试

                bool hasRecord = false;
                const QStringList WEEK = {"", "周一", "周二", "周三", "周四", "周五"};
                const QStringList TIME = {"", "08:00-11:00", "14:00-17:00", "19:00-22:00"};

                for (int i = 0; i < of.Ordersize; ++i)
                {
                    // 取出 studentID
                    std::string sid = of.my_orderData[i]["studentID"];
                    if (sid.empty()) sid = of.my_orderData[i]["studentId"];  // 兼容小写
                    if (sid.empty()) sid = of.my_orderData[i]["StudentID"];
                    sid.erase(0, sid.find_first_not_of(" \t\r\n"));
                    sid.erase(sid.find_last_not_of(" \t\r\n") + 1);
                    QString fileID = QString::fromStdString(sid);

                    //打印调试
                    qDebug() << "文件里学号:" << fileID << "查找学号:" << stuID << "是否相等:" << (fileID == stuID);

                    if (fileID == stuID)
                    {
                        hasRecord = true;
                        qDebug() << "正在写入第" << i << "条记录";

                        int d = of.my_orderData[i]["date"].c_str()[0] - '0';
                        int t = of.my_orderData[i]["interval"].c_str()[0] - '0';
                        int r = std::stoi(of.my_orderData[i]["roomID"]);
                        int s = std::stoi(of.my_orderData[i]["seat"]);
                        string status = of.my_orderData[i]["status"];

                        QString statusText = (status == "1") ? "审核中" :
                                                 (status == "2") ? "预约成功" :
                                                 (status == "-1") ? "已取消" : "审核失败";

                        out << QString("预约%1：%2 %3 | 机房%4 机位%5 | 状态：%6\n")
                                   .arg(i+1).arg(WEEK[d]).arg(TIME[t]).arg(r).arg(s).arg(statusText);
                    }
                }

                if (!hasRecord)
                    out << "暂无预约记录\n";

                file.close();
                displayResult += QString("\n已生成预约记录文件：\n%1").arg(filename);
                QDesktopServices::openUrl(QUrl::fromLocalFile(folderPath));
            }
            else
            {
                displayResult += "文件生成失败！";
            }
        }
    }
    else if (result.contains("教师："))
    {
        // 教师：
        QRegularExpression re("教师：(.+) \\((.+)\\)");
        QRegularExpressionMatch match = re.match(result);
        if (match.hasMatch())
        {
            QString teaName = match.captured(1);
            QString teaID   = match.captured(2);
            displayResult = QString("教师：%1\n工号：%2").arg(teaName, teaID);
        }
    }
    else
    {
        displayResult = result;
    }
    QMessageBox::information(this, "查找结果", displayResult);
}


void manager_searchForPerson::on_pushButton_cancel_clicked()
{
    reject();
}

