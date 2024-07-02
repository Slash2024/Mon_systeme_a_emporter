#include "currentuser.h"

CurrentUser::CurrentUser()
{
}

CurrentUser::~CurrentUser()
{
}

//提供当前的Currentuser，后续调用Currentuser的成员函数*必须*通过这个函数
CurrentUser& CurrentUser::getInstance()
{
    static CurrentUser instance;
    return instance;
}

//设置
void CurrentUser::setUser(const User &user)
{
    this->user = user; //当前这个currentuser的user被传入的user赋值
}

//获取（const说明，只读）
User CurrentUser::getUser() const
{
    return user;
}
