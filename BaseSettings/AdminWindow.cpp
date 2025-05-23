#include "AdminWindow.h"
#include "ui_AdminWindow.h"

AdminWindow::AdminWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminWindow), parent(parent)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    QBUTTON_CONNECT_MYCREATE(this->ui->btnAdd, &AdminWindow::addNewAdmin);
    QBUTTON_CONNECT_MYCREATE(this->ui->btnEdit, &AdminWindow::updateAdmin);
    QBUTTON_CONNECT_MYCREATE(this->ui->btnDel, &AdminWindow::deleteAdmin);
    QObject::connect(this->ui->tbUser, &QTableWidget::doubleClicked, this, &AdminWindow::doubleClickAdminTable);

    this->refreshTable();
}

AdminWindow::~AdminWindow()
{
    delete ui;
    parent->setEnabled(true);
}

void AdminWindow::refreshTable() {
    Admin mapper;
    for(int r=this->ui->tbUser->rowCount()-1; ~r; r--) {
        this->ui->tbUser->removeRow(r);
    }
    std::vector<DataObject*> vec(mapper.selectAll());
    this->ui->tbUser->setRowCount(vec.size());
    for(int i=0; i<vec.size(); i++) {
        Admin* p = (Admin*)vec[i];
        this->ui->tbUser->setItem(i, 0, new QTableWidgetItem(QString("%1").arg(p->id)));
        this->ui->tbUser->setItem(i, 1, new QTableWidgetItem(STDTOQSTR(p->userName)));
        this->ui->tbUser->setItem(i, 2, new QTableWidgetItem(STDTOQSTR(p->userPwd)));
    }
}

void AdminWindow::addNewAdmin() {
    QString userName = this->ui->editName->text();
    QString userPwd = this->ui->editPwd->text();
    if(userName=="" || userPwd=="") {
        WARN_MESSAGE("请输入有效的信息！");
        return;
    }
    Admin mapper;
    mapper.userName = userName.toStdString();
    mapper.userPwd = userPwd.toStdString();
    if(mapper.insert()) {
        SUCCESS_MESSAGE("添加用户成功！");
        this->refreshTable();
    } else {
        ERROR_MESSAGE("添加用户失败！数据库错误");
    }
}

void AdminWindow::updateAdmin() {
    this->doubleClickAdminTable();
}

void AdminWindow::deleteAdmin() {
    if(!QUESTION_CONFIRMED_MESSAGE(QString("是否要删除用户 %1 ？").arg(ui->tbUser->item(ui->tbUser->currentRow(), 1)->text()))) {
        return;
    }
    Admin mapper;
    mapper.id = ui->tbUser->item(ui->tbUser->currentRow(), 0)->text().toInt();
    if(mapper.deleteData()) {
        SUCCESS_MESSAGE("删除成功！");
        this->refreshTable();
    } else {
        ERROR_MESSAGE("删除失败！");
    }
}

void AdminWindow::doubleClickAdminTable() {
    QString keyName, inputTitle, inputLabel;
    switch (ui->tbUser->currentColumn()) {
    case 0:
        WARN_MESSAGE("编号不可修改！");
        return;
    case 1:
        keyName = "userName";
        inputTitle = "修改用户名";
        inputLabel = "请输入修改后的用户名：";
        break;
    case 2:
        keyName = "userPwd";
        inputTitle = "修改密码";
        inputLabel = "请输入修改后的密码：";
        break;
    default:
        keyName = "UNKNOWN";
        inputTitle = "ERROR404";
        inputLabel = "AN EXPRESSION OCCURRED...";
    }
    if(keyName == "UNKNOWN") {
        WARN_MESSAGE("请先选择一个数据！");
        return;
    }
    Admin mapper;
    mapper.id = ui->tbUser->item(ui->tbUser->currentRow(), 0)->text().toInt();
    if(!mapper.selectById(mapper.id)) {
        ERROR_MESSAGE("修改失败！\n可能是数据库文件损坏、出现重名问题。");
        return;
    }
    QString value = QInputDialog::getText(this, inputTitle, inputLabel, QLineEdit::Normal, ui->tbUser->currentItem()->text());
    if (value != "") {
        if(keyName == "userPwd") mapper.userPwd = value.toStdString();
        else if(keyName == "userName") mapper.userName = value.toStdString();
        else {
            ERROR_MESSAGE("修改失败！\n可能是数据库文件损坏、出现重名问题。");
            return;
        }
        if (mapper.updateData()) {
            SUCCESS_MESSAGE("修改成功！");
            this->refreshTable();
        }
        else
            ERROR_MESSAGE("修改失败！\n可能是数据库文件损坏、出现重名问题。");
    }
}
