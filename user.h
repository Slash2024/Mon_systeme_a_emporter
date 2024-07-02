//（普通）用户类

#ifndef USER_H
#define USER_H

#include <string>
#include <Qstring>

using namespace std;

class User
{
public:
    User(const string &username = "", const string &password = "");
    string getUsername() const;
    string getPassword() const;
    //提供修改密码的接口
    void resetpasswd(QString newpassword);

private:
    string username;
    string password;
};

#endif // USER_H
