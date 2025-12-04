#ifndef MANAGER_ABLITY_WINDOW_H
#define MANAGER_ABLITY_WINDOW_H

#include <QMainWindow>
#include"manager.h"
#include <QMainWindow>
#include <QMessageBox>
#include<QTimer>
#include<QDateTime>
#include<QInputDialog>
#include"manager_addperson.h"
#include"manager_searchforperson.h"
namespace Ui {
class manager_ablity_window;
}

class manager_ablity_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit manager_ablity_window(const manager& Admin,QWidget *parent = nullptr);
    ~manager_ablity_window();
    void updateTime();

private slots:
    void on_pushButton_addPerson_clicked();

    void on_pushButton_SearchforPerson_clicked();

    void on_pushButton_SearchforComputerRoom_clicked();

    void on_pushButton_OrderClear_clicked();

    void on_pushButton_exit_clicked();

private:
    Ui::manager_ablity_window *ui;
    QTimer *timer;
    manager m_admin;
};

#endif // MANAGER_ABLITY_WINDOW_H
