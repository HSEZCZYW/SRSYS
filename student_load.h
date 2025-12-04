#ifndef STUDENT_LOAD_H
#define STUDENT_LOAD_H

#include <QDialog>
#include<QMessageBox>
#include"globalfile.h"
#include"student.h"
#include"logininfile.h"
#include"student_ability_window.h"
namespace Ui {
class student_load;
}

class student_load : public QDialog
{
    Q_OBJECT

public:
    explicit student_load(QWidget *parent = nullptr);
    ~student_load();
    //从登录页面读取的内容
    QString getName() const;
    QString getId() const;
    QString getPwd() const;

private slots:
    void on_pushButton_load_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::student_load *ui;
};

#endif // STUDENT_LOAD_H
