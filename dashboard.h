#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>
#include "BaseSettings/ClassSettingWindow.h"
#include "BaseSettings/MajorSettingWindow.h"
#include "BaseSettings/SubjectSettingWindow.h"

namespace Ui {
class DashBoard;
}

class DashBoard : public QMainWindow
{
    Q_OBJECT

public:
    explicit DashBoard(QWidget *parent = nullptr);
    ~DashBoard();
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::DashBoard *ui;
    QWidget* parentPointer;

public slots:
    void openClassSetting();
    void openMajorSetting();
    void openSubjectSetting();
};

#endif // DASHBOARD_H
