#include "loginwindow.h"
#include <QApplication>
#include <DataObjects/dataobject.h>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(!DataObject::openDataBase()) {
        QMessageBox::critical(nullptr, "错误", "数据库打开失败！");
        return 1;
    }
    LoginWindow w;
    w.show();
    DataObject::closeDataBase();
    return a.exec();
}
