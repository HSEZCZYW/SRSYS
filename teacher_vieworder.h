#ifndef TEACHER_VIEWORDER_H
#define TEACHER_VIEWORDER_H

#include <QDialog>
#include "teacher_viewOrder.h"


#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>

#include "orderFile.h"
#include "globalfile.h"
#include <string>
#include <vector>
namespace Ui {class teacher_viewOrder;}

class teacher_viewOrder : public QDialog
{
    Q_OBJECT

public:
    explicit teacher_viewOrder(QWidget *parent = nullptr);
    ~teacher_viewOrder();

private slots:
    void on_pushButton_ture_clicked();

    void on_pushButton_false_clicked();

    void on_pushButton_cancle_clicked();

    void on_listWidget_order_currentRowChanged(int currentRow);

    void refreshList();

private:
    Ui::teacher_viewOrder *ui;
    QVector<int> pendingIndices;   //存待审核记录的真实下标
    orderFile of;                  //读取一次文件，避免反复读
};

#endif // TEACHER_VIEWORDER_H
