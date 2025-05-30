#ifndef SCOREANALYSIS_H
#define SCOREANALYSIS_H

#include <QMainWindow>
#include"DataObjects/dataobject.h"
#include"DataObjects/major.h"
#include"DataObjects/subject.h"
namespace Ui {
class ScoreAnalysis;
}

class ScoreAnalysis : public QMainWindow
{
    Q_OBJECT

public:
    explicit ScoreAnalysis(QWidget *parent = nullptr);
    ~ScoreAnalysis();
    void loadSubToCombobox();
    void loadMajorToCombobox();
    void loadSubToCombobox(int majorId);
    void loadChartToCombobox();

private:
    Ui::ScoreAnalysis *ui;
    QWidget* parent;
};

#endif // SCOREANALYSIS_H
