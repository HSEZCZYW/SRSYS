
#ifndef LOGININFILE_H
#define LOGININFILE_H

#include "identity.h"
#include "student.h"
#include "teacher.h"
#include"manager.h"
#include "globalfile.h"
#include <fstream>
#include <string>

using namespace std;

identity* loginInFile(const std::string& filename,
                      int type,
                      const std::string& name,
                      const std::string& pwd,
                      const std::string& id = "");
#endif // LOGININFILE_H
