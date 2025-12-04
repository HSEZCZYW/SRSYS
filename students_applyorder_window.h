#ifndef STUDENTS_APPLYORDER_WINDOW_H
#define STUDENTS_APPLYORDER_WINDOW_H

#include <QDialog>
#include <QVector>
#include "student.h"
#include"computerRoom.h"   // 基类
#include"publicroom.h"     // 普通机房
#include"labroom.h"        // 实验室

namespace Ui {
class Students_applyOrder_Window;
}

class Students_applyOrder_Window : public QDialog
{
    Q_OBJECT

public:


    explicit Students_applyOrder_Window(const student& stu, QWidget *parent = nullptr);
    ~Students_applyOrder_Window();

private slots:
    void on_pushButton_sure_clicked();
    void on_pushButton_cancel_clicked();
    void on_comboBox_room_currentIndexChanged(int index);  // 关键：切换机房时控制“几人”选项

private:
    void loadComputerRooms();     // 多态加载所有机房
    void updatePersonsControl();  // 根据机房类型显示/隐藏“几人”

    Ui::Students_applyOrder_Window *ui;
    student m_student;

    QVector<ComputerRoom*> m_rooms;  // 所有机房的多态容器（核心！）
};

#endif
