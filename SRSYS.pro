QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    findbyid.cpp \
    findbyname.cpp \
    labroom.cpp \
    logininfile.cpp \
    main.cpp \
    mainwindow.cpp \
    manager.cpp \
    manager_ablity_window.cpp \
    manager_addperson.cpp \
    manager_login.cpp \
    manager_searchforperson.cpp \
    orderFile.cpp \
    publicroom.cpp \
    student.cpp \
    student_ability_window.cpp \
    student_load.cpp \
    students_applyorder_window.cpp \
    teacher.cpp \
    teacher_ability_window.cpp \
    teacher_login.cpp \
    teacher_vieworder.cpp

HEADERS += \
    computerRoom.h \
    findbyid.h \
    findbyname.h \
    globalfile.h \
    identity.h \
    labroom.h \
    logininfile.h \
    mainwindow.h \
    manager.h \
    manager_ablity_window.h \
    manager_addperson.h \
    manager_login.h \
    manager_searchforperson.h \
    orderFile.h \
    personfinder.h \
    publicroom.h \
    student.h \
    student_ability_window.h \
    student_load.h \
    students_applyorder_window.h \
    teacher.h \
    teacher_ability_window.h \
    teacher_login.h \
    teacher_vieworder.h

FORMS += \
    mainwindow.ui \
    manager_ablity_window.ui \
    manager_addperson.ui \
    manager_login.ui \
    manager_searchforperson.ui \
    student_ability_window.ui \
    student_load.ui \
    students_applyorder_window.ui \
    teacher_ability_window.ui \
    teacher_login.ui \
    teacher_vieworder.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
