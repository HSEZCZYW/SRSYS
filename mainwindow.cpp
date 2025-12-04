#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
    timer->start(1000);        // 每1000毫秒（1秒）触发一次
    updateTime();              // 立刻显示一次当前时间
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::updateTime()
{
    QString timeStr = QDateTime::currentDateTime().toString("yyyy年MM月dd日 hh:mm:ss dddd");
    ui->label_time->setText(timeStr);
    ui->label_time->setStyleSheet("color: cyan; font: bold 16px; background-color: rgba(0,0,0,100); padding: 5px;");
}

void MainWindow::on_btn_manager_clicked()
{
    manager_login dlg(this);
    dlg.setWindowTitle("教师登录");
    dlg.exec();
}


void MainWindow::on_btn_student_clicked()
{
    student_load dlg(this);
    dlg.setWindowTitle("学生登录");
    dlg.exec();

}


void MainWindow::on_btn_teacher_clicked()
{
    teacher_login dlg(this);
    dlg.setWindowTitle("教师登录");
    dlg.exec();
}


void MainWindow::on_btn_exit_clicked()
{

    int ret = QMessageBox::question(this, "退出系统",
                                    "确定要退出机房预约系统吗？",
                                    QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes)
    {

        qApp->quit();
    }
}

