//单例处理的特殊订单类
//要保证整个程序只有一个*当前用户*，并且不同的界面都能读写数据

#ifndef CURRENTUSER_H
#define CURRENTUSER_H

#include "user.h"

class CurrentUser
{
public:
    static CurrentUser& getInstance(); //提供一个唯一的访问接口
    void setUser(const User &user); //成员函数
    User getUser() const;

private:
    CurrentUser(); //把构造函数和析构函数私有化，避免生成其他的*当前用户*
    ~CurrentUser();
    User user;
};

#endif // CURRENTUSER_H
