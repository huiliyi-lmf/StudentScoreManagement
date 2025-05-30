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

void ScoreAnalysis::onChartChanged() {
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
