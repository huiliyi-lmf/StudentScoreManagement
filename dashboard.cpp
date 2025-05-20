#include "dashboard.h"
#include "ui_dashboard.h"

DashBoard::DashBoard(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DashBoard), parentPointer(parent)
{
    ui->setupUi(this);
    QObject::connect(this->ui->majorSettingAction, &QAction::triggered, this, &DashBoard::openMajorSetting);
    QObject::connect(this->ui->classSettingAction, &QAction::triggered, this, &DashBoard::openClassSetting);
    QObject::connect(this->ui->subjectSettingAction, &QAction::triggered, this, &DashBoard::openSubjectSetting);
}

DashBoard::~DashBoard()
{
    delete ui;
    parentPointer->show();
}

void DashBoard::closeEvent(QCloseEvent *event) {
    if(QMessageBox::question(this, "提示", "确定要退出吗？") == QMessageBox::Yes) {
        event->accept();
        this->deleteLater();
    } else {
        event->ignore();
    }
}

void DashBoard::openClassSetting() {
    this->setEnabled(false);
    ClassSettingWindow* window = new ClassSettingWindow(this);
    window->show();
}
void DashBoard::openMajorSetting() {
    this->setEnabled(false);
    MajorSettingWindow* window = new MajorSettingWindow(this);
    window->show();
}
void DashBoard::openSubjectSetting() {
    this->setEnabled(false);
    SubjectSettingWindow* window = new SubjectSettingWindow(this);
    window->show();
}
