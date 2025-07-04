#ifndef SCOREANALYSIS_H
#define SCOREANALYSIS_H

#include <QMainWindow>
#include "DataObjects/dataobject.h"
#include "DataObjects/major.h"
#include "DataObjects/subject.h"
#include "DataObjects/score.h"
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
    QWidget* lastChartView;
    QChartView* createHistogramChart(const std::vector<double>& data, int binCount);
    QChartView* createScoreDistributionPieChart(const std::vector<double>& scores);
};

#endif // SCOREANALYSIS_H
