#include "ScoreAnalysis.h"
#include "ui_ScoreAnalysis.h"

ScoreAnalysis::ScoreAnalysis(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ScoreAnalysis), parent(parent), lastChartView(nullptr)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    loadMajorToCombobox();
    loadSubToCombobox(ui->cboxMajor->currentData().toInt());

    // 初始化图表
    ui->cboxChart->addItem("直方图", QVariant("0"));
    ui->cboxChart->addItem("扇形图", QVariant("1"));
    ui->cboxChart->setCurrentIndex(0);
    this->onChartChanged();
    QObject::connect(ui->cboxChart, &QComboBox::currentIndexChanged, this, &ScoreAnalysis::onChartChanged);
    QObject::connect(ui->cboxMajor, &QComboBox::currentIndexChanged, this, &ScoreAnalysis::onMajorChanged);
    QObject::connect(ui->cboxSub, &QComboBox::currentIndexChanged, this, &ScoreAnalysis::onSubjectChanged);
}

ScoreAnalysis::~ScoreAnalysis()
{
    delete ui;
    if(lastChartView != nullptr) {
        delete lastChartView;
        lastChartView = nullptr;
    }
    parent->setEnabled(true);
}
void ScoreAnalysis::loadMajorToCombobox(){
    ui->cboxMajor->clear();
    Major majorFetcher;
    std::vector<DataObject*>dataObjects=majorFetcher.selectAll();
    if(dataObjects.empty()){
        ui->cboxMajor->addItem("无可用专业");
        ui->cboxMajor->setEnabled(false);
    }else{
        ui->cboxMajor->setEnabled(true);
        for(DataObject* dobj:dataObjects){
            Major* currentMajor=static_cast<Major*>(dobj);
            if(currentMajor){
                ui->cboxMajor->addItem(QString::fromStdString(currentMajor->majorName),QVariant(currentMajor->id));
            }
        }
    }
    ui->cboxMajor->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    for (DataObject* dobj : dataObjects) {
        delete dobj; // 删除每个 Major 对象
    }
}
void ScoreAnalysis::loadSubToCombobox(int majorId){
    // 根据专业ID加载对应的科目
    ui->cboxSub->clear();

    if(majorId <= 0){
        return;
    }

    Subject subjectFetcher;
    std::vector<DataObject*> allSubjects = subjectFetcher.selectAll();
    std::vector<DataObject*> filteredSubjects;

    // 筛选属于指定专业的科目
    for(DataObject* dobj : allSubjects){
        Subject* subject = static_cast<Subject*>(dobj);
        if(subject && subject->majorId == majorId){
            filteredSubjects.push_back(subject);
        }
    }

    if(filteredSubjects.empty()){
        ui->cboxSub->addItem("该专业暂无课程");
        ui->cboxSub->setEnabled(false);
    }else{
        ui->cboxSub->setEnabled(true);
        for(DataObject* dobj : filteredSubjects){
            Subject* currentSubject = static_cast<Subject*>(dobj);
            if(currentSubject){
                ui->cboxSub->addItem(QString::fromStdString(currentSubject->subName), QVariant(currentSubject->id));
            }
        }
    }
    // 清理内存
    for(DataObject* dobj : allSubjects){
        delete dobj;
    }
}

QChartView* ScoreAnalysis::createHistogramChart(const std::vector<double>& data, int binCount) {
    if (data.empty() || binCount < 1)
        return nullptr;

    // 计算数据范围
    auto [minVal, maxVal] = std::minmax_element(data.begin(), data.end());
    double range = *maxVal - *minVal;
    double binWidth = range / binCount;

    // 创建bin并计数
    std::vector<int> binCounts(binCount, 0);
    for (double value : data) {
        int binIndex = qBound(0, static_cast<int>((value - *minVal) / binWidth), binCount - 1);
        binCounts[binIndex]++;
    }

    // 创建直方图条
    QBarSet *barSet = new QBarSet("");
    for (int count : binCounts) {
        *barSet << count;
    }

    // 创建直方图系列
    QBarSeries *histogram = new QBarSeries();
    histogram->append(barSet);
    histogram->setBarWidth(1.0);  // 关键：使条之间无间隔
    histogram->setVisible(true);

    // 创建图表
    QChart *chart = new QChart();
    chart->addSeries(histogram);
    chart->setTitle("数据分布直方图");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // 创建X轴（bins范围）
    QStringList categories;
    for (int i = 0; i < binCount; ++i) {
        double start = *minVal + i * binWidth;
        double end = start + binWidth;
        categories << QString("%1~%2").arg(start, 0, 'f', 1).arg(end, 0, 'f', 1);
    }

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setTitleText("成绩区间");
    chart->addAxis(axisX, Qt::AlignBottom);
    histogram->attachAxis(axisX);

    // 创建Y轴（频率）
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("人数频率");
    axisY->setMin(0);
    axisY->setMax(*std::max_element(binCounts.begin(), binCounts.end()) * 1.1);
    chart->addAxis(axisY, Qt::AlignLeft);
    histogram->attachAxis(axisY);

    // 创建图表视图
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    return chartView;
}

QChartView* ScoreAnalysis::createScoreDistributionPieChart(const std::vector<double>& scores) {
    if (scores.empty()) {
        // 处理空数据的情况
        QChart* emptyChart = new QChart();
        emptyChart->setTitle("无可用数据");

        QChartView* chartView = new QChartView(emptyChart);
        chartView->setRenderHint(QPainter::Antialiasing);
        return chartView;
    }

    // 1. 定义分数段范围（可自定义调整）
    struct ScoreRange {
        double min;
        double max;
        QString label;
        QColor color;
    };

    QVector<ScoreRange> ranges = {
        {0, 60, "不及格 (0-60)", QColor(220, 53, 69)},
        {60, 70, "及格 (60-70)", QColor(253, 126, 20)},
        {70, 80, "中 (70-80)", QColor(255, 193, 7)},
        {80, 90, "良 (80-90)", QColor(40, 167, 69)},
        {90, 100, "优 (90-100)", QColor(0, 123, 255)},
        {100, std::numeric_limits<double>::max(), "满分 (>100)", QColor(108, 117, 125)}
    };

    // 2. 统计各个分数段的人数
    QVector<int> counts(ranges.size(), 0);
    for (double score : scores) {
        for (int i = 0; i < ranges.size(); ++i) {
            if (score >= ranges[i].min && score < ranges[i].max) {
                counts[i]++;
                break;
            }
        }
    }

    // 3. 创建饼图数据序列
    QPieSeries *series = new QPieSeries();
    series->setPieSize(0.7); // 设置饼图大小（相对于图表区域）

    // 4. 添加各分数段数据到饼图
    for (int i = 0; i < ranges.size(); ++i) {
        if (counts[i] == 0) continue; // 跳过零值项

        QPieSlice *slice = series->append(ranges[i].label, counts[i]);
        slice->setColor(ranges[i].color);

        // 设置悬停效果：鼠标悬停时突出显示
        slice->setExploded(true);
        slice->setExplodeDistanceFactor(0.05); // 突出距离

        // 设置标签显示格式
        slice->setLabelVisible();
        double percentage = (counts[i] * 100.0) / scores.size();
        slice->setLabel(QString("%1\n%2人 (%3%)")
                            .arg(slice->label())
                            .arg(counts[i])
                            .arg(percentage, 0, 'f', 1));

        // 设置标签字体
        slice->setLabelFont(QFont("Microsoft YaHei", 10));
    }

    // 5. 创建图表
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("学生成绩分布扇形图");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight); // 图例放在右侧
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // 6. 设置标题和字体
    QFont titleFont("Microsoft YaHei", 14, QFont::Bold);
    chart->setTitleFont(titleFont);

    // 7. 设置图例样式
    chart->legend()->setFont(QFont("Microsoft YaHei", 9));
    chart->legend()->setMarkerShape(QLegend::MarkerShapeCircle);

    // 8. 添加总人数标注
    QGraphicsSimpleTextItem *totalText = new QGraphicsSimpleTextItem(chart);
    totalText->setText(QString("总人数: %1").arg(scores.size()));
    totalText->setPos(20, 10); // 左上角位置
    totalText->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));

    // 9. 创建图表视图
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setBackgroundBrush(QBrush(QColor(240, 240, 240))); // 设置背景色

    return chartView;
}


void ScoreAnalysis::onChartChanged() {
    int subId = ui->cboxSub->currentData().toInt();
    if(subId <= 0) {
        return;
    }
    Score scoreMapper;
    auto allScores = scoreMapper.selectAll();
    std::vector<double> scores;
    for(auto pb:allScores) {
        auto p = (Score*)pb;
        if(p->subId == subId) {
            scores.push_back(p->score);
        }
    }
    std::sort(scores.begin(), scores.end());

    if(ui->cboxChart->currentIndex()==0) {
        // 读取直方图
        QChartView *chart = createHistogramChart(scores, 15); // 15个bins
        ui->graphicsView->setChart(chart->chart());
        if(lastChartView != nullptr) {
            delete lastChartView;
            lastChartView = chart;
        }
    } else {
        // 读取扇形图
        QChartView* chart = createScoreDistributionPieChart(scores);
        ui->graphicsView->setChart(chart->chart());
        if(lastChartView != nullptr) {
            delete lastChartView;
            lastChartView = chart;
        }
    }
}

void ScoreAnalysis::onMajorChanged() {
    if(ui->cboxMajor->currentData().toInt() <= 0) {
        return;
    }
    loadSubToCombobox(ui->cboxMajor->currentData().toInt());
    this->onChartChanged();
}

void ScoreAnalysis::onSubjectChanged() {
    if(ui->cboxSub->currentData().toInt() <= 0) {
        return;
    }
    this->onChartChanged();
}
