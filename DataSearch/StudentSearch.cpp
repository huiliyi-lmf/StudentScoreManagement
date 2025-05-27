#include "StudentSearch.h"
#include "ui_StudentSearch.h"

StudentSearch::StudentSearch(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StudentSearch), parent(parent)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(this->ui->btnQuery, &QPushButton::clicked, this, &StudentSearch::onSearchBtnClicked);
    QObject::connect(this->ui->editKey, &QLineEdit::textChanged, this, &StudentSearch::onKeywordChanged);
    
    // 初始化界面
    loadStuIdToComboBox();  // 初始化查询条件下拉框
    loadStuToTable();       // 加载并显示所有学生数据
}

StudentSearch::~StudentSearch()
{
    delete ui;
    parent->setEnabled(true);
}

void StudentSearch::onSearchBtnClicked(){
    QString searchCondition = ui->cboxCondition->currentText();
    QString keyword = ui->editKey->text().trimmed();
    
    Student studentFetcher;
    std::vector<DataObject*> allStudents = studentFetcher.selectAll();
    
    // 检查数据库是否有学生数据
    if(allStudents.empty()){
        QMessageBox::information(this, "提示", "数据库中暂无学生信息！");
        this->setWindowTitle("学生信息查询 - 数据库为空");
        // 清空表格
        ui->tbStudent->setRowCount(0);
        return;
    }
    
    std::vector<DataObject*> filteredStudents;
    
    // 根据选择的条件进行过滤
    for(DataObject* dobj : allStudents){
        Student* student = static_cast<Student*>(dobj);
        if(!student) continue;
        
        bool matches = false;
        
        if(keyword.isEmpty()){
            // 如果没有输入关键字，显示所有学生
            matches = true;
        }
        else if(searchCondition == "学生编号"){
            // 按学生编号查询（支持部分匹配）
            QString studentId = QString::number(student->id);
            matches = studentId.contains(keyword, Qt::CaseInsensitive);
        }
        else if(searchCondition == "学生姓名"){
            // 按学生姓名查询（支持部分匹配）
            QString studentName = QString::fromStdString(student->stuName);
            matches = studentName.contains(keyword, Qt::CaseInsensitive);
        }
        else if(searchCondition == "专业"){
            // 按专业查询（需要先获取专业名称）
            Major major;
            if(major.selectById(student->majorID)){
                QString majorName = QString::fromStdString(major.majorName);
                matches = majorName.contains(keyword, Qt::CaseInsensitive);
            }
        }
        else if(searchCondition == "性别"){
            // 按性别查询（精确匹配或部分匹配）
            QString studentSex = QString::fromStdString(student->sex);
            matches = studentSex.contains(keyword, Qt::CaseInsensitive);
        }
        else if(searchCondition == "家庭住址"){
            // 按家庭住址查询（支持部分匹配）
            QString studentAddress = QString::fromStdString(student->address);
            matches = studentAddress.contains(keyword, Qt::CaseInsensitive);
        }
        
        if(matches){
            filteredStudents.push_back(student);
        }
    }
    
    // 显示过滤后的结果
    loadStuToTable(filteredStudents);
    
    // 根据查询结果显示不同的提示信息
    QString resultInfo;
    if(keyword.isEmpty()){
        // 显示所有学生
        resultInfo = QString("显示所有学生，共 %1 条记录").arg(filteredStudents.size());
        this->setWindowTitle("学生信息查询 - " + resultInfo);
    }
    else if(filteredStudents.empty()){
        // 查无结果
        QString searchType;
        if(searchCondition == "学生编号") searchType = "编号";
        else if(searchCondition == "学生姓名") searchType = "姓名";
        else if(searchCondition == "专业") searchType = "专业";
        else if(searchCondition == "性别") searchType = "性别";
        else if(searchCondition == "家庭住址") searchType = "家庭住址";
        
        QString message = QString("未找到%1包含 \"%2\" 的学生信息！\n\n请检查：\n1. 输入的关键字是否正确\n2. 选择的查询条件是否正确\n3. 该学生是否已录入系统")
                         .arg(searchType).arg(keyword);
        
        QMessageBox::information(this, "查询结果", message);
        resultInfo = QString("查无结果 - 关键字：%1").arg(keyword);
        this->setWindowTitle("学生信息查询 - " + resultInfo);
    }
    else{
        // 找到结果
        QString searchType;
        if(searchCondition == "学生编号") searchType = "编号";
        else if(searchCondition == "学生姓名") searchType = "姓名";
        else if(searchCondition == "专业") searchType = "专业";
        else if(searchCondition == "性别") searchType = "性别";
        else if(searchCondition == "家庭住址") searchType = "家庭住址";
        
        resultInfo = QString("按%1查询 \"%2\"，找到 %3 条记录").arg(searchType).arg(keyword).arg(filteredStudents.size());
        this->setWindowTitle("学生信息查询 - " + resultInfo);
        
        // 如果只找到一条记录，可以给出特别提示
        if(filteredStudents.size() == 1){
            Student* foundStudent = static_cast<Student*>(filteredStudents[0]);
            if(foundStudent){
                QString successMessage = QString("找到学生：%1 (编号：%2)")
                                       .arg(QString::fromStdString(foundStudent->stuName))
                                       .arg(foundStudent->id);
                QMessageBox::information(this, "查询成功", successMessage);
            }
        }
        else if(filteredStudents.size() <= 5){
            // 如果找到的结果较少（5条以内），可以在提示中列出所有学生姓名
            QStringList studentNames;
            for(DataObject* dobj : filteredStudents){
                Student* student = static_cast<Student*>(dobj);
                if(student){
                    studentNames << QString::fromStdString(student->stuName);
                }
            }
            QString successMessage = QString("找到 %1 名学生：\n%2")
                                   .arg(filteredStudents.size())
                                   .arg(studentNames.join("、"));
            QMessageBox::information(this, "查询成功", successMessage);
        }
    }
    
    // 清理内存
    for(DataObject* dobj : allStudents){
        delete dobj;
    }
}
void StudentSearch::loadStuIdToComboBox(){
    ui->cboxCondition->clear();
    ui->cboxCondition->addItem("学生编号");
    ui->cboxCondition->addItem("学生姓名");
    ui->cboxCondition->addItem("专业");
    ui->cboxCondition->addItem("性别");
    ui->cboxCondition->addItem("家庭住址");
    ui->cboxCondition->setCurrentIndex(0); // 默认选择第一项
}
void StudentSearch::loadStuToTable(const std::vector<DataObject*>& students){
    ui->tbStudent->setColumnCount(8);
    ui->tbStudent->setRowCount(0);
    ui->tbStudent->setRowCount(students.size());
    for(size_t row=0;row<students.size();++row){
        Student* currentStudent=static_cast<Student*>(students[row]);
        if(!currentStudent){
            continue;
        }
        Major major;
        major.selectById(currentStudent->majorID);
        Class classFetcher;
        classFetcher.selectById(currentStudent->classID);
        QTableWidgetItem*idItem=new QTableWidgetItem(QString::number(currentStudent->id));
        QTableWidgetItem*stuNameItem=new QTableWidgetItem(QString::fromStdString(currentStudent->stuName));
        QTableWidgetItem*stuSubItem=new QTableWidgetItem(QString::fromStdString(major.majorName));
        QTableWidgetItem*stuClassItem=new QTableWidgetItem(QString::fromStdString(classFetcher.className));
        QTableWidgetItem*ageItem=new QTableWidgetItem(QString::number(currentStudent->age));//年龄
        QTableWidgetItem*genderItem=new QTableWidgetItem(QString::fromStdString(currentStudent->sex));//性别
        QTableWidgetItem*phoneNumItem=new QTableWidgetItem(QString::fromStdString(currentStudent->phoneNum));//联系电话
        QTableWidgetItem*addressItem=new QTableWidgetItem(QString::fromStdString(currentStudent->address));//家庭住址
        ui->tbStudent->setItem(row,0,idItem);
        ui->tbStudent->setItem(row,1,stuNameItem);
        ui->tbStudent->setItem(row,2,stuSubItem);
        ui->tbStudent->setItem(row,3,stuClassItem);
        ui->tbStudent->setItem(row,4,ageItem);
        ui->tbStudent->setItem(row,5,genderItem);
        ui->tbStudent->setItem(row,6,phoneNumItem);
        ui->tbStudent->setItem(row,7,addressItem);
    }
    ui->tbStudent->resizeColumnsToContents();
}
void StudentSearch::loadStuToTable(){
    Student studentFetcher;
    std::vector<DataObject*> dataObjects = studentFetcher.selectAll();
    loadStuToTable(dataObjects);
    // 清理内存
    for(DataObject* dobj : dataObjects){
        delete dobj;
    }
}

void StudentSearch::onKeywordChanged(){
    QString keyword = ui->editKey->text().trimmed();
    
    // 如果输入框被清空，自动显示所有学生
    if(keyword.isEmpty()){
        loadStuToTable();
        this->setWindowTitle("学生信息查询 - 显示所有学生");
    }
}
