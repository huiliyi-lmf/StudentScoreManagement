#include "MajorAnalysis.h"
#include "ui_MajorAnalysis.h"
#include <QMessageBox>
#include <QBarSet>
#include <vector>
#include <QTimer>

MajorAnalysis::MajorAnalysis(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MajorAnalysis), parent(parent)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    
    // 加载专业数据并创建柱状图
    loadMajorData();
    createMajorBarChart();
}

MajorAnalysis::~MajorAnalysis()
{
    delete ui;
    parent->setEnabled(true);
}

void MajorAnalysis::loadMajorData()
{
    // 清空之前的数据
    majorNames.clear();
    studentCounts.clear();
    
    // 获取所有专业
    Major majorFetcher;
    std::vector<DataObject*> majors = majorFetcher.selectAll();
    
    if (majors.empty()) {
        QMessageBox::warning(this, "警告", "没有专业数据可供分析！");
        return;
    }
    
    // 获取每个专业的学生人数
    Student studentFetcher;
    for (DataObject* obj : majors) {
        Major* major = static_cast<Major*>(obj);
        if (major) {
            // 添加专业名称
            majorNames.append(QString::fromStdString(major->majorName));
            
            // 获取该专业的学生人数
            int count = studentFetcher.getStudentCountByMajorId(major->id);
            studentCounts.append(count);
        }
    }
    
    // 释放内存
    for (DataObject* obj : majors) {
        delete obj;
    }
}

void MajorAnalysis::createMajorBarChart()
{
    if (majorNames.isEmpty()) {
        return;
    }
    
    // 创建数据集
    QBarSet *barSet = new QBarSet("专业人数");
    
    // 设置柱子颜色
    barSet->setColor(QColor(65, 105, 225)); // 蓝色
    barSet->setBorderColor(QColor(25, 25, 112)); // 深蓝色边框
    
    // 填充数据
    for (int count : studentCounts) {
        *barSet << count;
    }
    
    // 创建柱状图系列
    QBarSeries *series = new QBarSeries();
    series->append(barSet);
    
    // 设置柱子宽度（0-1之间，1表示最大宽度）
    series->setBarWidth(0.6);
    
    // 禁用自动标签，我们将手动添加标签
    series->setLabelsVisible(false);
    
    // 创建图表
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("各专业学生人数统计");
    
    // 禁用动画，有时动画会导致标签位置不正确
    chart->setAnimationOptions(QChart::NoAnimation);
    
    // 设置标题字体
    QFont titleFont;
    titleFont.setBold(true);
    titleFont.setPointSize(14);
    chart->setTitleFont(titleFont);
    
    // 设置X轴（专业名称）
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(majorNames);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    
    // 设置X轴标签字体
    QFont axisXFont;
    axisXFont.setBold(true);
    axisXFont.setPointSize(9);
    axisX->setLabelsFont(axisXFont);
    
    // 设置Y轴（学生人数）
    QValueAxis *axisY = new QValueAxis();
    // 增加Y轴上限，确保有足够空间显示标签
    int maxValue = *std::max_element(studentCounts.begin(), studentCounts.end());
    axisY->setRange(0, maxValue * 1.3); // 留出更多空间给标签
    axisY->setTitleText("学生人数");
    axisY->setTickCount(12);
    axisY->applyNiceNumbers(); // 使Y轴刻度更美观
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    
    // 设置Y轴标题字体
    QFont axisYTitleFont;
    axisYTitleFont.setBold(true);
    axisYTitleFont.setPointSize(10);
    axisY->setTitleFont(axisYTitleFont);
    
    // 设置图例
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    
    // 设置图表主题
    chart->setTheme(QChart::ChartThemeLight);
    
    // 应用到ChartView
    ui->graphicsView->setChart(chart);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    
    // 直接添加标签，不使用信号连接
    QTimer::singleShot(100, this, [=]() {
        // 获取图表的绘图区域
        QRectF plotArea = chart->plotArea();
        
        // 计算每个柱子的宽度和间距
        qreal barWidth = plotArea.width() / majorNames.size();
        
        // 为每个柱子添加标签
        for (int i = 0; i < majorNames.size(); ++i) {
            int value = studentCounts.at(i);
            
            // 创建文本标签
            QGraphicsTextItem* label = new QGraphicsTextItem(QString::number(value) + " 人", chart);
            
            // 设置字体
            QFont font;
            font.setBold(true);
            font.setPointSize(10);
            label->setFont(font);
            label->setDefaultTextColor(Qt::black);
            
            // 计算标签位置 - 在柱子正上方居中
            qreal xPos = plotArea.left() + i * barWidth + barWidth / 2 - label->boundingRect().width() / 2;
            
            // 计算柱子高度占Y轴的比例
            qreal barHeight = (value / static_cast<qreal>(axisY->max())) * plotArea.height();
            qreal yPos = plotArea.bottom() - barHeight - label->boundingRect().height() - 5;
            
            // 设置标签位置
            label->setPos(xPos, yPos);
            
            // 确保标签在图表内
            if (yPos < plotArea.top()) {
                yPos = plotArea.top();
                label->setPos(xPos, yPos);
            }
        }
    });
}
