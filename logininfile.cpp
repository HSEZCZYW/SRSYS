// LoginInFile.cpp
#include "LoginInFile.h"
#include "identity.h"
#include "student.h"
#include "teacher.h"
#include "manager.h"
#include "globalfile.h"

#include <fstream>
#include <sstream>   // 关键！用 stringstream 彻底无视 \r\n
#include <string>

using namespace std;

identity* loginInFile(const string& filename, int type,
                      const string& name, const string& pwd,
                      const string& id)
{
    ifstream ifs(filename);
    if (!ifs.is_open()) {
        return nullptr;
    }

    string line;
    while (getline(ifs, line)) {               // 每次读一整行，自动去掉 \r\n
        stringstream ss(line);
        string fName, fId, fPwd;

        if (type == 1) {   // 学生
            if (!(ss >> fName >> fId >> fPwd)) continue;
            if (fName == name && fId == id && fPwd == pwd) {
                return new student(id, pwd, name);
            }
        }
        else if (type == 2) {  // 老师
            if (!(ss >> fName >> fId >> fPwd)) continue;
            if (fName == name && fId == id && fPwd == pwd) {
                return new teacher(id, name, pwd);
            }
        }
        else if (type == 3) {  // 管理员
            if (!(ss >> fName >> fPwd)) continue;
            if (fName == name && fPwd == pwd) {
                return new manager(name, pwd);
            }
        }
    }
    return nullptr;   // 没找到
}
