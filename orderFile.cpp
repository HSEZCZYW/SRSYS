#include "orderFile.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

using namespace std;
void orderFile::parseField(const string& field, map<string, string>& m)
{
	size_t pos = field.find(":");
	if (pos != string::npos)
	{
		string key = field.substr(0, pos);
		string value = field.substr(pos + 1);
		m.insert(make_pair(key, value));
	}
}

//构造函数
orderFile::orderFile()
{
    ifstream ifs(ORDER_FILE, ios::in);
    if (!ifs.is_open()) {
        cout << "订单文件打开失败！" << endl;
        this->Ordersize = 0;
        return;
    }

    string line;
    this->Ordersize = 0;

    while (getline(ifs, line)) {
        if (line.empty()) continue;

        map<string, string> m;

        stringstream ss(line);
        string field;
        while (ss >> field) {
            if (field.find("|persons:") != string::npos) {
                // 提取机位人数状态
                size_t pos = field.find(":");
                if (pos != string::npos) {
                    string value = field.substr(pos + 1);
                    m["persons"] = value;  // 保存人数
                }
                continue;
            }
            parseField(field, m);
        }


        if (m.count("date") && m.count("interval") && m.count("studentID") &&
            m.count("studentName") && m.count("roomID") && m.count("seat") && m.count("status")) {

            this->my_orderData.push_back(m);
            this->Ordersize = this->my_orderData.size();
        }
        //兼容过去数据
        if (m.find("persons") == m.end()) {
            m["persons"] = "1";
        }
    }

    ifs.close();
    cout << "成功加载 " << Ordersize << " 条预约记录" << endl;
}


void orderFile::updateOrder()
{
    if (this->Ordersize == 0) {
        ofstream ofs(ORDER_FILE, ios::out | ios::trunc);
        ofs.close();
        return;
    }

    ofstream ofs(ORDER_FILE, ios::out | ios::trunc);
    if (!ofs.is_open()) {
        cout << "订单文件写入失败！" << endl;
        return;
    }

    for (int i = 0; i < this->Ordersize; i++)
    {
        auto& m = this->my_orderData[i];

        ofs << "date:" << m["date"] << " "
            << "interval:" << m["interval"] << " "
            << "studentID:" << m["studentID"] << " "
            << "studentName:" << m["studentName"] << " "
            << "roomID:" << m["roomID"] << " "
            << "seat:" << m["seat"] << " "
            << "status:" << m["status"];
        if (m.count("persons") && m["persons"] == "2")
        {
            ofs << " |persons:2";
        }

        ofs << endl;
    }

    ofs.close();
    cout << "订单文件已更新，共 " << Ordersize << " 条记录" << endl;
}
