#include "adminlogin.h"
#include "ui_adminlogin.h"
#include "adminconsole.h"

adminlogin::adminlogin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::adminlogin)
{
    ui->setupUi(this);
}

adminlogin::~adminlogin()
{
    delete ui;
}

//登录处理
void adminlogin::on_logInBotton_clicked()
{
    //从行编辑器中获取名字与密码
    QString name = ui->nameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    //进行比对（管理员的名字与密码在代码中设定）。注意==只能在标准String字符串中比较，所以要进行格式转换
    if(name.toStdString() == "Matthew" && password.toStdString() == "20240628")
    {
        QMessageBox::information(this, "欢迎", "认证成功！");
        //登录成功后跳转到管理员控制台界面
        this -> close();
        adminconsole *ac = new adminconsole;
        ac-> show();
    }
    else
    {
        QMessageBox::warning(this, "警告", "非法访问！");
    }
}
