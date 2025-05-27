#ifndef STUDENTMANAGE_H
#define STUDENTMANAGE_H

#include <QMainWindow>
#include "DataObjects/student.h"
#include "DataObjects/class.h"
#include "DataObjects/major.h"
#include "DataObjects/class.h"
#include <vector>

namespace Ui {
class StudentManage;
}

class StudentManage : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudentManage(QWidget *parent = nullptr);
    ~StudentManage();

private:
    Ui::StudentManage *ui;
    QWidget* parent;
    void loadStuToTable();
    void loadStuToTable(const std::vector<DataObject*>& students);
    void loadMajorToCombobox();
    void loadClassToCombobox(std::string majorName = "");

private slots:
    void onMajorChanged();
    void onBtnEditClicked();
    void onBtnAddClicked();
    void onBtnQueryClicked();
    void onBtnDelClicked();
};

#endif // STUDENTMANAGE_H
