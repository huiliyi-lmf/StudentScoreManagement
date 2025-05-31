#include "AiScoreWindow.h"
#include "ui_AiScoreWindow.h"

AiScoreWindow::AiScoreWindow(QWidget *parent)
    : AIBaseWindow(parent)
    , ui(new Ui::AiScoreWindow)
{
    ui->setupUi(this);
    loadMajorToCombobox();
    loadSubToCombobox(ui->cboxMajor->currentData().toInt());
    QObject::connect(ui->cboxSub, &QComboBox::currentTextChanged, this, &AiScoreWindow::onSubjectChanged);
    QObject::connect(ui->cboxMajor, &QComboBox::currentTextChanged, this, &AiScoreWindow::onMajorChanged);
    onSubjectChanged();
}

AiScoreWindow::~AiScoreWindow()
{
    delete ui;
}

void AiScoreWindow::sendData() {
    int subId = ui->cboxSub->currentData().toInt();
    Score scoreMapper;
    auto allScores = scoreMapper.selectAll();
    std::vector<double> scores;
    for(auto pb : allScores) {
        auto p = (Score*)pb;
        if(p->subId == subId) {
            scores.push_back(p->score);
        }
    }
    QStringList sl;
    sl << "{\n" << "    \"major\": \"" << ui->cboxMajor->currentText() << "\", \n";
    sl << "    \"subject\": \"" << ui->cboxSub->currentText() << "\", \n";
    sl << "    \"scores\": [";
    for(int i = 0; i < scores.size(); i++) {
        sl << QString("%1").arg(scores[i]) << (i==scores.size()-1 ? "]" : ", ");
    }
    sl << "\n}";
    QString query = "";
    for(auto s : sl) {
        query += s;
    }
    ui->showEdit->clear();
    auto widgets = new std::vector<QWidget*>;
    widgets->push_back(ui->cboxMajor);
    widgets->push_back(ui->cboxSub);
    this->sendAndFlush(ui->showEdit, QString("%1/chat/analysis").arg(STDTOQSTR(serverUrl)), query, widgets);
}

void AiScoreWindow::loadMajorToCombobox(){
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
void AiScoreWindow::loadSubToCombobox(int majorId){
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

void AiScoreWindow::onSubjectChanged() {
    if(isMajorChanging) return;
    this->sendData();
}

void AiScoreWindow::onMajorChanged() {
    isMajorChanging = true;
    this->loadSubToCombobox(ui->cboxMajor->currentData().toInt());
    isMajorChanging = false;
    onSubjectChanged();
}
