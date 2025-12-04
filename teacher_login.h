#ifndef TEACHER_LOGIN_H
#define TEACHER_LOGIN_H

#include <QDialog>
#include<QMessageBox>
#include"globalfile.h"
#include"teacher.h"
#include"logininfile.h"
#include"teacher_ability_window.h"
namespace Ui {
class teacher_login;
}

class teacher_login : public QDialog
{
    Q_OBJECT

public:
    explicit teacher_login(QWidget *parent = nullptr);
    ~teacher_login();
    //从登录页面读取的内容
    QString getName() const;
    QString getId() const;
    QString getPwd() const;

private slots:
    void on_pushButton_login_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::teacher_login *ui;
};

#endif // TEACHER_LOGIN_H
