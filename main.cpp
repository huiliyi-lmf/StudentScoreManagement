#include "loginwindow.h"
#include<iostream>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWindow w;
    std::cout<<"nnnnnnnnnn"<<std::endl;
    w.show();
    return a.exec();
}
