#ifndef TEACHER_ABILITY_WINDOW_H
#define TEACHER_ABILITY_WINDOW_H

#include <QMainWindow>
#include"teacher.h"
#include<QTimer>
#include<QDateTime>
#include<QMessageBox>
#include"globalfile.h"
#include"orderFile.h"
#include<QInputDialog>
#include<QComboBox>
#include"teacher_vieworder.h"
namespace Ui {
class teacher_ability_window;
}

class teacher_ability_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit teacher_ability_window(const teacher& tea,QWidget *parent = nullptr);
    ~teacher_ability_window();
    void updateTime();

private slots:
    void on_pushButton_checkAllOrder_clicked();

    void on_pushButton_volidOrder_clicked();

    void on_pushButton_exit_clicked();

private:
    Ui::teacher_ability_window *ui;
    teacher m_teacher;
    QTimer *timer;
};

#endif // TEACHER_ABILITY_WINDOW_H
