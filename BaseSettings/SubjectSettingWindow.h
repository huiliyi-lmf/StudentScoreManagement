#ifndef SUBJECTSETTINGWINDOW_H
#define SUBJECTSETTINGWINDOW_H
#include"DataObjects/major.h"
#include <QMainWindow>
#include"DataObjects/subject.h"
#include<vector>

namespace Ui {
class SubjectSettingWindow;
}

class SubjectSettingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SubjectSettingWindow(QWidget *parent = nullptr);
    ~SubjectSettingWindow();
    void loadMajorsToComboBox();
    void loadSubjectsToTable();
private:
    Ui::SubjectSettingWindow *ui;
    QWidget* parentPointer;

public slots:
    void onAddBtnClicked();
    void onUpdateBtnClicked();
    void onDeleteBtnClicked();
    void onExitBtnClicked();
    void onTbSubjectItemDoubleClicked(int row, int column); // 新增的槽函数
};

#endif // SUBJECTSETTINGWINDOW_H
