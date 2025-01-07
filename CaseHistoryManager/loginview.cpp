#include "loginview.h"
#include "ui_loginview.h"
#include "idatabase.h"
#include <QMessageBox>

LoginView::LoginView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginView)
{
    ui->setupUi(this);
}

LoginView::~LoginView()
{
    delete ui;
}

void LoginView::on_btnLogin_clicked()
{
    QString username = ui->inputUsername->text();
    QString password = ui->inputPassword->text();
    QString role = ui->comboRole->currentText();

    QString status;
    if (role == "医生") {
        status = IDatabase::getInstance().doctorLogin(username, password);
    } else {
        status = IDatabase::getInstance().userLogin(username, password);
    }

    if (status == "loginOK") {
        emit loginSuccess(role);  // 传递登录身份
    } else {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误");
    }
}

