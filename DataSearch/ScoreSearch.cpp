#include "ScoreSearch.h"
#include "ui_ScoreSearch.h"

ScoreSearch::ScoreSearch(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ScoreSearch), parent(parent)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->ui->btnQuery->hide();
    QObject::connect(this->ui->btnQuery, &QPushButton::clicked, this, &ScoreSearch::onSearchBtnClicked);
    QObject::connect(this->ui->cboxMajor, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ScoreSearch::onMajorSelectionChanged);
    loadScoreToTable();
    loadMajorToCombobox();
    loadSubToCombobox(); // 初始化时加载所有科目
}

ScoreSearch::~ScoreSearch()
{
    delete ui;
    parent->setEnabled(true);
}

void ScoreSearch::onSearchBtnClicked(){
    // 获取查询条件
    QString studentName = ui->editName->text().trimmed();
    int selectedMajorId = ui->cboxMajor->currentData().toInt();
    int selectedSubjectId = ui->cboxSubject->currentData().toInt();
    
    // 获取所有成绩记录
    Score scoreFetcher;
    std::vector<DataObject*> allScores = scoreFetcher.selectAll();
    std::vector<DataObject*> filteredScores;
    
    // 根据条件筛选成绩记录
    for(DataObject* dobj : allScores){
        Score* score = static_cast<Score*>(dobj);
        if(!score) continue;
        
        bool matchesConditions = true;
        
        // 如果输入了学生姓名，检查是否匹配
        if(!studentName.isEmpty()){
            Student student;
            if(student.selectById(score->stuId)){
                QString stuName = QString::fromStdString(student.stuName);
                if(!stuName.contains(studentName, Qt::CaseInsensitive)){
                    matchesConditions = false;
                }
            } else {
                matchesConditions = false;
            }
        } 
        // 如果没有输入学生姓名，则根据专业和课程筛选
        else {
            // 检查专业是否匹配
            if(selectedMajorId > 0){
                Student student;
                if(student.selectById(score->stuId)){
                    if(student.majorID != selectedMajorId){
                        matchesConditions = false;
                    }
                } else {
                    matchesConditions = false;
                }
            }
            
            // 检查课程是否匹配
            if(selectedSubjectId > 0 && score->subId != selectedSubjectId){
                matchesConditions = false;
            }
        }
        
        // 如果符合所有条件，添加到筛选结果中
        if(matchesConditions){
            Score* scoreCopy = new Score();
            scoreCopy->id = score->id;
            scoreCopy->stuId = score->stuId;
            scoreCopy->subId = score->subId;
            scoreCopy->score = score->score;
            filteredScores.push_back(scoreCopy);
        }
    }
    
    // 显示筛选后的结果
    loadScoreToTable(filteredScores);
    
    // 更新窗口标题显示查询结果数量
    QString resultInfo;
    if(!studentName.isEmpty()){
        resultInfo = QString("按学生姓名 \"%1\" 查询，找到 %2 条记录").arg(studentName).arg(filteredScores.size());
    } else {
        QString majorName = (selectedMajorId > 0) ? ui->cboxMajor->currentText() : "所有专业";
        QString subjectName = (selectedSubjectId > 0) ? ui->cboxSubject->currentText() : "所有课程";
        resultInfo = QString("按专业 \"%1\"、课程 \"%2\" 查询，找到 %3 条记录")
                    .arg(majorName).arg(subjectName).arg(filteredScores.size());
    }
    this->setWindowTitle("学生成绩查询 - " + resultInfo);
    
    // 显示查询结果提示
    if(filteredScores.empty()){
        QString message;
        if(!studentName.isEmpty()){
            message = QString("未找到姓名包含 \"%1\" 的学生成绩记录！").arg(studentName);
        } else {
            QString majorName = (selectedMajorId > 0) ? ui->cboxMajor->currentText() : "所有专业";
            QString subjectName = (selectedSubjectId > 0) ? ui->cboxSubject->currentText() : "所有课程";
            message = QString("未找到专业 \"%1\"、课程 \"%2\" 的学生成绩记录！").arg(majorName).arg(subjectName);
        }
        // QMessageBox::information(this, "查询结果", message);
    }
    
    // 清理内存
    for(DataObject* dobj : allScores){
        delete dobj;
    }
    
    // 如果创建了筛选后的副本，也需要清理
    for(DataObject* dobj : filteredScores){
        delete dobj;
    }
}
void ScoreSearch::loadMajorToCombobox(){
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
void ScoreSearch::loadSubToCombobox(){
    // 加载所有科目（当没有选择专业时）
    ui->cboxSubject->clear();
    ui->cboxSubject->addItem("所有课程", QVariant(-1));
    
    Subject subjectFetcher;
    std::vector<DataObject*> dataObjects = subjectFetcher.selectAll();
    
    if(!dataObjects.empty()){
        ui->cboxSubject->setEnabled(true);
        for(DataObject* dobj : dataObjects){
            Subject* currentSubject = static_cast<Subject*>(dobj);
            if(currentSubject){
                ui->cboxSubject->addItem(QString::fromStdString(currentSubject->subName), QVariant(currentSubject->id));
            }
        }
    }else{
        ui->cboxSubject->addItem("暂无课程");
        ui->cboxSubject->setEnabled(false);
    }
    
    ui->cboxSubject->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    for(DataObject* dobj : dataObjects){
        delete dobj;
    }
}

void ScoreSearch::loadSubToCombobox(int majorId){
    // 根据专业ID加载对应的科目
    ui->cboxSubject->clear();
    
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
        ui->cboxSubject->addItem("该专业暂无课程");
        ui->cboxSubject->setEnabled(false);
    }else{
        ui->cboxSubject->setEnabled(true);
        ui->cboxSubject->addItem("请选择课程...", QVariant(-1));
        for(DataObject* dobj : filteredSubjects){
            Subject* currentSubject = static_cast<Subject*>(dobj);
            if(currentSubject){
                ui->cboxSubject->addItem(QString::fromStdString(currentSubject->subName), QVariant(currentSubject->id));
            }
        }
    }
    
    ui->cboxSubject->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    
    // 清理内存
    for(DataObject* dobj : allSubjects){
        delete dobj;
    }
}

void ScoreSearch::onMajorSelectionChanged(){
    // 获取当前选中专业的ID
    QVariant currentData = ui->cboxMajor->currentData();
    
    if(currentData.isValid()){
        int majorId = currentData.toInt();
        if(majorId > 0){
            // 选择了有效的专业，加载该专业的科目
            loadSubToCombobox(majorId);
        }else{
            // 选择了"请选择专业..."或其他无效选项，加载所有科目
            loadSubToCombobox();
        }
    }else{
        // 没有有效数据，加载所有科目
        loadSubToCombobox();
    }
}
void ScoreSearch::loadScoreToTable(const std::vector<DataObject*>& score){
    ui->tbResult->setColumnCount(7);
    ui->tbResult->setRowCount(0);
    ui->tbResult->setRowCount(score.size());
    for(size_t row=0;row<score.size();++row){
        Score* currentScore=static_cast<Score*>(score[row]);
        if(!currentScore){
            continue;
        }
        Student student;
        student.selectById(currentScore->stuId);
        Subject subject;
        subject.selectById(currentScore->subId);
        Major major;
        major.selectById(student.majorID);
        Class classFetcher;
        classFetcher.selectById(student.classID);
        QTableWidgetItem*stuIdItem=new QTableWidgetItem(QString::number(student.id));
        QTableWidgetItem*stuNameItem=new QTableWidgetItem(QString::fromStdString(student.stuName));
        QTableWidgetItem*majorItem=new QTableWidgetItem(QString::fromStdString(major.majorName));
        QTableWidgetItem*classNameItem=new QTableWidgetItem(QString::fromStdString(classFetcher.className));
        QTableWidgetItem*stuSubItem=new QTableWidgetItem(QString::fromStdString(subject.subName));
        QTableWidgetItem*resultIdItem=new QTableWidgetItem(QString::number(currentScore->id));
        QTableWidgetItem*resultItem=new QTableWidgetItem(QString::number(currentScore->score));
        ui->tbResult->setItem(row,0,stuIdItem);
        ui->tbResult->setItem(row,1,stuNameItem);
        ui->tbResult->setItem(row,2,majorItem);
        ui->tbResult->setItem(row,3,classNameItem);
        ui->tbResult->setItem(row,4,stuSubItem);
        ui->tbResult->setItem(row,5,resultIdItem);
        ui->tbResult->setItem(row,6,resultItem);
    }
}
void ScoreSearch::loadScoreToTable(){
    Score scoreFetcher;
    std::vector<DataObject*>dataObjects=scoreFetcher.selectAll();
    loadScoreToTable(dataObjects);
    for(DataObject*dobj:dataObjects){
        delete dobj;
    }
}
