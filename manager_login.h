#ifndef MANAGER_LOGIN_H
#define MANAGER_LOGIN_H

#include <QDialog>
#include "LoginInFile.h"

#include "identity.h"
#include "student.h"
#include "teacher.h"
#include "manager.h"
#include "globalfile.h"
#include"student_ability_window.h"
#include <fstream>
#include <string>
#include <QDialog>
#include <QMessageBox>
namespace Ui {class manager_login;}

class manager_login : public QDialog
{
    Q_OBJECT

public:
    QString getName() const;
    QString getPwd() const;

    identity* m_currentUser = nullptr;
    explicit manager_login(QWidget *parent = nullptr);
    ~manager_login();

private slots:
    void on_pushButton_login_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::manager_login *ui;
};

#endif // MANAGER_LOGIN_H
