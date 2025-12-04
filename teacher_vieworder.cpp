#include "teacher_vieworder.h"
#include "ui_teacher_vieworder.h"

teacher_viewOrder::teacher_viewOrder(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::teacher_viewOrder)
{
    ui->setupUi(this);
    of = orderFile();          // 读取一次文件
    refreshList();
}

teacher_viewOrder::~teacher_viewOrder()
{
    delete ui;
}

void teacher_viewOrder::on_pushButton_ture_clicked()
{
    int row = ui->listWidget_order->currentRow();
    if (row < 0 || row >= pendingIndices.size())
    {
        QMessageBox::warning(this, "提示", "请先选择一条待审核的预约记录！");
        return;
    }

    // 二次确认
    if (QMessageBox::question(this, "审核通过",
                              "确定要通过该预约吗？",
                              QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        return;

    int realIndex = pendingIndices[row];
    of.my_orderData[realIndex]["status"] = "2";
    of.updateOrder();

    QMessageBox::information(this, "成功", "该预约审核通过！");

    // 刷新列表
    refreshList();
}


void teacher_viewOrder::on_pushButton_false_clicked()
{
    int row = ui->listWidget_order->currentRow();
    if (row < 0 || row >= pendingIndices.size())
    {
        QMessageBox::warning(this, "提示", "请先选择一条待审核的预约记录！");
        return;
    }

    // 二次确认
    if (QMessageBox::question(this, "审核不通过",
                              "确定不通过该预约吗？",
                              QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        return;

    int realIndex = pendingIndices[row];
    of.my_orderData[realIndex]["status"] = "0";
    of.updateOrder();

    QMessageBox::information(this, "成功", "该预约审核不通过！");

    // 刷新列表
    refreshList();
}


void teacher_viewOrder::on_pushButton_cancle_clicked()
{
    this->close();
}


void teacher_viewOrder::on_listWidget_order_currentRowChanged(int currentRow)
{
    bool valid = (currentRow >= 0 && currentRow < pendingIndices.size());
    ui->pushButton_ture->setEnabled(valid);
    ui->pushButton_false->setEnabled(valid);
}
void teacher_viewOrder::refreshList()
{
    ui->listWidget_order->clear();
    pendingIndices.clear();

    const QStringList WEEK = {"", "周一", "周二", "周三", "周四", "周五"};
    const QStringList TIME = {"", "08:00-11:00", "14:00-17:00", "19:00-22:00"};

    for (int i = 0; i < of.Ordersize; ++i)
    {
        if (of.my_orderData[i]["status"] != "1") continue;  // 只显示审核中

        int d = of.my_orderData[i]["date"].c_str()[0] - '0';
        int t = of.my_orderData[i]["interval"].c_str()[0] - '0';
        int r = std::stoi(of.my_orderData[i]["roomID"]);
        int s = std::stoi(of.my_orderData[i]["seat"]);
        QString name = QString::fromStdString(of.my_orderData[i]["studentName"]);
        QString id   = QString::fromStdString(of.my_orderData[i]["studentID"]);

        QString text = QString("编号%1 | %2 %3 | 机房%4 机位%5 | %6 (%7)")
                           .arg(i+1).arg(WEEK[d]).arg(TIME[t]).arg(r).arg(s).arg(name).arg(id);

        ui->listWidget_order->addItem(text);
        pendingIndices.append(i);
    }

    if (pendingIndices.isEmpty())
    {
        QMessageBox::information(this, "完成", "所有预约已审核完毕！");
        this->close();
    }

    ui->pushButton_ture->setEnabled(false);
    ui->pushButton_false->setEnabled(false);
}
