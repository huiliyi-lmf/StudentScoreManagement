#ifndef MAJORANALYSIS_H
#define MAJORANALYSIS_H
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QMainWindow>
#include "DataObjects/major.h"
#include "DataObjects/student.h"



namespace Ui {
class MajorAnalysis;
}

class MajorAnalysis : public QMainWindow
{
    Q_OBJECT

public:
    explicit MajorAnalysis(QWidget *parent = nullptr);
    ~MajorAnalysis();

private:
    Ui::MajorAnalysis *ui;
    QWidget* parent;
    
    // 加载专业数据
    void loadMajorData();
    
    // 创建柱状图
    void createMajorBarChart();
    
    // 存储专业名称和对应的学生人数
    QStringList majorNames;
    QList<int> studentCounts;
};

#endif // MAJORANALYSIS_H
