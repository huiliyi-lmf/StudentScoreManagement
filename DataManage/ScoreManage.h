#ifndef SCOREMANAGE_H
#define SCOREMANAGE_H

#include <QMainWindow>
#include <QMessageBox>
#include <QStringList>
#include"DataObjects/score.h"
#include"DataObjects/student.h"
#include"DataObjects/major.h"
#include"DataObjects/class.h"
#include"DataObjects/subject.h"
namespace Ui {
class ScoreManage;
}

class ScoreManage : public QMainWindow
{
    Q_OBJECT

public:
    explicit ScoreManage(QWidget *parent = nullptr);
    ~ScoreManage();
    void loadMajorToCombobox();
    void loadClassToCombobox();
    void loadClassToCombobox(int majorId);
    void loadSubToCombobox();
    void loadSubToCombobox(int majorId);
    void loadStuNameToCombobox();
    void loadStuNameToCombobox(int majorId,int classId);
    // 辅助方法：根据当前专业和班级选择更新学生下拉框
    void updateStudentComboBox();
    void loadScoreToTable(const std::vector<DataObject*>& score);
    void loadScoreToTable();
private:
    Ui::ScoreManage *ui;
    QWidget* parent;
    

    
public slots:
    void onMajorSelectionChanged();
    void onClassSelectionChanged();
    void onbtnAddClicked();
    void onbtnDelClicked();
    void onbtnEditClicked();
    void onbtnQueryClicked();
};

#endif // SCOREMANAGE_H
