#include "SubjectSettingWindow.h"
#include "ui_SubjectSettingWindow.h"

SubjectSettingWindow::SubjectSettingWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SubjectSettingWindow), parentPointer(parent)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(this->ui->btnAdd, &QPushButton::clicked, this, &SubjectSettingWindow::onAddBtnClicked);
    QObject::connect(this->ui->btnDel, &QPushButton::clicked, this, &SubjectSettingWindow::onDeleteBtnClicked);
    QObject::connect(this->ui->btnEdit, &QPushButton::clicked, this, &SubjectSettingWindow::onUpdateBtnClicked);
    QObject::connect(this->ui->btnExit, &QPushButton::clicked, this, &SubjectSettingWindow::onExitBtnClicked);
    loadMajorsToComboBox();
    loadSubjectsToTable();
}

SubjectSettingWindow::~SubjectSettingWindow()
{
    delete ui;
    parentPointer->setEnabled(true);
}

void SubjectSettingWindow::onAddBtnClicked() {
    QString subjectName = ui->editName->text().trimmed();
    int majorId = ui->subBox->currentData().toInt();
    if (subjectName.isEmpty()) {
        QMessageBox::warning(this, "警告", "学科名称不能为空！");
        return;
    }
    if (majorId <= 0) {
        QMessageBox::warning(this, "警告", "请选择一个有效的专业！");
        ui->subBox->clear();
        return;
    }
    Subject newsubject;
    newsubject.subName=subjectName.toStdString();
    newsubject.majorId=majorId;
    if(newsubject.isSubjectNameTakenInMajor(subjectName.toStdString(),majorId)){
        QMessageBox::warning(this,"警告","该专业下已存在同名学科！");
        ui->editName->clear();
        return;
    }
    if(newsubject.isSubjectNameTakenInMajor(subjectName.toStdString(),majorId)){
        QMessageBox::warning(this,"警告","该专业下已存在同名学科！");
        ui->editName->clear();
        return;
    }


    if(newsubject.insert()){
        QMessageBox::information(this, "成功", "学科添加成功！");
        ui->editName->clear();
        ui->subBox->setCurrentIndex(0);
        loadSubjectsToTable();
    }else {
        QMessageBox::critical(this, "错误", "学科添加失败！" );
    }
}
void SubjectSettingWindow::onUpdateBtnClicked() {
    QString newSubNameQString=ui->editName->text().trimmed();
    QString idstr=ui->editID->text().trimmed();
    int majorId = ui->subBox->currentData().toInt();
    if (majorId <= 0) {
        QMessageBox::warning(this, "警告", "请选择一个有效的专业！");
        ui->subBox->setFocus();
        return;
    }
    if(newSubNameQString.isEmpty()||idstr.isEmpty()){
        QMessageBox::warning(this, "警告", "ID和学科名称不能为空");
        return;
    }
    bool ok;
    int id=idstr.toInt(&ok);
    if(!ok){
        QMessageBox::warning(this, "警告", "无效的ID格式");
        return;
    }
    Subject subToUpdate;
    if(!subToUpdate.selectById(id)){
        QMessageBox::warning(this, "警告", "指定ID (" + idstr + ") 的学科不存在。");
        return;
    }
    std::string newSubNameStd = newSubNameQString.toStdString();

    if (subToUpdate.subName != newSubNameStd) {

        if(subToUpdate.isSubjectNameTakenInMajor(newSubNameStd, majorId, id)) {
            QMessageBox::warning(this, "警告", "学科名称 \"" + newSubNameQString + "\" 在该专业下已被使用，请输入不同的名称。");
            this->ui->editName->clear();
            return; // 新名称冲突，中断更新
        }
    }
    subToUpdate.subName=newSubNameStd;
    subToUpdate.majorId=majorId;
    if(subToUpdate.updateData()){
        QMessageBox::information(this, "成功", "学科信息更新成功！");
        ui->editName->clear();
        ui->editID->clear();
        loadSubjectsToTable();
    }     else {
        QMessageBox::critical(this, "错误", "学科信息更新失败。");
    }
}
void SubjectSettingWindow::onDeleteBtnClicked() {
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
    Subject subFetcher;
    subFetcher.id=id;
    if(subFetcher.deleteData()){
        QMessageBox::information(this, "成功", "删除成功");
        ui->editID->clear();
        loadSubjectsToTable();
    }
    else{
        QMessageBox::critical(this, "错误", "删除失败");
    }
}
void SubjectSettingWindow::onExitBtnClicked(){
    parentPointer->setEnabled(true);
    this->close();
}
void SubjectSettingWindow::loadMajorsToComboBox(){
    ui->subBox->clear();
    Major majorFetcher;
    std::vector<DataObject*>dataObjects=majorFetcher.selectAll();
    if(dataObjects.empty()){
        ui->subBox->addItem("无可用专业");
        ui->subBox->setEnabled(false);
    }else{
        ui->subBox->setEnabled(true);
        ui->subBox->addItem("请选择专业...",QVariant(-1));
        for(DataObject* dobj:dataObjects){
            Major* currentMajor=static_cast<Major*>(dobj);
            if(currentMajor){
                ui->subBox->addItem(QString::fromStdString(currentMajor->majorName),QVariant(currentMajor->id));
            }
        }
    }
    ui->subBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    for (DataObject* dobj : dataObjects) {
        delete dobj; // 删除每个 Major 对象
    }
}
void SubjectSettingWindow::loadSubjectsToTable(){
    Subject subject;
    std::vector<DataObject*>dataObjects=subject.selectAll();
    ui->tbSubject->setColumnCount(3);
    ui->tbSubject->setRowCount(0);
    ui->tbSubject->setRowCount(dataObjects.size());
    for(size_t row=0;row<dataObjects.size();++row){
        Subject* currentSubject=static_cast<Subject*>(dataObjects[row]);
        if(!currentSubject){
            continue;
        }
        QTableWidgetItem*idItem=new QTableWidgetItem(QString::number(currentSubject->id));
        QTableWidgetItem *majorIdItem = new QTableWidgetItem(QString::number(currentSubject->majorId));
        QTableWidgetItem *subNameItem = new QTableWidgetItem(QString::fromStdString(currentSubject->subName));
        ui->tbSubject->setItem(row, 0, idItem);   // 第0列放ID
        ui->tbSubject->setItem(row, 1, majorIdItem); // 第1列放学科Id
        ui->tbSubject->setItem(row, 2, subNameItem); // 第2列放学科名称
    }
    for(DataObject*dobj:dataObjects){
        delete dobj;
    }
    ui->tbSubject->resizeColumnsToContents();
}
