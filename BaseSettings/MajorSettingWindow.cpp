#include "MajorSettingWindow.h"
#include "ui_MajorSettingWindow.h"
#include "DataObjects/major.h"
#include"DataObjects/dataobject.h"
#include <QSqlQuery>
#include <QtSql>
#include<vector>

MajorSettingWindow::MajorSettingWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MajorSettingWindow), parentPointer(parent)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(this->ui->btnAdd, &QPushButton::clicked, this, &MajorSettingWindow::onAddBtnClicked);
    QObject::connect(this->ui->btnDel, &QPushButton::clicked, this, &MajorSettingWindow::onDeleteBtnClicked);
    QObject::connect(this->ui->btnEdit, &QPushButton::clicked, this, &MajorSettingWindow::onUpdateBtnClicked);
    QObject::connect(this->ui->btnExit,&QPushButton::clicked,this,&MajorSettingWindow::onExitBtnClicked);
    loadMajorsToTable();
}

MajorSettingWindow::~MajorSettingWindow()
{
    delete ui;
    parentPointer->setEnabled(true);
}

void MajorSettingWindow::onAddBtnClicked() {
    QString majorName = ui->editName->text().trimmed();
    if (majorName.isEmpty()) {
        QMessageBox::warning(this, "警告", "专业名称不能为空");
        return;
    }

    Major major;
    major.majorName = majorName.toStdString(); // 先设置名称

    if (!major.isNameExists(this)) {  // 检查是否存在
        if (major.insert()) {
            QMessageBox::information(this, "信息", "专业添加成功");
            ui->editName->clear();
            loadMajorsToTable();
        } else {
            QMessageBox::critical(this, "错误", "专业添加失败！");
        }
    }
    else{
        ui->editName->clear();
    }
}
void MajorSettingWindow::onUpdateBtnClicked() {
    QString newMajorNameQString = ui->editName->text().trimmed();
    QString idStr = ui->editID->text().trimmed();

    if (newMajorNameQString.isEmpty() || idStr.isEmpty()) {
        QMessageBox::warning(this, "警告", "ID和专业名称不能为空");
        return;
    }

    bool ok;
    int id = idStr.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "警告", "无效的ID格式");
        return;
    }

    Major majorToUpdate; // 用于操作的Major对象
    // 1. 获取当前正在编辑的专业的信息（包括其原始名称和ID）
    if (!majorToUpdate.selectById(id)) {
        QMessageBox::warning(this, "警告", "指定ID (" + idStr + ") 的专业不存在。");
        return;
    }

    std::string newMajorNameStd = newMajorNameQString.toStdString();

    // 2. 检查专业名称是否真的被修改了
    if (majorToUpdate.majorName != newMajorNameStd) {
        // 3. 如果名称被修改了，才需要检查新名称是否与数据库中其他专业名称冲突
        //    调用 isNameTakenByOther，传入新名称和当前专业的ID
        if (majorToUpdate.isNameTakenByOther(newMajorNameStd, majorToUpdate.id, this)) {
            QMessageBox::warning(this, "警告", "专业名称 \"" + newMajorNameQString + "\" 已被其他专业使用，请输入不同的名称。");
            this->ui->editName->clear();
            return; // 新名称冲突，中断更新
        }
    }
    // 如果代码执行到这里，意味着：
    // a) 专业名称没有被修改，或者
    // b) 专业名称被修改了，但新名称是唯一的（未被其他专业占用）。

    // 4. 执行更新操作
    majorToUpdate.majorName = newMajorNameStd; // 将对象中的名称设置为新的（或未改变的）名称
    if (majorToUpdate.updateData()) {
        QMessageBox::information(this, "成功", "专业信息更新成功！");
        ui->editName->clear();
        ui->editID->clear();
        loadMajorsToTable();
    } else {
        QMessageBox::critical(this, "错误", "专业信息更新失败。");
    }
}
void MajorSettingWindow::onDeleteBtnClicked() {
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
    Major major;
    major.id=id;
    if(major.deleteData()){
        QMessageBox::information(this, "成功", "删除成功");
        ui->editID->clear();
        loadMajorsToTable();
    }
    else{
         QMessageBox::critical(this, "错误", "删除失败");
    }
}
void MajorSettingWindow::onExitBtnClicked(){
    parentPointer->setEnabled(true);
    this->close();
}
void  MajorSettingWindow::loadMajorsToTable(){
    Major majorFetcher;
    std::vector<DataObject*>dataObjects=majorFetcher.selectAll();
    ui->tbSubject->setColumnCount(2);
    ui->tbSubject->setRowCount(0);
    ui->tbSubject->setRowCount(dataObjects.size());
    for(size_t row=0;row<dataObjects.size();++row){
        Major* currentMajor=static_cast<Major*>(dataObjects[row]);
        if(!currentMajor){
            continue;
        }
        QTableWidgetItem*idItem=new QTableWidgetItem(QString::number(currentMajor->id));
        QTableWidgetItem *nameItem = new QTableWidgetItem(QString::fromStdString(currentMajor->majorName));
        ui->tbSubject->setItem(row, 0, idItem);   // 第0列放ID
        ui->tbSubject->setItem(row, 1, nameItem); // 第1列放专业名称
    }
    for(DataObject*dobj:dataObjects){
        delete dobj;
    }
     ui->tbSubject->resizeColumnsToContents();
}
