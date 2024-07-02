//订单类

#ifndef ORDER_H
#define ORDER_H

#include <vector>
#include <QString>
#include <QDateTime> //记录下单时间
#include "food.h"
#include "user.h"

using namespace std;

class Order
{
public:
    //构造函数
    Order(int id, const User &user, const QString &merchant);
    //加购菜品，获取已点菜品，计算总价
    void addFood(const Food &food);
    vector<Food> getFoods() const;
    double getTotalPrice() const;
    //获取ID
    int getId() const;
    //处理为字符串和从字符串反处理为对象
    QString serialize() const;
    static Order deserialize(const QString &data);
    //配送状态的获取与设置
    bool isDelivered() const;
    void setDelivered(bool delivered);
    //下单用户，接单商家以及下单时间的获取
    QString getUser() const;
    QDateTime getOrderTime() const;
    QString getMerchant() const;


    private:
    //订单的数据条目
    int id;
    User user;
    QDateTime orderTime;
    QString merchant;
    vector<Food> foods;
    double totalPrice;
    bool delivered;
};

#endif // ORDER_H
