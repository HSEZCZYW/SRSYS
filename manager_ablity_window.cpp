#include "manager_ablity_window.h"
#include "ui_manager_ablity_window.h"
#include<QRegularExpression>
#include"publicroom.h"
#include"labroom.h"
manager_ablity_window::manager_ablity_window(const manager& Admin,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::manager_ablity_window)
    ,m_admin(Admin)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &manager_ablity_window::updateTime);
    timer->start(1000);        // 每1000毫秒（1秒）触发一次
    updateTime();
}

void manager_ablity_window::updateTime()
{
    QString timeStr = QDateTime::currentDateTime().toString("yyyy年MM月dd日 hh:mm:ss dddd");
    ui->label_time->setText(timeStr);
    ui->label_time->setStyleSheet("color: cyan; font: bold 16px; background-color: rgba(0,0,0,100); padding: 5px;");
}

manager_ablity_window::~manager_ablity_window()
{
    delete ui;
}

void manager_ablity_window::on_pushButton_addPerson_clicked()
{
    manager_addPerson *auditDlg = new manager_addPerson(this);
    auditDlg->exec();
}


void manager_ablity_window::on_pushButton_SearchforPerson_clicked()
{
    manager_searchForPerson *auditDlg = new manager_searchForPerson(this);
    auditDlg->exec();
}



void manager_ablity_window::on_pushButton_OrderClear_clicked()
{
    int ret = QMessageBox::question(this, "清空所有预约",
                                    "确定要清空所有预约记录吗？"
                                    "此操作将删除所有，无法恢复",
                                    QMessageBox::Yes | QMessageBox::No);

    if (ret != QMessageBox::Yes)
        return;

    std::ofstream ofs(ORDER_FILE, std::ios::trunc);
    if (ofs.is_open())
    {
        ofs.close();
        QMessageBox::information(this, "成功", "所有预约记录已清空！");
    }
    else
    {
        QMessageBox::critical(this, "失败", "文件打开失败，路径可能有误！");
    }
}


void manager_ablity_window::on_pushButton_exit_clicked()
{
    int ret = QMessageBox::question(this, "退出系统",
                                    "确定要退出机房预约系统吗？",
                                    QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes)
    {

        qApp->quit();
    }
}

