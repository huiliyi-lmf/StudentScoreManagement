QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DataObjects/admin.cpp \
    DataObjects/class.cpp \
    DataObjects/dataobject.cpp \
    DataObjects/major.cpp \
    DataObjects/score.cpp \
    DataObjects/student.cpp \
    DataObjects/subject.cpp \
    dashboard.cpp \
    main.cpp \
    loginwindow.cpp \
    registerwindow.cpp

HEADERS += \
    DataObjects/admin.h \
    DataObjects/class.h \
    DataObjects/dataobject.h \
    DataObjects/major.h \
    DataObjects/score.h \
    DataObjects/student.h \
    DataObjects/subject.h \
    dashboard.h \
    loginwindow.h \
    registerwindow.h

FORMS += \
    dashboard.ui \
    loginwindow.ui \
    registerwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    tempsql.sql

RESOURCES += \
    rc.qrc
