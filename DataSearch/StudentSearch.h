#ifndef STUDENTSEARCH_H
#define STUDENTSEARCH_H

#include <QMainWindow>
#include <QMessageBox>
#include <QStringList>

#include"DataObjects/student.h"
#include"DataObjects/major.h"
#include"DataObjects/class.h"
namespace Ui {
class StudentSearch;
}

class StudentSearch : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudentSearch(QWidget *parent = nullptr);
    ~StudentSearch();
    void loadStuIdToComboBox();
    void loadStuToTable();
    void loadStuToTable(const std::vector<DataObject*>& students);
private:
    Ui::StudentSearch *ui;
    QWidget* parent;
public slots:
    void onSearchBtnClicked();
    void onKeywordChanged();
};

#endif // STUDENTSEARCH_H
