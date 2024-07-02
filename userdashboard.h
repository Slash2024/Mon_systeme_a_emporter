//顾客订餐台界面

#ifndef USERDASHBOARD_H
#define USERDASHBOARD_H

#include <QWidget>
#include <QTimer> //用于计时，模拟配送
#include <QMessageBox>
#include <cstdlib> //用于计时，模拟配送
#include <vector>
#include "user.h"
#include "food.h"
#include "order.h"
#include "database.h"
#include "currentuser.h"

namespace Ui {
class userdashboard;
}

class userdashboard : public QWidget
{
    Q_OBJECT

public:
    explicit userdashboard(QWidget *parent = 0);
    ~userdashboard();

private slots:
    //菜品浏览
    void on_merchantComboBox_currentIndexChanged(int index);
    void on_browseFoodButton_clicked();
    //选购与确认订单
    void on_addButton_clicked();
    void on_confirmOrderButton_clicked();
    //页面跳转
    void on_manageOrderButton_clicked();
    void on_logOutButton_clicked();

private:
    Ui::userdashboard *ui;
    //成员数据：当前，页面正处理的订单。下单就是把currentorder传递给数据库的addOrder函数，添加到vector容器之中。
    Order currentOrder;
    //计时的指针
    QTimer *orderTimer;
    //页面刷新与显示
    void updateFoodDisplay();
    void updateOrderDisplay();
    void updateProfileDisplay();
    void updateOrderList();
    void updateTotalAmount();
    //时间到后自动刷新
    void updateMerchantComboBox();
    void checkOrderStatus();
};

#endif // USERDASHBOARD_H
