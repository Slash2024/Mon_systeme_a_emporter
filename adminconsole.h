//管理员控制台的头文件

#ifndef ADMINCONSOLE_H
#define ADMINCONSOLE_H

//需要用到的库和数据类
#include <QWidget>
#include <QString>
#include <QMessageBox>
#include "database.h"
#include "user.h"

namespace Ui {
class adminconsole;
}

class adminconsole : public QWidget
{
    Q_OBJECT //启用设计师界面的宏

public:
    explicit adminconsole(QWidget *parent = 0);
    ~adminconsole();

private slots: //槽函数（事件处理函数，按下按钮后执行）
    //商家与食品管理区
    void on_merchantListWidget_currentIndexChanged(int index);
    void on_addMerchantButton_clicked();
    void on_addFoodButton_clicked();
    //用户管理区
    void on_userLogOutButton_clicked();
    void on_userResetPassword_clicked();
    //订单管理区
    void on_detailsButton_clicked();
    void on_cancelOrderButton_clicked();
    //页面跳转与刷新
    void on_reloadButton_clicked();
    void on_backButton_clicked();

private:
    //链接UI界面
    Ui::adminconsole *ui;
    //界面刷新/显示函数（普通成员函数）
    void updateUserList();
    void updateOrderList();
    void updateDetailsDisplay();
    void updateMerchantList();
    void updateFoodDisplay();
    void generateOrderStatistics();
};

#endif // ADMINCONSOLE_H
