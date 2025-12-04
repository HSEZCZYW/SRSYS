#include "manager_addperson.h"
#include "ui_manager_addperson.h"
#include"globalfile.h"
manager_addPerson::manager_addPerson(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::manager_addPerson)
{
    ui->setupUi(this);
    ui->comboBox_identity->addItem("学生");
    ui->comboBox_identity->addItem("教师");
    ui->comboBox_identity->setCurrentIndex(0);
}

manager_addPerson::~manager_addPerson()
{
    delete ui;
}
void manager_addPerson::on_pushButton_sure_clicked()
{
    QString name = ui->lineEdit_name->text().trimmed();
    QString id   = ui->lineEdit_number->text().trimmed();
    QString pwd  = ui->lineEdit_password->text();

    if (name.isEmpty() || id.isEmpty() || pwd.isEmpty())
    {
        QMessageBox::warning(this, "错误", "请填写完整信息！");
        return;
    }

    QString identity = ui->comboBox_identity->currentText();
    bool isStudent = (identity == "学生");

    const char* targetFile = isStudent ? STUDENT_FILE : TEACHER_FILE;
    std::ifstream ifs(targetFile);
    if (!ifs.is_open()){   }
    else
    {
        std::string line;
        while (std::getline(ifs, line))
        {
            std::istringstream iss(line);
            std::string existingName, existingId, existingPwd;

            // 按空格读取三个字段
            if (iss >> existingName >> existingId >> existingPwd)
            {
                if (existingId == id.toStdString())
                {
                    ifs.close();
                    QMessageBox::warning(this, "失败",
                                         QString("该%1已被占用！\n%2：%3")
                                             .arg(isStudent ? "学号" : "工号")
                                             .arg(isStudent ? "学号" : "工号", id));
                    return;
                }
            }
        }
        ifs.close();
    }

    std::ofstream ofs(targetFile, std::ios::app);
    if (!ofs.is_open())
    {
        QMessageBox::critical(this, "失败", "文件打开失败，无法添加账号！");
        return;
    }

    ofs << name.toStdString() << " "
        << id.toStdString()   << " "
        << pwd.toStdString()  << std::endl;

    ofs.close();

    QMessageBox::information(this, "成功",
                             tr("已成功添加%1账号！\n姓名：%2\n%3：%4")
                                 .arg(identity, name,
                                      isStudent ? "学号" : "工号", id));

    //清空输入框
    ui->lineEdit_name->clear();
    ui->lineEdit_number->clear();
    ui->lineEdit_password->clear();
    ui->comboBox_identity->setCurrentIndex(0);
}
void manager_addPerson::on_pushButton_cancel_clicked()
{
    reject();
}

