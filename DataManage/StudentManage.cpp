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
    onBtnQueryClicked();
}

void StudentManage::onBtnEditClicked() {
    int col = this->ui->tbStudent->currentColumn();
    int row = this->ui->tbStudent->currentRow();
    QString keyName, inputTitle, inputLabel;
    if(row < 0) {
        WARN_MESSAGE("请选择合法的数据！");
        return;
    }
    switch (col) {
    case 0:
        WARN_MESSAGE("编号不可修改！");
        return;
    case 1:
        keyName = "stuName";
        inputTitle = "修改学生姓名";
        inputLabel = "请输入修改后的学生姓名：";
        break;
    case 2:
        keyName = "majorId";
        inputTitle = "修改学生专业";
        inputLabel = "请选择修改后的学生专业：";
        break;
    case 3:
        keyName = "classId";
        inputTitle = "修改学生班级";
        inputLabel = "请选择修改后的学生班级：";
        break;
    case 4:
        keyName = "age";
        inputTitle = "修改学生年龄";
        inputLabel = "请输入修改后的学生年龄：";
        break;
    case 5:
        keyName = "sex";
        inputTitle = "修改学生性别";
        inputLabel = "请选择修改后的学生性别：";
        break;
    case 6:
        keyName = "phoneNum";
        inputTitle = "修改学生电话";
        inputLabel = "请输入修改后的学生姓名：";
        break;
    case 7:
        keyName = "address";
        inputTitle = "修改学生地址";
        inputLabel = "请输入修改后的学生地址：";
        break;
    default:
        WARN_MESSAGE("请选择合法的数据！");
        return;
    }

    QString value = "";
    Student stuMapper;
    if(!stuMapper.selectById(ui->tbStudent->item(row, 0)->text().toInt())) {
        ERROR_MESSAGE("修改失败！\n可能是数据库文件损坏、出现重名问题。");
        return;
    }

    if(keyName == "stuName" || keyName == "address" || keyName == "phoneNum") {
        value = QInputDialog::getText(this, inputTitle, inputLabel, QLineEdit::Normal, ui->tbStudent->currentItem()->text());
        if(value == "") {
            return;
        }
        if(keyName == "stuName") stuMapper.stuName = value.toStdString();
        else if(keyName == "address") stuMapper.stuName = value.toStdString();
        else stuMapper.phoneNum = value.toStdString();
    } else if(keyName == "majorId") {
        QStringList sl;
        for(int idx = 1; idx < ui->cboxMajor->count(); idx++) {
            sl << ui->cboxMajor->itemText(idx);
        }
        int curIdx = -1;
        for(int idx = 0; idx < sl.count(); idx++) {
            if(sl[idx] == ui->tbStudent->currentItem()->text()) {
                curIdx = idx;
                break;
            }
        }
        value = QInputDialog::getItem(this, inputTitle, inputLabel, sl, curIdx, false);
        if(value == ui->tbStudent->currentItem()->text()) {
            return;
        }
        Major majorMapper;
        auto allMajors = majorMapper.selectAll();
        int majorId = -1;
        for(auto pb : allMajors) {
            auto p = (Major*)pb;
            if(p->majorName == value.toStdString()) {
                majorId = p->id;
                break;
            }
        }
        if(majorId <= 0) {
            ERROR_MESSAGE("修改失败，专业不存在！");
            return;
        }
        stuMapper.majorID = majorId;
        if(!stuMapper.updateData()) {
            ERROR_MESSAGE("修改失败，数据库错误！");
            return;
        }
        for(auto p:allMajors) {
            delete p;
        }
    } else if(keyName == "classId") {
        QStringList sl;
        for(int idx = 1; idx < ui->cboxClass->count(); idx++) {
            sl << ui->cboxClass->itemText(idx);
        }
        int curIdx = -1;
        for(int idx = 0; idx < sl.count(); idx++) {
            if(sl[idx] == ui->tbStudent->currentItem()->text()) {
                curIdx = idx;
                break;
            }
        }
        value = QInputDialog::getItem(this, inputTitle, inputLabel, sl, curIdx, false);
        if(value == ui->tbStudent->currentItem()->text()) {
            return;
        }
        Class classMapper;
        auto allClass = classMapper.selectAll();
        int classId = -1;
        for(auto pb : allClass) {
            auto p = (Class*)pb;
            if(p->className == value.toStdString()) {
                classId = p->id;
                break;
            }
        }
        if(classId <= 0) {
            ERROR_MESSAGE("修改失败，专业不存在！");
            return;
        }
        stuMapper.classID = classId;
        if(!stuMapper.updateData()) {
            ERROR_MESSAGE("修改失败，数据库错误！");
            return;
        }
        for(auto p:allClass) {
            delete p;
        }
    } else if(keyName == "sex") {
        QStringList sl;
        sl << "男" << "女";
        value = QInputDialog::getItem(this, inputTitle, inputLabel, sl, ui->tbStudent->currentItem()->text() == "男" ?0:1, false);
        if(value == ui->tbStudent->currentItem()->text()) {
            return;
        }
        stuMapper.sex = value.toStdString();
        if(!stuMapper.updateData()) {
            ERROR_MESSAGE("修改失败，数据库错误！");
            return;
        }
    } else if(keyName == "age") {
        stuMapper.age = QInputDialog::getInt(this, inputTitle, inputLabel, ui->tbStudent->currentItem()->text().toInt());
        if(!stuMapper.updateData()) {
            ERROR_MESSAGE("修改失败，数据库错误！");
            return;
        }
    } else {
        WARN_MESSAGE("请选择合法的数据！");
        return;
    }
    SUCCESS_MESSAGE("修改成功！");
    this->onBtnQueryClicked();
}

void StudentManage::onBtnAddClicked() {
    if(ui->cboxMajor->currentData().toInt() <= 0) {
        WARN_MESSAGE("请选择具体专业！");
    }
    if(ui->cboxClass->currentData().toInt() <= 0) {
        WARN_MESSAGE("请选择具体班级！");
    }
    if(ui->editAddress->text() == "") {
        WARN_MESSAGE("请输入具体地址！");
    }
    if(ui->editAge->text() == "") {
        WARN_MESSAGE("请输入具体年龄！");
    }
    if(ui->editName->text() == "") {
        WARN_MESSAGE("请输入具体姓名！");
    }
    if(ui->editPhone->text() == "") {
        WARN_MESSAGE("请输入具体地址！");
    }
    Student stuMapper;
    stuMapper.stuName = ui->editName->text().toStdString();
    stuMapper.classID = ui->cboxClass->currentData().toInt();
    stuMapper.majorID = ui->cboxMajor->currentData().toInt();
    stuMapper.sex = ui->cboxSex->currentText().toStdString();
    stuMapper.age = ui->editAge->value();
    stuMapper.address = ui->editAddress->text().toStdString();
    stuMapper.phoneNum = ui->editPhone->text().toStdString();
    if(!stuMapper.insert()) {
        ERROR_MESSAGE("添加失败！数据库错误！");
    } else {
        SUCCESS_MESSAGE("添加成功！");
    }
}

void StudentManage::onBtnQueryClicked() {
    int majorId = ui->cboxMajor->currentData().toInt();
    int classId = ui->cboxClass->currentData().toInt();
    QString stuId = ui->editID->text();
    QString stuName = ui->editName->text();
    int age = ui->editAge->value();
    QString sex = ui->cboxSex->currentText();
    QString phone = ui->editPhone->text();
    QString address = ui->editAddress->text();
    Student stuMapper;
    auto allStus = stuMapper.selectAll();
    auto selectStus(allStus);
    selectStus.clear();
    for(auto pb:allStus) {
        auto p = (Student*)pb;
        if(majorId > 0 && p->majorID != majorId) continue;
        if(classId > 0 && p->classID != classId) continue;
        if((stuId!="" && !QString("%1").arg(p->id).contains(stuId))) continue;
        if((stuName!="" && !stuName.contains(STDTOQSTR(p->stuName))) || (address != "" && !address.contains(STDTOQSTR(p->address)))) continue;
        if(age > 0 && p->age != age) continue;
        if((sex!=""&&!sex.contains(STDTOQSTR(p->sex))) || (phone!=""&&!phone.contains(STDTOQSTR(p->phoneNum)))) continue;
        selectStus.push_back(pb);
    }
    this->loadStuToTable(selectStus);
    for(auto p:allStus) {
        delete p;
    }
}

void StudentManage::onBtnDelClicked() {
    Student stuMapper;
    int id = this->ui->tbStudent->item(this->ui->tbStudent->currentRow(), 0)->text().toInt();
    stuMapper.id = id;
    if(stuMapper.deleteData()) {
        SUCCESS_MESSAGE("删除学生信息成功！");
    } else {
        WARN_MESSAGE("删除失败！");
    }
}
