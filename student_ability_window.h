#ifndef STUDENT_ABILITY_WINDOW_H
#define STUDENT_ABILITY_WINDOW_H
#include"student.h"
#include <QMainWindow>
#include <QMessageBox>
#include<QTimer>
#include<QDateTime>
#include"students_applyorder_window.h"
#include<QInputDialog>
namespace Ui {
class Student_ability_Window;
}

class Student_ability_Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Student_ability_Window(const student& stu,QWidget *parent = nullptr);
    ~Student_ability_Window();
    void updateTime();

private slots:
    void on_pushButton_applyOrder_clicked();

    void on_pushButton_checkMyOrder_clicked();

    void on_pushButton_CancelOrDer_clicked();

    void on_pushButton_exit_clicked();

private:
    Ui::Student_ability_Window *ui;
    student m_student;
    QTimer *timer;
};

#endif // STUDENT_ABILITY_WINDOW_H
