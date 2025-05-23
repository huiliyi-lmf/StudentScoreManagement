#include "ClassSettingWindow.h"
#include "ui_ClassSettingWindow.h"
#include<vector>
#include <QMessageBox>
#include<QSqlError>

ClassSettingWindow::ClassSettingWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClassSettingWindow), parentPointer(parent)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(this->ui->btnAdd, &QPushButton::clicked, this, &ClassSettingWindow::onAddBtnClicked);
    QObject::connect(this->ui->btnDel, &QPushButton::clicked, this, &ClassSettingWindow::onDeleteBtnClicked);
    QObject::connect(this->ui->btnEdit, &QPushButton::clicked, this, &ClassSettingWindow::onUpdateBtnClicked);
    loadMajorsToComboBox();
    loadClassToTable();
}

ClassSettingWindow::~ClassSettingWindow()
{
    delete ui;
    parentPointer->setEnabled(true);
}

void ClassSettingWindow::onAddBtnClicked() {
    // 1. 获取输入
    QString className = ui->editName->text().trimmed();
    int majorId = ui->cboxGrade->currentData().toInt();
    if(className.isEmpty()){
        QMessageBox::warning(this, "警告", "班级名称不能为空！");
        ui->editName->setFocus();
        return;
    }

    // 2. 输入验证
    if (className.isEmpty()) {
        QMessageBox::warning(this, "警告", "班级名称不能为空！");
        ui->editName->setFocus();
        return;
    }
    if (majorId <= 0) {
        QMessageBox::warning(this, "警告", "请选择一个有效的专业！");
        ui->cboxGrade->setFocus();
        return;
    }

    // 4. 创建 Class 对象并插入数据库
    Class newClass;
    newClass.className = className.toStdString();
    newClass.majorId = majorId;
    if(newClass.isClassNameTakenInMajor(className.toStdString(),majorId)){
        QMessageBox::warning(this,"警告","该专业下已存在同名班级！");
        ui->editName->clear();
        return;
    }
    if (newClass.insert()) {
        QMessageBox::information(this, "成功", "班级添加成功！");
        ui->editName->clear();
        ui->cboxGrade->setCurrentIndex(0);
        loadClassToTable();
    } else {
        QMessageBox::critical(this, "错误", "班级添加失败！" );
        }
}

void ClassSettingWindow::onUpdateBtnClicked() {
    QString newClassNameQString=ui->editName->text().trimmed();
    QString idstr=ui->editID->text().trimmed();
    int majorId = ui->cboxGrade->currentData().toInt();
    if (majorId <= 0) {
        QMessageBox::warning(this, "警告", "请选择一个有效的专业！");
        ui->cboxGrade->setFocus();
        return;
    }
    if(newClassNameQString.isEmpty()||idstr.isEmpty()){
        QMessageBox::warning(this, "警告", "ID和班级名称不能为空");
        return;
    }
    bool ok;
    int id=idstr.toInt(&ok);
    if(!ok){
        QMessageBox::warning(this, "警告", "无效的ID格式");
        return;
    }
    Class classToUpdate;
    if(!classToUpdate.selectById(id)){
        QMessageBox::warning(this, "警告", "指定ID (" + idstr + ") 的班级不存在。");
        return;
    }
    std::string newClassNameStd = newClassNameQString.toStdString();

    if (classToUpdate.className != newClassNameStd) {

        if (classToUpdate.isClassNameTakenInMajor(newClassNameStd, majorId, id)) {
            QMessageBox::warning(this, "警告", "班级名称 \"" + newClassNameQString + "\" 在该专业下已被使用，请输入不同的名称。");
            this->ui->editName->clear();
            return; // 新名称冲突，中断更新
        }
    }
    classToUpdate.className=newClassNameStd;
    classToUpdate.majorId=majorId;
    if(classToUpdate.updateData()){
        QMessageBox::information(this, "成功", "班级信息更新成功！");
        ui->editName->clear();
        ui->editID->clear();
        loadClassToTable();
    }     else {
    QMessageBox::critical(this, "错误", "班级信息更新失败。");
}

}
void ClassSettingWindow::onDeleteBtnClicked() {
    QString idStr = ui->editID->text().trimmed();
    if ( idStr.isEmpty()) {
        QMessageBox::warning(this, "警告", "ID不能为空");
        return;
    }
    bool ok;
    int id = idStr.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "警告", "无效的ID格式");
        return;
    }
    Class classFetcher;
    classFetcher.id=id;
    if(classFetcher.deleteData()){
        QMessageBox::information(this, "成功", "删除成功");
        ui->editID->clear();
        loadClassToTable();
    }
    else{
        QMessageBox::critical(this, "错误", "删除失败");
    }
    
}
void ClassSettingWindow::loadMajorsToComboBox(){
    ui->cboxGrade->clear();
    Major majorFetcher;
    std::vector<DataObject*>dataObjects=majorFetcher.selectAll();
    if(dataObjects.empty()){
        ui->cboxGrade->addItem("无可用专业");
        ui->cboxGrade->setEnabled(false);
    }else{
        ui->cboxGrade->setEnabled(true);
        ui->cboxGrade->addItem("请选择专业...",QVariant(-1));
        for(DataObject* dobj:dataObjects){
            Major* currentMajor=static_cast<Major*>(dobj);
            if(currentMajor){
                ui->cboxGrade->addItem(QString::fromStdString(currentMajor->majorName),QVariant(currentMajor->id));
            }
        }
    }
    ui->cboxGrade->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    for (DataObject* dobj : dataObjects) {
        delete dobj; // 删除每个 Major 对象
    }
}
void ClassSettingWindow::onExitBtnClicked(){
    parentPointer->setEnabled(true);
    this->close();
}
void ClassSettingWindow::loadClassToTable(){
Class classFetcher;
    std::vector<DataObject*>dataObjects=classFetcher.selectAll();
    ui->tbClass->setColumnCount(3);
    ui->tbClass->setRowCount(0);
    ui->tbClass->setRowCount(dataObjects.size());
    for(size_t row=0;row<dataObjects.size();++row){
        Class* currentClass=static_cast<Class*>(dataObjects[row]);
        if(!currentClass){
            continue;
        }
        QTableWidgetItem*idItem=new QTableWidgetItem(QString::number(currentClass->id));
        QTableWidgetItem *majorIdItem = new QTableWidgetItem(QString::number(currentClass->majorId));
        QTableWidgetItem *classNameItem = new QTableWidgetItem(QString::fromStdString(currentClass->className));
        ui->tbClass->setItem(row, 0, idItem);   // 第0列放ID
        ui->tbClass->setItem(row, 1, majorIdItem); // 第1列放专业Id
        ui->tbClass->setItem(row, 2, classNameItem); // 第2列放班级名称
    }
    for(DataObject*dobj:dataObjects){
        delete dobj;
    }
     ui->tbClass->resizeColumnsToContents();
}

