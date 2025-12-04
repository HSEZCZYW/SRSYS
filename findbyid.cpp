#include "findbyid.h"


QString FindByID::find(const QString& keyword)
{
    std::ifstream ifs(STUDENT_FILE);
    std::string line;
    while (std::getline(ifs, line))
    {
        std::istringstream iss(line);
        std::string name, id, pwd;
        iss >> name >> id >> pwd;
        if (QString::fromStdString(id) == keyword)
            return QString("学生：%1 (%2) 密码：%3").arg(QString::fromStdString(name), keyword, QString::fromStdString(pwd));
    }
    ifs.close();

    ifs.open(TEACHER_FILE);
    while (std::getline(ifs, line))
    {
        std::istringstream iss(line);
        std::string name, id, pwd;
        iss >> name >> id >> pwd;
        if (QString::fromStdString(id) == keyword)
            return QString("教师：%1 (%2) 密码：%3").arg(QString::fromStdString(name), keyword, QString::fromStdString(pwd));
    }
    return "未找到！";
}
