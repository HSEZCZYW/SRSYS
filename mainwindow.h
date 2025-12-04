#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMessageBox>
#include"student_load.h"
#include<QTimer>
#include <QMainWindow>
#include<QDateTime>
#include"teacher_login.h"
#include"manager_login.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateTime();

private slots:
    void on_btn_manager_clicked();

    void on_btn_student_clicked();

    void on_btn_teacher_clicked();

    void on_btn_exit_clicked();



private:
    Ui::MainWindow *ui;
    QTimer *timer;
};
#endif // MAINWINDOW_H
