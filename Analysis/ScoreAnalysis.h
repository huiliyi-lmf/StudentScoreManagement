#ifndef SCOREANALYSIS_H
#define SCOREANALYSIS_H

#include <QMainWindow>
#include"DataObjects/dataobject.h"
#include"DataObjects/major.h"
#include"DataObjects/subject.h"
#include <QtCharts>

namespace Ui {
class ScoreAnalysis;
}

class ScoreAnalysis : public QMainWindow
{
    Q_OBJECT

public:
    explicit ScoreAnalysis(QWidget *parent = nullptr);
    ~ScoreAnalysis();
    void loadMajorToCombobox();
    void loadSubToCombobox(int majorId);
public slots:
    void onChartChanged();
    void onMajorChanged();
    void onSubjectChanged();
private:
    Ui::ScoreAnalysis *ui;
    QWidget* parent;
};

#endif // SCOREANALYSIS_H
