#ifndef SCORESEARCH_H
#define SCORESEARCH_H

#include <QMainWindow>
#include <QMessageBox>
#include <QStringList>
#include"DataObjects/score.h"
#include"DataObjects/student.h"
#include"DataObjects/major.h"
#include"DataObjects/class.h"
#include"DataObjects/subject.h"
namespace Ui {
class ScoreSearch;
}

class ScoreSearch : public QMainWindow
{
    Q_OBJECT

public:
    explicit ScoreSearch(QWidget *parent = nullptr);
    ~ScoreSearch();
    void loadScoreToTable(const std::vector<DataObject*>& students);
    void loadMajorToCombobox();
    void loadSubToCombobox();
    void loadSubToCombobox(int majorId);
    void loadScoreToTable();
private:
    Ui::ScoreSearch *ui;
    QWidget* parent;
public slots:
    void onSearchBtnClicked();
    void onExitBtnClicked();
    void onMajorSelectionChanged();
};

#endif // SCORESEARCH_H
