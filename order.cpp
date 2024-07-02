#include "order.h"

//构造函数
Order::Order(int id, const User &user, const QString &merchant)
    : id(0),
      user(user),
      merchant(merchant),
      orderTime(QDateTime::currentDateTime()),
      totalPrice(0.0),
      delivered(false)
{
}

//加购菜品
void Order::addFood(const Food &food)
{
    foods.push_back(food); //vector容器的push_back方法
    totalPrice += food.getPrice(); //每次加购菜品都计算总价
}

//获取目前已点的菜品
vector<Food> Order::getFoods() const
{
    return foods;
}

//获取总价
double Order::getTotalPrice() const
{
    return totalPrice;
}

//查询是否配送
bool Order::isDelivered() const
{
    return delivered;
}

//设置配送状态
void Order::setDelivered(bool delivered)
{
    this -> delivered = delivered;
}

//获取下单用户
QString Order::getUser() const
{
    return QString::fromStdString(user.getUsername()); //字符串的格式转换
}

//获取下单时间
QDateTime Order::getOrderTime() const
{
    return orderTime;
}

//获取接单商家
QString Order::getMerchant() const
{
    return merchant;
}

//获取ID
int Order::getId() const
{
    return id;
}

//将order对象转化为字符串data
QString Order::serialize() const
{
    QString data = getUser() + "|" + orderTime.toString(Qt::ISODate) + "|" + merchant + "|" + QString::number(totalPrice) + "|" + (delivered ? "1" : "0");
    for (const auto &food : foods) {
        data += "|" + QString::fromStdString(food.getName()) + "," + QString::number(food.getPrice());
    }
    return data;
}

//将字符串data还原为order对象
Order Order::deserialize(const QString &data)
{
    //按规则分割字符串并进行暂存
    QStringList parts = data.split("|");
    //第一个部分（存在parts[0]中），提取文本，转化为String类型，并以此创建一个User对象（密码在这里不重要，设定为空）
    User user(parts[0].toStdString(), "");
    //user和第三部分商家共同构造一个Order类对象
    Order order(4, user, parts[2]);
    //对order的信息进行填充
    order.orderTime = QDateTime::fromString(parts[1], Qt::ISODate);
    order.totalPrice = parts[3].toDouble();
    order.delivered = parts[4] == "1";
    for (int i = 5; i < parts.size(); ++i) {
        QStringList foodParts = parts[i].split(","); //这里的分隔符为逗号，分割食物名称与价格
        order.addFood(Food(foodParts[0].toStdString(), foodParts[1].toDouble())); //根据名称和价格构造Food对象
    }
    return order;
}
