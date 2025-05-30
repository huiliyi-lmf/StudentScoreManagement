#include "ScoreAnalysis.h"
#include "ui_ScoreAnalysis.h"

ScoreAnalysis::ScoreAnalysis(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ScoreAnalysis), parent(parent)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    loadMajorToCombobox();
    loadSubToCombobox();
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
        ui->cboxMajor->addItem("请选择专业...",QVariant(-1));
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
        // 如果传入的专业ID无效，加载所有科目
        loadSubToCombobox();
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
        ui->cboxSub->addItem("请选择课程...", QVariant(-1));
        for(DataObject* dobj : filteredSubjects){
            Subject* currentSubject = static_cast<Subject*>(dobj);
            if(currentSubject){
                ui->cboxSub->addItem(QString::fromStdString(currentSubject->subName), QVariant(currentSubject->id));
            }
        }
    }

    ui->cboxSub->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    // 清理内存
    for(DataObject* dobj : allSubjects){
        delete dobj;
    }
}
void ScoreAnalysis::loadSubToCombobox(){
    // 加载所有科目（当没有选择专业时）
    ui->cboxSub->clear();
    ui->cboxSub->addItem("所有课程", QVariant(-1));

    Subject subjectFetcher;
    std::vector<DataObject*> dataObjects = subjectFetcher.selectAll();

    if(!dataObjects.empty()){
        ui->cboxSub->setEnabled(true);
        for(DataObject* dobj : dataObjects){
            Subject* currentSubject = static_cast<Subject*>(dobj);
            if(currentSubject){
                ui->cboxSub->addItem(QString::fromStdString(currentSubject->subName), QVariant(currentSubject->id));
            }
        }
    }else{
        ui->cboxSub->addItem("暂无课程");
        ui->cboxSub->setEnabled(false);
    }

    ui->cboxSub->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    for(DataObject* dobj : dataObjects){
        delete dobj;
    }
}
