QT       += core gui
QT       += sql
QT       += charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
INCLUDEPATH += $$PWD/DataObjects
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Analysis/MajorAnalysis.cpp \
    Analysis/ScoreAnalysis.cpp \
    BaseSettings/AdminWindow.cpp \
    BaseSettings/ClassSettingWindow.cpp \
    BaseSettings/MajorSettingWindow.cpp \
    BaseSettings/SubjectSettingWindow.cpp \
    DataManage/ScoreManage.cpp \
    DataManage/StudentManage.cpp \
    DataObjects/admin.cpp \
    DataObjects/class.cpp \
    DataObjects/dataobject.cpp \
    DataObjects/major.cpp \
    DataObjects/score.cpp \
    DataObjects/student.cpp \
    DataObjects/subject.cpp \
    DataSearch/ScoreSearch.cpp \
    DataSearch/StudentSearch.cpp \
    dashboard.cpp \
    main.cpp \
    loginwindow.cpp \
    registerwindow.cpp

HEADERS += \
    Analysis/MajorAnalysis.h \
    Analysis/ScoreAnalysis.h \
    BaseSettings/AdminWindow.h \
    BaseSettings/ClassSettingWindow.h \
    BaseSettings/MajorSettingWindow.h \
    BaseSettings/SubjectSettingWindow.h \
    DataManage/ScoreManage.h \
    DataManage/StudentManage.h \
    DataObjects/admin.h \
    DataObjects/class.h \
    DataObjects/dataobject.h \
    DataObjects/major.h \
    DataObjects/score.h \
    DataObjects/student.h \
    DataObjects/subject.h \
    DataSearch/ScoreSearch.h \
    DataSearch/StudentSearch.h \
    MyMacro.h \
    dashboard.h \
    loginwindow.h \
    registerwindow.h

FORMS += \
    Analysis/MajorAnalysis.ui \
    Analysis/ScoreAnalysis.ui \
    BaseSettings/AdminWindow.ui \
    BaseSettings/ClassSettingWindow.ui \
    BaseSettings/MajorSettingWindow.ui \
    BaseSettings/SubjectSettingWindow.ui \
    DataManage/ScoreManage.ui \
    DataManage/StudentManage.ui \
    DataSearch/ScoreSearch.ui \
    DataSearch/StudentSearch.ui \
    dashboard.ui \
    loginwindow.ui \
    registerwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rc.qrc
