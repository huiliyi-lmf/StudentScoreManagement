#include "ScoreAnalysis.h"
#include "ui_ScoreAnalysis.h"

ScoreAnalysis::ScoreAnalysis(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ScoreAnalysis), parent(parent)
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

QChartView* createHistogramChart(const QVector<double>& data, int binCount) {
    if (data.isEmpty() || binCount < 1)
        return nullptr;

    // 计算数据范围
    auto [minVal, maxVal] = std::minmax_element(data.begin(), data.end());
    double range = *maxVal - *minVal;
    double binWidth = range / binCount;

    // 创建bin并计数
    QVector<int> binCounts(binCount, 0);
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
    axisX->setTitleText("数据区间");
    chart->addAxis(axisX, Qt::AlignBottom);
    histogram->attachAxis(axisX);

    // 创建Y轴（频率）
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("频数");
    axisY->setMin(0);
    axisY->setMax(*std::max_element(binCounts.begin(), binCounts.end()) * 1.1);
    chart->addAxis(axisY, Qt::AlignLeft);
    histogram->attachAxis(axisY);

    // 创建图表视图
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    return chartView;
}


void ScoreAnalysis::onChartChanged() {

    QVector<double> data;
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(100.0, 15.0); // 均值100，标准差15

    for (int i = 0; i < 1000; ++i) {
        data.append(distribution(generator));
    }

    QChartView *histoChart = createHistogramChart(data, 15); // 15个bins
    setCentralWidget(histoChart);

    if(ui->cboxChart->currentIndex()==0) {
        // 读取直方图

    } else {
        // 读取扇形图
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
