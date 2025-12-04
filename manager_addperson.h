#ifndef MANAGER_ADDPERSON_H
#define MANAGER_ADDPERSON_H

#include <QDialog>
#include"globalfile.h"
#include <QMessageBox>
#include <QString>
#include <QFile>
#include <fstream>
#include <string>
namespace Ui {
class manager_addPerson;
}

class manager_addPerson : public QDialog
{
    Q_OBJECT

public:
    explicit manager_addPerson(QWidget *parent = nullptr);
    ~manager_addPerson();


private slots:
    void on_pushButton_sure_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::manager_addPerson *ui;
};

#endif // MANAGER_ADDPERSON_H
