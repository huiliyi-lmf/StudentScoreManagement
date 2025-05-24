#include "dashboard.h"
#include "ui_dashboard.h"

DashBoard::DashBoard(QWidget *parent, std::string userName)
    : QMainWindow(parent)
    , ui(new Ui::DashBoard), parentPointer(parent)
{
    ui->setupUi(this);
    QACTION_CONNECT_MYCREATE(this->ui->majorSettingAction, &DashBoard::openMajorSetting);
    QACTION_CONNECT_MYCREATE(this->ui->classSettingAction, &DashBoard::openClassSetting);
    QACTION_CONNECT_MYCREATE(this->ui->subjectSettingAction, &DashBoard::openSubjectSetting);
    QACTION_CONNECT_MYCREATE(this->ui->adminSettingAction, &DashBoard::openAdminWindow);
    QACTION_CONNECT_MYCREATE(this->ui->majorAnalysisAction, &DashBoard::openMajorAnalysis);
    QACTION_CONNECT_MYCREATE(this->ui->scoreAnalysisAction, &DashBoard::openScoreAnalysis);
    QACTION_CONNECT_MYCREATE(this->ui->scoreDataAction, &DashBoard::openScoreManage);
    QACTION_CONNECT_MYCREATE(this->ui->studentDataAction, &DashBoard::openStudentManage);
    QACTION_CONNECT_MYCREATE(this->ui->scoreSearchAction, &DashBoard::openScoreSearch);
    QACTION_CONNECT_MYCREATE(this->ui->studentSearchAction, &DashBoard::openStudentSearch);

    QLabel* userInfo = new QLabel;
    tmpWidgetPointer.push_back(userInfo);
    userInfo->setText(QString("当前登陆用户：%1").arg(STDTOQSTR(userName)));
    ui->statusbar->addWidget(userInfo);
    QLabel* timeInfo = new QLabel;
    tmpWidgetPointer.push_back(timeInfo);
    QTime time = QTime::currentTime();
    QDate date = QDate::currentDate();
    timeInfo->setText("登陆时间：" + date.toString("yyyy-MM-dd") + " " + time.toString("hh:mm:ss"));
    ui->statusbar->addWidget(timeInfo);
    QLabel* composedInfo = new QLabel;
    tmpWidgetPointer.push_back(composedInfo);
    composedInfo->setText("2025年面向对象程序设计C++课程设计");
    ui->statusbar->addWidget(composedInfo);
}

DashBoard::~DashBoard()
{
    delete ui;
    parentPointer->show();
    for(QWidget* p : tmpWidgetPointer) {
        delete p;
    }
}

void DashBoard::closeEvent(QCloseEvent *event) {
    if(QMessageBox::question(this, "提示", "确定要退出吗？") == QMessageBox::Yes) {
        event->accept();
        this->deleteLater();
    } else {
        event->ignore();
    }
}

GENERATE_CREATE_SUBWIN_SLOT_IMPL(DashBoard::openClassSetting, ClassSettingWindow);
GENERATE_CREATE_SUBWIN_SLOT_IMPL(DashBoard::openMajorSetting, MajorSettingWindow);
GENERATE_CREATE_SUBWIN_SLOT_IMPL(DashBoard::openSubjectSetting, SubjectSettingWindow);
GENERATE_CREATE_SUBWIN_SLOT_IMPL(DashBoard::openAdminWindow, AdminWindow);
GENERATE_CREATE_SUBWIN_SLOT_IMPL(DashBoard::openMajorAnalysis, MajorAnalysis);
GENERATE_CREATE_SUBWIN_SLOT_IMPL(DashBoard::openScoreAnalysis, ScoreAnalysis);
GENERATE_CREATE_SUBWIN_SLOT_IMPL(DashBoard::openScoreManage, ScoreManage);
GENERATE_CREATE_SUBWIN_SLOT_IMPL(DashBoard::openStudentManage, StudentManage);
GENERATE_CREATE_SUBWIN_SLOT_IMPL(DashBoard::openScoreSearch, ScoreSearch);
GENERATE_CREATE_SUBWIN_SLOT_IMPL(DashBoard::openStudentSearch, StudentSearch);
