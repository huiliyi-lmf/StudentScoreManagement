#include "SubjectSettingWindow.h"
#include "ui_SubjectSettingWindow.h"
#include <QInputDialog>
#include <QComboBox>
#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QMessageBox>
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
    // 连接双击信号到新的槽函数
    QObject::connect(ui->tbSubject, &QTableWidget::cellDoubleClicked, this, &SubjectSettingWindow::onTbSubjectItemDoubleClicked);
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
        QMessageBox::warning(this, "警告", "该课程下存在学生成绩，无法删除");
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
        Major major;
        major.selectById(currentSubject->majorId);
        QTableWidgetItem*idItem=new QTableWidgetItem(QString::number(currentSubject->id));
        QTableWidgetItem *majorNameItem = new QTableWidgetItem(QString::fromStdString(major.majorName));
        QTableWidgetItem *subNameItem = new QTableWidgetItem(QString::fromStdString(currentSubject->subName));
        ui->tbSubject->setItem(row, 0, idItem);   // 第0列放ID
        ui->tbSubject->setItem(row, 1, majorNameItem); // 第1列放专业名称
        ui->tbSubject->setItem(row, 2, subNameItem); // 第2列放学科名称
    }
    for(DataObject*dobj:dataObjects){
        delete dobj;
    }
    ui->tbSubject->resizeColumnsToContents();
}
void SubjectSettingWindow::onTbSubjectItemDoubleClicked(int row, int column) {
    QTableWidgetItem* idItem = ui->tbSubject->item(row, 0); //学科ID在第0列
    if (!idItem) {
        QMessageBox::warning(this, "错误", "无法获取学科ID。");
        return;
    }
    int subjectId = idItem->text().toInt();

    Subject subjectToEdit;
    if (!subjectToEdit.selectById(subjectId)) {
        QMessageBox::warning(this, "错误", "无法加载要编辑的学科信息。");
        return;
    }

    bool ok = false;
    QString newValue;

    if (column == 1) { // 专业列 (假设专业名称在第1列)
        QDialog dialog(this);
        dialog.setWindowTitle("选择专业");
        QVBoxLayout* layout = new QVBoxLayout(&dialog);
        QComboBox* comboBox = new QComboBox(&dialog);

        // 填充专业下拉框
        Major majorFetcher;
        std::vector<DataObject*> majors = majorFetcher.selectAll();
        if (majors.empty()) {
            QMessageBox::warning(this, "警告", "没有可用的专业！");
            for (DataObject* obj : majors) delete obj; // 清理内存
            return;
        }

        int currentMajorIndex = -1;
        for (size_t i = 0; i < majors.size(); ++i) {
            Major* major = static_cast<Major*>(majors[i]);
            if (major) {
                comboBox->addItem(QString::fromStdString(major->majorName), QVariant(major->id));
                if (major->id == subjectToEdit.majorId) {
                    currentMajorIndex = i;
                }
            }
        }
        if (currentMajorIndex != -1) {
            comboBox->setCurrentIndex(currentMajorIndex);
        }

        layout->addWidget(comboBox);
        QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
        connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
        layout->addWidget(buttonBox);

        if (dialog.exec() == QDialog::Accepted) {
            int newMajorId = comboBox->currentData().toInt();
            if (newMajorId != subjectToEdit.majorId) {
                 // 检查新专业下是否已有同名学科
                if (subjectToEdit.isSubjectNameTakenInMajor(subjectToEdit.subName, newMajorId, subjectId)) {
                    QMessageBox::warning(this, "警告", "学科名称 \"" + QString::fromStdString(subjectToEdit.subName) + "\" 在新选择的专业下已被使用。");
                    for (DataObject* obj : majors) delete obj;
                    return;
                }
                subjectToEdit.majorId = newMajorId;
                if (subjectToEdit.updateData()) {
                    loadSubjectsToTable(); // 重新加载表格以更新显示
                    QMessageBox::information(this, "成功", "学科的专业更新成功！");
                } else {
                    QMessageBox::critical(this, "错误", "学科的专业更新失败！");
                }
            }
        }
        for (DataObject* obj : majors) delete obj; // 清理 majors 列表

    } else if (column == 2) { // 学科名称列 (假设学科名称在第2列)
        newValue = QInputDialog::getText(this, "编辑学科名称",
                                       "请输入新的学科名称:", QLineEdit::Normal,
                                       QString::fromStdString(subjectToEdit.subName), &ok);
        if (ok && !newValue.isEmpty()) {
            if (newValue.toStdString() != subjectToEdit.subName) {
                if (subjectToEdit.isSubjectNameTakenInMajor(newValue.toStdString(), subjectToEdit.majorId, subjectId)) {
                    QMessageBox::warning(this, "警告", "学科名称 \"" + newValue + "\" 在该专业下已被使用。");
                    return;
                }
                subjectToEdit.subName = newValue.toStdString();
                if (subjectToEdit.updateData()) {
                    ui->tbSubject->item(row, column)->setText(newValue); // 直接更新单元格
                    QMessageBox::information(this, "成功", "学科名称更新成功！");
                } else {
                    QMessageBox::critical(this, "错误", "学科名称更新失败！");
                }
            }
        } else if (ok && newValue.isEmpty()) {
            QMessageBox::warning(this, "警告", "学科名称不能为空！");
        }
    } else if (column == 0) { // ID 列
        QMessageBox::information(this, "提示", "学科ID不可直接编辑。");
    }
}
