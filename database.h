//数据库头文件

#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <unordered_map>
//用到了STL标准库，map及vector
#include <QString>
#include <QHash>
//哈希算法，用map需要添加的，不然会报错
#include "user.h"
#include "food.h"
#include "order.h"

using namespace std;

//哈希算法，用map的相关功能需要添加
namespace std {
    template <>
    struct hash<QString> {
        std::size_t operator()(const QString& k) const {
            return qHash(k);
        }
    };
}

//数据库类，同样为单例存储。整个程序只有1个数据库对象，作为公共数据池，并与文本文件（存储）进行数据交换。
class Database
{
public:
    static Database& getInstance(); //单例模式
    //用户的注册与登录（本质上是添加操作和查询操作）
    bool registerUser(const QString &username, const QString &password);
    bool loginUser(const QString &username, const QString &password, User &user);
    //菜品的添加，查询
    void addFood(const QString &merchant, const Food &food); //升级函数的时候，声明和定义要匹配
    vector<Food> getFoods(const QString &merchant) const;
    //订单的添加，查询，删除
    void addOrder(const Order &order);
    vector<Order>& getOrders(); //返回引用是为了能够修改
    void removeOrder(int index);
    //用户的查询，删除，重置密码
    vector<User>& getUsers();
    void logoutUser(int index);
    void resetPassword(int index, QString newpassword);
    //商家的添加，查询，查询某一商家的名称
    void addMerchant(const QString &merchant);
    vector<QString> getMerchants() const;
    QString thisMerchant(int index) const;

private:
    //单例模式
    Database();
    ~Database();
    //四个容器，存储用户，订单和商家名vector容器。还有商家和菜品连接的map容器。
    vector<User> users;
    unordered_map<QString, vector<Food>> merchantFoods;
    vector<Order> orders;
    vector<QString> merchants;
    //文件读写函数
    void loadUsers();
    void saveUsers() const;
    void loadFoods();
    void saveFoods() const;
    void loadOrders();
    void saveOrders() const;
    void loadMerchants();
    void saveMerchants() const;
};

#endif // DATABASE_H
