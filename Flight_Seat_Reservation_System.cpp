#include<iostream>
#include"identity.h"
#include"student.h"
#include"manager.h"
#include"teacher.h"
#include"globalfile.h"
#include<fstream>
#include<string>
#include "mainwindow.h"

#include <QApplication>
using namespace std;
//教师菜单
void teacherMenu(identity*& Teacher)
{
    while (true)
    {
        //调用教师子菜单
        Teacher->openmenu();

        //将父类指针转化为子类指针，调用子类接口
        teacher* man = dynamic_cast<teacher*>(Teacher);

        int select = 0;
        cin >> select;

        if (select == 1)//查看所有预约
        {
            cout << "查看所有预约" << endl;
            man->showAllorder();
            
        }
        else if (select == 2)//审核预约
        {
            cout << "审核预约" << endl;
            man->volidOrder();
        }
        else
        {
            //注销
            delete Teacher;
            cout << "注销成功" << endl;
            system("pause");
            system("cls");
        }
    }
}
//学生菜单
void studentMenu(identity*& Student)
{
    while (true)
    {
        //调用学生子菜单
        Student->openmenu();

        //将父类指针转化为子类指针，调用子类接口
        student* man = (student*)Student;

        int select = 0;
        cin >> select;

        if (select == 1)//申请预约
        {
            cout << "申请预约" << endl;
            man->applyOrder();
        }
        else if (select == 2)//查看我的预约
        {
            cout << "查看我的预约" << endl;
            man->showMyOrder();
        }
        else if (select == 3)//取消预约
        {
            cout << "取消预约" << endl;
            man->cancleOrder();
        }
        else
        {
            //注销
            delete Student;
            cout << "注销成功" << endl;
            system("pause");
            system("cls");
        }
    }
}
//管理员菜单
void managerMenu(identity*& Manager)
{
    while (true)
    {
        //调用管理员子菜单
        Manager->openmenu();

        //将父类指针转化为子类指针，调用子类接口
        manager* man = (manager*)Manager;

        int select = 0;
        cin >> select;
        
        if (select == 1)//添加账号
        {
            cout << "添加账号" << endl;
            man->addPerson();
        }
        else if (select == 2)//查看账号
        {
            cout << "查看账号" << endl;
            man->showPerson();
        }
        else if (select == 3)//查看机房
        {
            cout << "查看机房" << endl;
            man->showComputer();
        }
        else if (select == 4)//清空预约
        {
            cout << "清空预约" << endl;
            man->claenFile();
        }
        else
        {
            //注销
            delete Manager;
            cout << "注销成功" << endl;
            system("pause");
            system("cls");
        }
    }
}

//登录
void login(string filename, int type)//操作的文件名称和登录的用户类别
{
    //父类指针，指向子类对象
    identity* person = NULL;

    //读文件
    ifstream ifs;
    ifs.open(filename,ios::in);

    //判断文件是否存在
    if (!ifs.is_open())
    {
        cout << "文件不存在" << endl;
        ifs.close();
        return;
    }

    //准备接受用户信息
    string id; string name; string pswd;

    
    cout << "请输入用户名" << endl;
    cin >> name;

    //判断身份
    if (type == 1)//学生
    {
        cout << "请输入你的学号：" << endl;
        cin >> id;
    }
    else if (type == 2)//老师
    {
        cout << "请输入你的工号：" << endl;
        cin >> id;
    }
    
    cout << "请输入密码" << endl;
    cin >> pswd;
    //身份验证
    if (type == 1)
    {
        //学生身份认证
        //从文件中读取学生信息
        string fID;//文件中的ID
        string fname;//文件中的姓名
        string fpswd;//文件中的密码
        //开始验证
        while (ifs >> fname && ifs >> fID && ifs >> fpswd)
        {
            if ((fID == id) && (fname == name )&& (fpswd == pswd))
            {
                cout << "验证成功！" << endl;
                system("pause");
                system("cls");
                person = new student(id,pswd,name);
                //登录成功进入学生菜单
                studentMenu(person);
                return;
            }
        }
    }
    else if (type == 2)
    {
        //教师身份认证
        //从文件中读取教师信息
        string fID;//文件中的ID
        string fname;//文件中的姓名
        string fpswd;//文件中的密码
        //开始验证
        while (ifs >> fname && ifs >> fID && ifs >> fpswd)
        {
            if ((fID == id) && (fname == name) && (fpswd == pswd))
            {
                cout << "验证成功！" << endl;
                system("pause");
                system("cls");
                person = new teacher(id, name,pswd);
                //登录成功进入教师页面
                teacherMenu(person);
                return;
            }
        }
    }
    else if (type == 3)
    {
        //管理员身份认证
        //从文件中读取管理员信息
        string fname;//文件中的姓名
        string fpswd;//文件中的密码
        //开始验证
        while (ifs >> fname && ifs >> fpswd)
        {
            if ((fname == name) && (fpswd == pswd))
            {
                cout << "验证成功！" << endl;
                system("pause");
                system("cls");
                person = new manager(name,pswd);
                //登录成功进入管理员菜单
                managerMenu(person);
                return;
            }
        }
    }
    
    cout << "登录失败，请重试！" << endl;
    system("pause");
    system("cls");
}



/*int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

    /*int select = 0;

    cin >> select; //接受用户选择
    switch (select) //根据用户选择 实现不同接口
    {
    case 1: //学生身份
        login(STUDENT_FILE, 1);
        break;
    case 2: //老师身份
        login(TEACHER_FILE, 2);
        break;
    case 3:  //管理员身份
        login(ADMIN_FILE, 3);
        break;
    case 0:  //退出系统
        cout << "欢迎下一次使用" << endl;
        system("pause");
        return 0;
        break;
    default:
        cout << "输入有误，请重新选择！" << endl;
        system("pause");
        system("cls");
        break;
    }
}/*
