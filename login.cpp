#include "login.h"
#include "ui_login.h"
#include "signup.h"
#include <QMessageBox>
#include "userdashboard.h"
#include "adminlogin.h"

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}

//跳转到注册界面
void login::on_signUpButton_clicked()
{
    this->close();
    signup *s = new signup;
    s->show();
}

//登录处理
void login::on_logInBotton_clicked()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    User user;
    //用户的登录调用数据库的login函数接口，在vector容器中遍历比对
    if(Database::getInstance().loginUser(username, password, user))
    {
        //设定“当前用户”，以便于后续记录操作人信息
        CurrentUser::getInstance().setUser(user);
        QMessageBox::information(this, "欢迎", "登入成功！");
        this -> close();
        userdashboard *us = new userdashboard;
        us -> show();
    }
    else
    {
        QMessageBox::warning(this, "警告", "登入失败！");
    }
}

//跳转到管理员登录界面
void login::on_administratorButton_clicked()
{
    this->close();
    adminlogin *al = new adminlogin;
    al->show();
}
