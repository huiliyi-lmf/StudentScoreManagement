#include "ScoreManage.h"
#include "ui_ScoreManage.h"
#include <QInputDialog>

ScoreManage::ScoreManage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ScoreManage), parent(parent)
{
    ui->setupUi(this);
    this->ui->btnQuery->hide();
    this->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(this->ui->cboxMajors, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ScoreManage::onMajorSelectionChanged);
    QObject::connect(this->ui->cboxClass, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ScoreManage::onClassSelectionChanged);
    QObject::connect(this->ui->btnAdd,&QPushButton::clicked,this,&ScoreManage::onbtnAddClicked);
    QObject::connect(this->ui->btnDel,&QPushButton::clicked,this,&ScoreManage::onbtnDelClicked);
    QObject::connect(this->ui->btnEdit,&QPushButton::clicked,this,&ScoreManage::onbtnEditClicked);
    QObject::connect(this->ui->btnQuery,&QPushButton::clicked,this,&ScoreManage::onbtnQueryClicked);
    // 连接表格双击事件
    QObject::connect(this->ui->tbResult, &QTableWidget::cellDoubleClicked, this, &ScoreManage::onTableItemDoubleClicked);
    loadMajorToCombobox();
    loadClassToCombobox();
    loadSubToCombobox();
    loadStuNameToCombobox();
    loadScoreToTable();
    
    // 初始状态下禁用班级下拉框，直到用户选择了专业
    ui->cboxClass->setEnabled(false);
}

ScoreManage::~ScoreManage()
{
    delete ui;
    parent->setEnabled(true);
}
void ScoreManage::onbtnAddClicked(){
    QString stuName=ui->cboxStuName->currentText().trimmed();
    QString subName=ui->cboxSubjects->currentText().trimmed();
    if(stuName.isEmpty()||subName.isEmpty()){
        QMessageBox::warning(this,"警告","学生姓名或课程不能为空");
        return;
    }
    
    // 检查是否选择了有效的学生和课程
    QVariant stuData = ui->cboxStuName->currentData();
    QVariant subData = ui->cboxSubjects->currentData();
    
    if(!stuData.isValid() || stuData.toInt() <= 0) {
        QMessageBox::warning(this,"警告","请选择有效的学生");
        return;
    }
    
    if(!subData.isValid() || subData.toInt() <= 0) {
        QMessageBox::warning(this,"警告","请选择有效的课程");
        return;
    }
    
    int stuId = stuData.toInt();
    int subId = subData.toInt();
    double scoreValue = ui->editResult->value();
    
    // 验证成绩范围
    if(scoreValue < 0 || scoreValue > 100) {
        QMessageBox::warning(this,"警告","成绩应在0-100分之间");
        return;
    }
    
    // 创建Score对象并设置值
    Score score;
    score.stuId = stuId;
    score.subId = subId;
    score.score = scoreValue;
    
    // 检查是否已存在该学生该课程的成绩
    if(score.isScoreExits(this)) {
        return; // isScoreExits方法内部已经显示警告消息
    }
    
    // 插入新成绩记录
    if(score.insert()) {
        QMessageBox::information(this,"成功","成绩添加成功！");
        // 清空输入
        ui->editResult->setValue(0);
        // 刷新表格显示
        loadScoreToTable();
    } else {
        QMessageBox::critical(this,"错误","成绩添加失败！");
    }
}
void ScoreManage::onbtnDelClicked(){
    // 获取表格中当前选中的行
    int currentRow = ui->tbResult->currentRow();
    if(currentRow < 0){
        QMessageBox::warning(this, "警告", "请先选择要删除的成绩记录");
        return;
    }

    // 获取选中行的成绩ID（第6列）
    QTableWidgetItem* scoreIdItem = ui->tbResult->item(currentRow, 5);
    if(!scoreIdItem){
        QMessageBox::warning(this, "警告", "无法获取成绩ID");
        return;
    }

    int scoreId = scoreIdItem->text().toInt();
    if(scoreId <= 0){
        QMessageBox::warning(this, "警告", "无效的成绩ID");
        return;
    }

    // 获取学生姓名和课程名称用于显示确认信息
    QString stuName = ui->tbResult->item(currentRow, 1)->text();
    QString subName = ui->tbResult->item(currentRow, 4)->text();

    // 确认删除
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除", 
                                 QString("确定要删除 %1 的 %2 课程成绩吗？").arg(stuName).arg(subName),
                                 QMessageBox::Yes|QMessageBox::No);
    
    if(reply != QMessageBox::Yes){
        return;
    }

    // 执行删除操作
    Score score;
    score.id = scoreId;
    if(score.deleteData()){
        QMessageBox::information(this, "成功", "成绩删除成功");
        // 刷新表格
        loadScoreToTable();
    }else{
        QMessageBox::critical(this, "错误", "删除成绩失败");
    }
}
void ScoreManage::onbtnEditClicked(){
    // 获取表格中当前选中的行
    int currentRow = ui->tbResult->currentRow();
    if(currentRow < 0){
        QMessageBox::warning(this, "警告", "请先选择要编辑的成绩记录");
        return;
    }

    // 调用双击处理函数，并传入当前行和成绩列（第6列）
    onTableItemDoubleClicked(currentRow, 6);
}

// 新增双击处理函数
void ScoreManage::onTableItemDoubleClicked(int row, int column) {
    // 只处理成绩列（第6列）的双击事件
    if(column != 6) {
        return;
    }
    
    // 获取选中行的成绩ID（第5列）
    QTableWidgetItem* scoreIdItem = ui->tbResult->item(row, 5);
    if(!scoreIdItem){
        QMessageBox::warning(this, "警告", "无法获取成绩ID");
        return;
    }

    int scoreId = scoreIdItem->text().toInt();
    if(scoreId <= 0){
        QMessageBox::warning(this, "警告", "无效的成绩ID");
        return;
    }

    // 获取学生姓名和课程名称用于显示确认信息
    QString stuName = ui->tbResult->item(row, 1)->text();
    QString subName = ui->tbResult->item(row, 4)->text();
    QString currentScore = ui->tbResult->item(row, 6)->text();

    // 获取当前成绩记录信息
    Score score;
    if(!score.selectById(scoreId)){
        QMessageBox::warning(this, "警告", "无法找到该成绩记录");
        return;
    }

    // 创建一个对话框来确认新成绩
    bool ok;
    double newScore = QInputDialog::getDouble(this, "编辑成绩", 
                                             QString("%1 的 %2 课程成绩:").arg(stuName).arg(subName),
                                             score.score, 0, 100, 1, &ok);
    
    if(!ok){
        return; // 用户取消了编辑
    }

    // 更新成绩记录
    score.score = newScore;
    if(score.updateData()){
        QMessageBox::information(this, "成功", "成绩更新成功");
        // 刷新表格
        loadScoreToTable();
    }else{
        QMessageBox::critical(this, "错误", "更新成绩失败");
    }
}
void ScoreManage::onbtnQueryClicked(){
    // 获取选中的筛选条件
    QVariant majorData = ui->cboxMajors->currentData();
    QVariant classData = ui->cboxClass->currentData();
    QVariant subjectData = ui->cboxSubjects->currentData();
    QVariant studentData = ui->cboxStuName->currentData();
    
    // 检查是否有选择具体的专业、班级、科目和学生
    // 只有当下拉框选择了具体项目（不是默认的"请选择..."或"所有..."项）时才作为筛选条件
    int selectMajorId = (majorData.isValid() && majorData.toInt() > 0) ? majorData.toInt() : -1;
    int selectClassId = (classData.isValid() && classData.toInt() > 0) ? classData.toInt() : -1;
    int selectSubId = (subjectData.isValid() && subjectData.toInt() > 0) ? subjectData.toInt() : -1;
    int selectStudentId = (studentData.isValid() && studentData.toInt() > 0) ? studentData.toInt() : -1;
    
    // 如果没有选择任何有效的筛选条件，显示所有记录
    if(selectMajorId <= 0 && selectClassId <= 0 && selectSubId <= 0 && selectStudentId <= 0) {
        loadScoreToTable();
        return;
    }
    
    // 获取所有成绩记录
    Score scoreFetcher;
    std::vector<DataObject*> allScores = scoreFetcher.selectAll();
    std::vector<DataObject*> filteredScores;
    
    // 遍历所有成绩记录
    for(DataObject* dobj : allScores) {
        Score* score = static_cast<Score*>(dobj);
        if(!score) {
            continue;
        }
        
        bool matchesConditions = true;
        
        // 如果选择了学生ID，直接根据学生ID筛选
        if(selectStudentId > 0) {
            if(score->stuId != selectStudentId) {
                matchesConditions = false;
            }
        } else {
            // 如果没有选择具体学生，但选择了专业，则根据专业筛选
            if(selectMajorId > 0) {
                Student student;
                if(student.selectById(score->stuId)) {
                    if(student.majorID != selectMajorId) {
                        matchesConditions = false;
                    }
                } else {
                    matchesConditions = false;
                }
            }
            
            // 如果没有选择具体学生，但选择了班级，则根据班级筛选
            if(matchesConditions && selectClassId > 0) {
                Student student;
                if(student.selectById(score->stuId)) {
                    if(student.classID != selectClassId) {
                        matchesConditions = false;
                    }
                } else {
                    matchesConditions = false;
                }
            }
        }
        
        // 如果选择了科目，则根据科目筛选
        if(matchesConditions && selectSubId > 0) {
            if(score->subId != selectSubId) {
                matchesConditions = false;
            }
        }
        
        // 如果符合所有条件，添加到筛选结果中
        if(matchesConditions) {
            Score* scoreCopy = new Score();
            scoreCopy->id = score->id;
            scoreCopy->stuId = score->stuId;
            scoreCopy->subId = score->subId;
            scoreCopy->score = score->score;
            filteredScores.push_back(scoreCopy);
        }
    }
    
    // 更新表格显示
    loadScoreToTable(filteredScores);
    
    // 清理内存
    for(DataObject* dobj : allScores) {
        delete dobj;
    }
    
    for(DataObject* dobj : filteredScores) {
        delete dobj;
    }
}
void ScoreManage::loadMajorToCombobox(){
    ui->cboxMajors->clear();
    Major majorFetcher;
    std::vector<DataObject*>dataObjects=majorFetcher.selectAll();
    if(dataObjects.empty()){
        ui->cboxMajors->addItem("无可用专业");
        ui->cboxMajors->setEnabled(false);
    }else{
        ui->cboxMajors->setEnabled(true);
        ui->cboxMajors->addItem("请选择专业...",QVariant(-1));
        for(DataObject* dobj:dataObjects){
            Major* currentMajor=static_cast<Major*>(dobj);
            if(currentMajor){
                ui->cboxMajors->addItem(QString::fromStdString(currentMajor->majorName),QVariant(currentMajor->id));
            }
        }
    }
    ui->cboxMajors->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    for (DataObject* dobj : dataObjects) {
        delete dobj; // 删除每个 Major 对象
    }
}
void ScoreManage::loadClassToCombobox(){
    // 加载所有班级（当没有选择专业时）
    ui->cboxClass->clear();
    ui->cboxClass->addItem("所有班级", QVariant(-1));

    Subject subjectFetcher;
    std::vector<DataObject*> dataObjects = subjectFetcher.selectAll();

    if(!dataObjects.empty()){
        ui->cboxClass->setEnabled(true);
        for(DataObject* dobj : dataObjects){
            Class* currentClass = static_cast<Class*>(dobj);
            if(currentClass){
                ui->cboxClass->addItem(QString::fromStdString(currentClass->className), QVariant(currentClass->id));
            }
        }
    }else{
        ui->cboxClass->addItem("暂无班级");
        ui->cboxClass->setEnabled(false);
    }

    for(DataObject* dobj : dataObjects){
        delete dobj;
    }
    
    // 当没有选择专业时，禁用班级下拉框
    QVariant currentMajorData = ui->cboxMajors->currentData();
    int majorId = (currentMajorData.isValid()) ? currentMajorData.toInt() : -1;
    
    if(majorId <= 0) {
        ui->cboxClass->setEnabled(false);
    }
}

void ScoreManage::loadClassToCombobox(int majorId){
    // 根据专业ID加载对应的班级
    ui->cboxClass->clear();

    if(majorId <= 0){
        // 如果传入的专业ID无效，加载所有班级但禁用下拉框
        loadClassToCombobox();
        ui->cboxClass->setEnabled(false);
        return;
    }

    Class classFetcher;
    std::vector<DataObject*> allClasses = classFetcher.selectAll();
    std::vector<DataObject*> filteredClasses;

    // 筛选属于指定专业的科目
    for(DataObject* dobj : allClasses){
        Class* classExample = static_cast<Class*>(dobj);
        if(classExample && classExample->majorId == majorId){
            filteredClasses.push_back(classExample);
        }
    }

    if(filteredClasses.empty()){
        ui->cboxClass->addItem("该专业暂无班级");
        ui->cboxClass->setEnabled(false);
    }else{
        ui->cboxClass->setEnabled(true);
        ui->cboxClass->addItem("请选择班级...", QVariant(-1));
        for(DataObject* dobj : filteredClasses){
            Class* currentClass = static_cast<Class*>(dobj);
            if(currentClass){
                ui->cboxClass->addItem(QString::fromStdString(currentClass->className), QVariant(currentClass->id));
            }
        }
    }

    // 清理内存
    for(DataObject* dobj : allClasses){
        delete dobj;
    }
}
void ScoreManage::loadSubToCombobox(){
    // 加载所有科目（当没有选择专业时）
    ui->cboxSubjects->clear();
    ui->cboxSubjects->addItem("所有课程", QVariant(-1));

    Subject subjectFetcher;
    std::vector<DataObject*> dataObjects = subjectFetcher.selectAll();

    if(!dataObjects.empty()){
        ui->cboxSubjects->setEnabled(true);
        for(DataObject* dobj : dataObjects){
            Subject* currentSubject = static_cast<Subject*>(dobj);
            if(currentSubject){
                ui->cboxSubjects->addItem(QString::fromStdString(currentSubject->subName), QVariant(currentSubject->id));
            }
        }
    }else{
        ui->cboxSubjects->addItem("暂无课程");
        ui->cboxSubjects->setEnabled(false);
    }


    for(DataObject* dobj : dataObjects){
        delete dobj;
    }
}

void ScoreManage::loadSubToCombobox(int majorId){
    // 根据专业ID加载对应的科目
    ui->cboxSubjects->clear();

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
        ui->cboxSubjects->addItem("该专业暂无课程");
        ui->cboxSubjects->setEnabled(false);
    }else{
        ui->cboxSubjects->setEnabled(true);
        ui->cboxSubjects->addItem("请选择课程...", QVariant(-1));
        for(DataObject* dobj : filteredSubjects){
            Subject* currentSubject = static_cast<Subject*>(dobj);
            if(currentSubject){
                ui->cboxSubjects->addItem(QString::fromStdString(currentSubject->subName), QVariant(currentSubject->id));
            }
        }
    }


    // 清理内存
    for(DataObject* dobj : allSubjects){
        delete dobj;
    }
}
void ScoreManage::loadStuNameToCombobox(){
    ui->cboxStuName->clear();
    ui->cboxStuName->addItem("所有学生姓名",QVariant(-1));
    Student studentFetcher;
    std::vector<DataObject*>dataObjects=studentFetcher.selectAll();
    if(!dataObjects.empty()){
        ui->cboxStuName->setEnabled(true);
        for(DataObject* dobj : dataObjects){
            Student* currentStudent = static_cast<Student*>(dobj);
            if(currentStudent){
                ui->cboxStuName->addItem(QString::fromStdString(currentStudent->stuName), QVariant(currentStudent->id));
            }
        }
    }else{
        ui->cboxStuName->addItem("暂无学生");
        ui->cboxStuName->setEnabled(false);
    }
    for(DataObject* dobj : dataObjects){
        delete dobj;
    }
}
void ScoreManage::loadStuNameToCombobox(int majorId, int classId){
    ui->cboxStuName->clear();
    
    // 如果没有指定专业和班级，加载所有学生
    if(majorId <= 0 && classId <= 0){
        loadStuNameToCombobox();
        return;
    }
    
    // 获取所有学生
    Student studentFetcher;
    std::vector<DataObject*> allStudents = studentFetcher.selectAll();
    std::vector<DataObject*> filteredStudents;
    
    // 根据条件筛选学生
    for(DataObject* dobj : allStudents){
        Student* student = static_cast<Student*>(dobj);
        if(!student) continue;
        
        bool matchesMajor = (majorId <= 0 || student->majorID == majorId);
        bool matchesClass = (classId <= 0 || student->classID == classId);
        
        if(matchesMajor && matchesClass){
            filteredStudents.push_back(student);
        }
    }
    
    // 更新下拉框
    if(filteredStudents.empty()){
        ui->cboxStuName->addItem("暂无学生");
        ui->cboxStuName->setEnabled(false);
    } else {
        ui->cboxStuName->setEnabled(true);
        ui->cboxStuName->addItem("请选择学生...", QVariant(-1));
        
        for(DataObject* dobj : filteredStudents){
            Student* currentStudent = static_cast<Student*>(dobj);
            if(currentStudent){
                ui->cboxStuName->addItem(QString::fromStdString(currentStudent->stuName), QVariant(currentStudent->id));
            }
        }
    }
    
    // 清理内存
    for(DataObject* dobj : allStudents){
        delete dobj;
    }
}

void ScoreManage::onMajorSelectionChanged(){
    // 获取当前选中专业的ID
    QVariant currentMajorData = ui->cboxMajors->currentData();
    int majorId = (currentMajorData.isValid()) ? currentMajorData.toInt() : -1;
    
    // 根据专业ID更新班级下拉框
    if(majorId > 0){
        // 更新专业相关的下拉框
        loadClassToCombobox(majorId);
        loadSubToCombobox(majorId);
        
        // 启用班级下拉框
        ui->cboxClass->setEnabled(true);
    } else {
        // 加载所有班级和科目
        loadClassToCombobox();
        loadSubToCombobox();
        
        // 禁用班级下拉框
        ui->cboxClass->setEnabled(false);
    }
    
    // 更新学生下拉框，基于当前专业和班级选择
    updateStudentComboBox();
}

void ScoreManage::onClassSelectionChanged(){
    // 仅更新学生下拉框，不改变班级和科目列表
    updateStudentComboBox();
}

// 辅助方法：根据当前专业和班级选择更新学生下拉框
void ScoreManage::updateStudentComboBox(){
    // 获取当前选中专业和班级的ID
    QVariant currentMajorData = ui->cboxMajors->currentData();
    QVariant currentClassData = ui->cboxClass->currentData();
    
    int majorId = (currentMajorData.isValid()) ? currentMajorData.toInt() : -1;
    int classId = (currentClassData.isValid()) ? currentClassData.toInt() : -1;
    
    // 根据当前选择更新学生下拉框
    loadStuNameToCombobox(majorId, classId);
}
void ScoreManage::loadScoreToTable(const std::vector<DataObject*>& score){
    ui->tbResult->horizontalHeader()->resizeSection(0, 100);
    ui->tbResult->horizontalHeader()->resizeSection(1, 150);
    ui->tbResult->horizontalHeader()->resizeSection(2, 150);
    ui->tbResult->horizontalHeader()->resizeSection(3, 150);
    ui->tbResult->horizontalHeader()->resizeSection(4, 150);
    ui->tbResult->horizontalHeader()->resizeSection(5, 150);
    ui->tbResult->horizontalHeader()->resizeSection(6, 150);
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
void ScoreManage::loadScoreToTable(){
    Score scoreFetcher;
    std::vector<DataObject*>dataObjects=scoreFetcher.selectAll();
    loadScoreToTable(dataObjects);
    for(DataObject*dobj:dataObjects){
        delete dobj;
    }
}

