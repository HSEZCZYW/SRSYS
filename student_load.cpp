#include "student_load.h"
#include "ui_student_load.h"
#include <QDebug>
student_load::student_load(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::student_load)
{
    ui->setupUi(this);
}

student_load::~student_load()
{
    delete ui;
}
//获取输入内容
QString student_load::getName() const { return ui->lineEdit_name->text(); }
QString student_load::getId()   const { return ui->lineEdit_number->text(); }
QString student_load::getPwd()  const { return ui->lineEdit_password->text(); }
void student_load::on_pushButton_load_clicked()
{
    QString name = getName();
    QString id   = getId();
    QString pwd  = getPwd();
    qDebug() << "你输入的是："
             << "姓名:" << name
             << "学号:" << id
             << "密码:" << pwd;
    if (name.isEmpty() || id.isEmpty() || pwd.isEmpty())
    {
        QMessageBox::warning(this, "错误", "请填写完整信息！");
        return;
    }

    identity* user = loginInFile(STUDENT_FILE, 1,
                                 name.toStdString(),
                                 pwd.toStdString(),
                                 id.toStdString());
    qDebug() << "loginInFile 返回的指针:" << user;
    if (user ) {

        student currentstu(id.toStdString(), pwd.toStdString(), name.toStdString());

        accept();


        Student_ability_Window* win = new Student_ability_Window(currentstu);
        win->setAttribute(Qt::WA_DeleteOnClose);
        win->show();
    }
    else
    {
        QMessageBox::critical(this, "失败", "信息错误！");
        ui->lineEdit_password->clear();
    }
}


void student_load::on_pushButton_cancel_clicked()
{
    reject();
}

