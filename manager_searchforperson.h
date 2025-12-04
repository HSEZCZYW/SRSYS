#ifndef MANAGER_SEARCHFORPERSON_H
#define MANAGER_SEARCHFORPERSON_H

#include <QDialog>
#include<QMessageBox>
#include"findbyname.h"
#include"findbyid.h"
#include"personfinder.h"
#include<string.h>
#include<QDateTime>
#include"orderFile.h"
#include<QFile>
namespace Ui {
class manager_searchForPerson;
}

class manager_searchForPerson : public QDialog
{
    Q_OBJECT

public:
    explicit manager_searchForPerson(QWidget *parent = nullptr);
    ~manager_searchForPerson();


private slots:
    void on_pushButton_sure_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::manager_searchForPerson *ui;
};

#endif // MANAGER_SEARCHFORPERSON_H
