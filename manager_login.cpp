#include "manager_login.h"
#include "ui_manager_login.h"
#include"manager_ablity_window.h"
manager_login::manager_login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::manager_login)
{
    ui->setupUi(this);
}

manager_login::~manager_login()
{
    delete ui;
}

// 获取输入内容（给主窗口用）
QString manager_login::getName() const { return ui->lineEdit_name->text(); }
QString manager_login::getPwd()  const { return ui->lineEdit_password->text(); }

void manager_login::on_pushButton_login_clicked()
{
    QString name = getName();
    QString pwd  = getPwd();

    if (name.isEmpty() || pwd.isEmpty())
    {
        QMessageBox::warning(this, "错误", "请填写完整信息！");
        return;
    }

    identity* user = loginInFile(ADMIN_FILE, 3,
                                 name.toStdString(),
                                 pwd.toStdString());

    if (user ) {
        // 构造当前管理员对象
        manager currentAdmin( pwd.toStdString(), name.toStdString());

        accept();   // 关闭登录弹窗

        // 打开管理员主界面
        manager_ablity_window* win = new manager_ablity_window(currentAdmin);
        win->setAttribute(Qt::WA_DeleteOnClose);  // 关闭时自动删除
        win->show();
    }
    else
    {
        QMessageBox::critical(this, "失败", "信息错误！");
        ui->lineEdit_password->clear();
    }
}


void manager_login::on_pushButton_cancel_clicked()
{
    reject();
}

