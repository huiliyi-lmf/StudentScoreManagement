#ifndef MAJORSETTINGWINDOW_H
#define MAJORSETTINGWINDOW_H
#include "DataObjects/major.h"
#include <QMainWindow>
#include<QMessageBox>

namespace Ui {
class MajorSettingWindow;
}

class MajorSettingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MajorSettingWindow(QWidget *parent = nullptr);
    ~MajorSettingWindow();

private:
    Ui::MajorSettingWindow *ui;
    QWidget* parentPointer;
    void loadMajorsToTable();

public slots:
    void onAddBtnClicked();
    void onUpdateBtnClicked();
    void onDeleteBtnClicked();
    void onExitBtnClicked();
};

#endif // MAJORSETTINGWINDOW_H
