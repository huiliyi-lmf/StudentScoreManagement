#include "ClassSettingWindow.h"
#include "ui_ClassSettingWindow.h"
#include<vector>
#include <QMessageBox>
#include<QSqlError>
#include <QInputDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QDialog>
#include <QLineEdit>

ClassSettingWindow::ClassSettingWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClassSettingWindow), parentPointer(parent)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(this->ui->btnAdd, &QPushButton::clicked, this, &ClassSettingWindow::onAddBtnClicked);
    QObject::connect(this->ui->btnDel, &QPushButton::clicked, this, &ClassSettingWindow::onDeleteBtnClicked);
    QObject::connect(this->ui->btnEdit, &QPushButton::clicked, this, &ClassSettingWindow::onUpdateBtnClicked);
    
    // 连接表格双击信号
    connect(ui->tbClass, &QTableWidget::cellDoubleClicked, this, &ClassSettingWindow::onTableItemDoubleClicked);
    
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
        Major major;
        major.selectById(currentClass->majorId);
        QTableWidgetItem*idItem=new QTableWidgetItem(QString::number(currentClass->id));
        QTableWidgetItem *majorNameItem = new QTableWidgetItem(QString::fromStdString(major.majorName));
        QTableWidgetItem *classNameItem = new QTableWidgetItem(QString::fromStdString(currentClass->className));
        
        // 设置ID列为不可编辑
        idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
        // 设置专业名称列为不可编辑（专业名称应该通过下拉框选择）
        majorNameItem->setFlags(majorNameItem->flags() & ~Qt::ItemIsEditable);
        
        ui->tbClass->setItem(row, 0, idItem);   // 第0列放ID
        ui->tbClass->setItem(row, 1, majorNameItem); // 第1列放专业名称
        ui->tbClass->setItem(row, 2, classNameItem); // 第2列放班级名称
    }
    for(DataObject*dobj:dataObjects){
        delete dobj;
    }
     ui->tbClass->resizeColumnsToContents();
}

void ClassSettingWindow::onTableItemDoubleClicked(int row, int column) {
    // 获取当前行的班级ID
    QTableWidgetItem* idItem = ui->tbClass->item(row, 0);
    if (!idItem) return;
    
    int classId = idItem->text().toInt();
    
    // 获取当前班级信息
    Class classToEdit;
    if (!classToEdit.selectById(classId)) {
        QMessageBox::warning(this, "警告", "无法获取班级信息！");
        return;
    }
    
    bool ok;
    QString newValue;
    
    // 根据列不同，显示不同的编辑对话框
    switch (column) {
        case 0: // ID列 - 不允许编辑
            QMessageBox::information(this, "提示", "班级ID不可编辑");
            break;
            
        case 1: // 专业列 - 使用下拉框选择
        {
            // 获取所有专业
            Major majorFetcher;
            std::vector<DataObject*> majors = majorFetcher.selectAll();
            
            if (majors.empty()) {
                QMessageBox::warning(this, "警告", "没有可用的专业！");
                return;
            }
            
            // 创建对话框
            QDialog dialog(this);
            dialog.setWindowTitle("选择专业");
            
            // 创建布局
            QVBoxLayout* layout = new QVBoxLayout(&dialog);
            
            // 创建下拉框
            QComboBox* comboBox = new QComboBox(&dialog);
            int currentIndex = 0;
            int index = 0;
            
            // 填充下拉框
            for (DataObject* obj : majors) {
                Major* major = static_cast<Major*>(obj);
                if (major) {
                    comboBox->addItem(QString::fromStdString(major->majorName), QVariant(major->id));
                    if (major->id == classToEdit.majorId) {
                        currentIndex = index;
                    }
                    index++;
                }
            }
            
            // 设置当前选中项
            comboBox->setCurrentIndex(currentIndex);
            
            // 添加到布局
            layout->addWidget(comboBox);
            
            // 添加按钮
            QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
            connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
            connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
            layout->addWidget(buttonBox);
            
            // 显示对话框
            if (dialog.exec() == QDialog::Accepted) {
                int newMajorId = comboBox->currentData().toInt();
                
                // 如果专业发生变化，则更新数据库
                if (newMajorId != classToEdit.majorId) {
                    classToEdit.majorId = newMajorId;
                    
                    if (classToEdit.updateData()) {
                        // 更新表格显示
                        Major newMajor;
                        newMajor.selectById(newMajorId);
                        ui->tbClass->item(row, 1)->setText(QString::fromStdString(newMajor.majorName));
                        QMessageBox::information(this, "成功", "专业更新成功！");
                    } else {
                        QMessageBox::critical(this, "错误", "专业更新失败！");
                    }
                }
            }
            
            // 释放资源
            for (DataObject* obj : majors) {
                delete obj;
            }
            break;
        }
            
        case 2: // 班级名称列 - 使用输入对话框
            newValue = QInputDialog::getText(this, "编辑班级名称", 
                                           "请输入新的班级名称:", 
                                           QLineEdit::Normal,
                                           ui->tbClass->item(row, column)->text(), &ok);
            
            if (ok && !newValue.isEmpty()) {
                // 检查名称是否已被使用
                if (classToEdit.isClassNameTakenInMajor(newValue.toStdString(), classToEdit.majorId, classId)) {
                    QMessageBox::warning(this, "警告", "班级名称 \"" + newValue + "\" 在该专业下已被使用，请输入不同的名称。");
                    return;
                }
                
                // 更新数据库
                classToEdit.className = newValue.toStdString();
                if (classToEdit.updateData()) {
                    // 更新表格显示
                    ui->tbClass->item(row, column)->setText(newValue);
                    QMessageBox::information(this, "成功", "班级名称更新成功！");
                } else {
                    QMessageBox::critical(this, "错误", "班级名称更新失败！");
                }
            }
            break;
    }
}

