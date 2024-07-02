#include "user.h"

//构造函数
User::User(const string &username, const string &password)
    : username(username), password(password)
{
}

//获取用户名
string User::getUsername() const
{
    return username;
}

//获取密码
string User::getPassword() const
{
    return password;
}

//重置密码
void User::resetpasswd(QString newpassword)
{
    password = newpassword.toStdString();
}
