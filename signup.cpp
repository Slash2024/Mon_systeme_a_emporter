#include "signup.h"
#include "ui_signup.h"
#include <QMessageBox>
#include "login.h"

signup::signup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::signup)
{
    ui->setupUi(this);
}

signup::~signup()
{
    delete ui;
}

//注册用户操作
void signup::on_sureButton_clicked()
{
    //从行编辑器中获取，用户的名称、密码和确认密码
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    QString surepassword = ui->surePasswordLineEdit->text();
    //先进行两次密码是否输入一致的检查
    if(password.toStdString() == surepassword.toStdString())
    {
        //调用数据库中新加用户的操作。所提供的函数自带了用户名重复检查报错。
        if(Database::getInstance().registerUser(username, password))
        {
        //弹出消息框
        QMessageBox::information(this, "祝贺", "注册成功!");
        }
        else
        {
        QMessageBox::warning(this, "啊哦", "出了一点问题~ 请换一个用户名，并确认所有的信息都已填写！");
        }
    }
    else
    {
        QMessageBox::warning(this, "啊哦", "出了一点问题~ 请确认两次密码输入一致！");
    }
}

//返回到登录界面
void signup::on_returnButton_clicked()
{
    this -> close();
    login *l = new login;
    l -> show();
}
