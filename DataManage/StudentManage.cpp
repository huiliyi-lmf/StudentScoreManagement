#include "StudentManage.h"
#include "ui_StudentManage.h"

StudentManage::StudentManage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StudentManage), parent(parent)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    QObject::connect(this->ui->cboxMajor, &QComboBox::currentIndexChanged, this, &StudentManage::onMajorChanged);
    QBUTTON_CONNECT_MYCREATE(this->ui->btnAdd, &StudentManage::onBtnAddClicked);
    QBUTTON_CONNECT_MYCREATE(this->ui->btnDel, &StudentManage::onBtnDelClicked);
    QBUTTON_CONNECT_MYCREATE(this->ui->btnEdit, &StudentManage::onBtnEditClicked);
    QBUTTON_CONNECT_MYCREATE(this->ui->btnQuery, &StudentManage::onBtnQueryClicked);

    this->loadStuToTable();
    this->loadMajorToCombobox();
    this->loadClassToCombobox();
}

StudentManage::~StudentManage()
{
    delete ui;
    parent->setEnabled(true);
}

void StudentManage::loadStuToTable(const std::vector<DataObject*>& students){
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
}


void StudentManage::loadStuToTable() {
    Student studentFetcher;
    std::vector<DataObject*> dataObjects = studentFetcher.selectAll();
    loadStuToTable(dataObjects);
    // 清理内存
    for(DataObject* dobj : dataObjects){
        delete dobj;
    }
}

void StudentManage::loadMajorToCombobox() {
    ui->cboxMajor->clear();
    Major majorFetcher;
    std::vector<DataObject*> dataObjects = majorFetcher.selectAll();
    if(dataObjects.empty()) {
        ui->cboxMajor->addItem("无可用专业", QVariant(-1));
        ui->cboxMajor->setEnabled(false);
    } else {
        ui->cboxMajor->setEnabled(true);
        ui->cboxMajor->addItem("请选择专业...", QVariant(-1));
        for(DataObject* dobj:dataObjects) {
            Major* currentMajor = static_cast<Major*>(dobj);
            if(currentMajor){
                ui->cboxMajor->addItem(QString::fromStdString(currentMajor->majorName), QVariant(currentMajor->id));
            }
        }
    }
    ui->cboxMajor->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    for (DataObject* dobj : dataObjects) {
        delete dobj; // 删除每个 Major 对象
    }
}

void StudentManage::loadClassToCombobox(std::string majorName) {
    this->ui->cboxClass->clear();

    Major majorMapper;
    int majorId = -1;
    auto allMajors = majorMapper.selectAll();
    for(auto pb : allMajors) {
        auto p = (Major*) pb;
        if(p->majorName == majorName) {
            majorId = p->id;
            break;
        }
    }

    Class classMapper;
    auto allClass = classMapper.selectAll();
    auto filterClass(allClass);
    if(majorName != "") {
        filterClass.clear();
        for(auto pb : allClass) {
            auto p = (Class*)pb;
            if(p->majorId == majorId) {
                filterClass.push_back(p);
            }
        }
    }
    if(filterClass.empty()) {
        this->ui->cboxClass->addItem("无可用班级", QVariant(-1));
        this->ui->cboxClass->setEnabled(false);
    } else {
        this->ui->cboxClass->setEnabled(true);
        this->ui->cboxClass->addItem("请选择班级...", QVariant(-1));
        for(auto pb : filterClass) {
            auto p = (Class*)pb;
            this->ui->cboxClass->addItem(QString::fromStdString(p->className), QVariant(p->id));
        }
    }
    for(auto p : allClass) {
        delete p;
    }
}

void StudentManage::onMajorChanged() {
    if(this->ui->cboxMajor->currentData().toInt() > 0) {
        this->loadClassToCombobox(this->ui->cboxMajor->currentText().toStdString());
    } else {
        this->loadClassToCombobox();
    }
}

void StudentManage::onBtnEditClicked() {

}

void StudentManage::onBtnAddClicked() {

}

void StudentManage::onBtnQueryClicked() {

}

void StudentManage::onBtnDelClicked() {

}
