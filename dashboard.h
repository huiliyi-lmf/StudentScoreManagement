#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>
#include <QLabel>
#include <QTime>
#include "MyMacro.h"
#include "BaseSettings/ClassSettingWindow.h"
#include "BaseSettings/MajorSettingWindow.h"
#include "BaseSettings/SubjectSettingWindow.h"
#include "BaseSettings/AdminWindow.h"
#include "Analysis/MajorAnalysis.h"
#include "Analysis/ScoreAnalysis.h"
#include "DataManage/ScoreManage.h"
#include "DataManage/StudentManage.h"
#include "DataSearch/ScoreSearch.h"
#include "DataSearch/StudentSearch.h"
#include "AIModel/AiChatWindow.h"
#include "AIModel/AiScoreWindow.h"

namespace Ui {
class DashBoard;
}

class DashBoard : public QMainWindow
{
    Q_OBJECT

public:
    explicit DashBoard(QWidget *parent = nullptr, std::string userName = "null");
    ~DashBoard();
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::DashBoard *ui;
    QWidget* parentPointer;
    std::vector<QWidget*> tmpWidgetPointer;

public slots:
    void openClassSetting();
    void openMajorSetting();
    void openSubjectSetting();
    void openAdminWindow();
    void openMajorAnalysis();
    void openScoreAnalysis();
    void openScoreManage();
    void openStudentManage();
    void openScoreSearch();
    void openStudentSearch();
    void openAiChat();
    void openAiScore();
};

#endif // DASHBOARD_H
