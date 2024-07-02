#include "database.h"
#include <fstream> //文件读写所需要的头文件
#include <QDir> //操作目录，文件读写的头文件
#include <QFile>
#include <QTextStream>

Database::Database()
{
    //从文件中读取数据
    loadUsers();
    loadFoods();
    loadOrders();
    loadMerchants();
}

Database::~Database()
{
    //保存数据到文件
    saveUsers();
    saveFoods();
    saveOrders();
    saveMerchants();
}

//静态局部变量模式 Meyers Singleton模式（单例模式的经典实现方式）
Database& Database::getInstance()
{
    static Database instance; //局部变量的静态，第一次调用时创建，生命周期持续到程序结束。只会调用一次
    return instance; //返回同一个对象的引用
}

//注册用户
bool Database::registerUser(const QString &username, const QString &password)
{
    //检查用户名是否重复
    for (const auto &user : users) {
        if (user.getUsername() == username.toStdString()) {
            //toStdString是把QString类型的字符串转化为标准类型的
            return false;
        }
    }
    //检查用户名密码是否为空
    if (username.isEmpty() || password.isEmpty()) {
        return false;
    }
    //经过检查后，把用户对象插入vector容器
    users.push_back(User(username.toStdString(), password.toStdString()));
    //保存Users数据
    saveUsers();
    return true;
}

//用户登录校验
bool Database::loginUser(const QString &username, const QString &password, User &user)
{
    //遍历用户
    for (const auto &u : users) {
        //登录条件，用户名和密码与在册数据一致
        if (u.getUsername() == username.toStdString() && u.getPassword() == password.toStdString()) {
            user = u;
            return true;
        }
    }
    return false;
}

//添加菜品
void Database::addFood(const QString &merchant, const Food &food)
{
    merchantFoods[merchant].push_back(food); //map版本的也是用push_back函数，用法稍有区别
    saveFoods();
}

//查阅菜单
vector<Food> Database::getFoods(const QString &merchant) const
{
    //根据给定的key-商家，查找对应的value-菜单vector
    auto it = merchantFoods.find(merchant);
    if (it != merchantFoods.end()) { //排除没找到的情况
        return it->second; //second位置存储的value就是菜单
    }
    return {};
}

//添加订单
void Database::addOrder(const Order &order)
{
    orders.push_back(order);
    saveOrders();
}

//查询订单
vector<Order>& Database::getOrders()
{
    return orders;
}

//删除订单
void Database::removeOrder(int index)
{
    if (index >= 0 && index < orders.size()) { //确定要删除的行数在范围内
        orders.erase(orders.begin() + index); //用erase函数移除元素
        saveOrders();
    }
}

//查询用户
vector<User>& Database::getUsers()
{
    return users;
}

//删除用户
void Database::logoutUser(int index)
{
    if (index >= 0 && index < users.size()) {
        users.erase(users.begin() + index);
        saveUsers();
    }
}

//修改用户密码
void Database::resetPassword(int index, QString newpassword)
{
    if (index >= 0 && index < users.size()) {
        users[index].resetpasswd(newpassword); //访问users位置为index的user对象的重置函数
        saveUsers();
    }
}

//添加商家
void Database::addMerchant(const QString &merchant)
{
    merchants.push_back(merchant);
}

//查询商家
vector<QString> Database::getMerchants() const
{
    return merchants;
}

//从文件加载用户信息
void Database::loadUsers()
{
    ifstream file("users.txt"); //输入文件流打开文件
    if (!file.is_open()) return; //文件打开失败的检查
    string username, password; //用于存储读取的用户名密码
    //下面的循环一直持续到没有东西可以读取。while中的条件是在读取数据，然后循环体内的语句创建对象。
    while (file >> username >> password) {
        users.push_back(User(username, password));
    }
    file.close(); //及时关闭文件
}

//把用户信息写入文件
void Database::saveUsers() const
{
    ofstream file("users.txt"); //输出文件流创建or覆盖文件
    //下面遍历当前users容器的所有元素
    for (const auto &user : users) {
        //下面这行操作用于获取并写入用户名与密码，中间进行空格和换行
        file << user.getUsername() << " " << user.getPassword() << endl;
    }
    file.close(); //及时关闭文件
}

//菜品读
void Database::loadFoods()
{
    ifstream file("foods.txt");
    if (!file.is_open()) return;
    string merchant, name;
    double price;
    while (file >> merchant >> name >> price) {
        merchantFoods[QString::fromStdString(merchant)].push_back(Food(name, price));
        //map容器的pushback方法
    }
    file.close();
}

//菜品写
void Database::saveFoods() const
{
    std::ofstream file("foods.txt");
    for (const auto &pair : merchantFoods) { //pair是一个临时变量，代表一个条目 (k+v)
        for (const auto &food : pair.second) {
            file << pair.first.toStdString() << " " << food.getName() << " " << food.getPrice() << std::endl;
        }
    }
    file.close();
}

//订单读
void Database::loadOrders()
{
    QFile file("orders.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&file);
    orders.clear();
    while (!in.atEnd()) {
        QString line = in.readLine();
        orders.push_back(Order::deserialize(line)); //订单的读写比较复杂，专门写了函数
    }
    file.close();
}

//订单写
void Database::saveOrders() const
{
    QFile file("orders.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file);
    for (const auto &order : orders) {
        out << order.serialize() << "\n";
    }
    file.close();
}

//商家读
void Database::loadMerchants()
{
    ifstream file("merchants.txt"); //输入文件流打开文件
    if (!file.is_open()) return; //文件打开失败的检查

    string merchant; //用于存储读取的用户名密码
    //下面的循环一直持续到没有东西可以读取。while中的条件是在读取数据，然后循环体内的语句创建对象。
    while (file >> merchant) {
        merchants.push_back(QString::fromStdString(merchant));
    }
    file.close(); //及时关闭文件
}

//商家写
void Database::saveMerchants() const
{
    ofstream file("merchants.txt"); //输出文件流创建or覆盖文件
    //下面遍历当前users容器的所有元素
    for (const auto &merchant : merchants) {
        //下面这行操作用于获取并写入用户名与密码，中间进行空格和换行
        file << merchant.toStdString() << endl;
    }
    file.close(); //及时关闭文件
}

//查询指定商家名称
QString Database::thisMerchant(int index) const
{
    return merchants[index];
}
