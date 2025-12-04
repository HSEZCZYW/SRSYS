#include "teacher_login.h"
#include "ui_teacher_login.h"

teacher_login::teacher_login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::teacher_login)
{
    ui->setupUi(this);
}

teacher_login::~teacher_login()
{
    delete ui;
}
// 获取输入内容（给主窗口用）
QString teacher_login::getName() const { return ui->lineEdit_name->text(); }
QString teacher_login::getId()   const { return ui->lineEdit_number->text(); }
QString teacher_login::getPwd()  const { return ui->lineEdit_password->text(); }

void teacher_login::on_pushButton_login_clicked()
{
    QString name = getName();
    QString id   = getId();
    QString pwd  = getPwd();

    if (name.isEmpty() || id.isEmpty() || pwd.isEmpty())
    {
        QMessageBox::warning(this, "错误", "请填写完整信息！");
        return;
    }

    identity* user = loginInFile(TEACHER_FILE, 2,
                                 name.toStdString(),
                                 id.toStdString(),
                                 pwd.toStdString());

    if (user ) {
        // 构造当前教师对象
        teacher currentTea(id.toStdString(), pwd.toStdString(), name.toStdString());

        accept();   // 关闭登录弹窗

        // 打开学生主界面
        teacher_ability_window* win = new teacher_ability_window(currentTea);
        win->setAttribute(Qt::WA_DeleteOnClose);  // 关闭时自动删除
        win->show();
    }
    else
    {
        QMessageBox::critical(this, "失败", "信息错误！");
        ui->lineEdit_password->clear();
    }
}


void teacher_login::on_pushButton_cancel_clicked()
{
    reject();
}

