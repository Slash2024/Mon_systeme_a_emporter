#include "usercheck.h"
#include "ui_usercheck.h"
#include "userdashboard.h"

usercheck::usercheck(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::usercheck)
{
    ui->setupUi(this);
}

usercheck::~usercheck()
{
    delete ui;
}

//展示用户名称并显示问候语
void usercheck::updateProfileDisplay()
{
    ui->profileTextEdit->setText("Enjoy your meal, " + QString::fromStdString(CurrentUser::getInstance().getUser().getUsername()) + "!");
}

//更新订单列表
void usercheck::updateOrderList()
{
    ui->orderListWidget->clear();
    for (const auto &order : Database::getInstance().getOrders()) {
    //筛选出下单人是当前用户的订单
    if (order.getUser().toStdString() == CurrentUser::getInstance().getUser().getUsername())
    {
        QString orderInfo = "下单人：" + order.getUser() + " 点单商家：" + order.getMerchant() + " - 订单金额：¥ " + QString::number(order.getTotalPrice());
        if (order.isDelivered()) {
            orderInfo += "（配送中）";
        } else {
            orderInfo += "（已配送）";
        }
        ui->orderListWidget->addItem(orderInfo);
    }
    }
}

//更新订单详情
void usercheck::updateDetailsDisplay()
{
    ui->orderDetailsListWidget->clear();
    int currentRow = ui->orderListWidget->currentRow();
    const auto& orders = Database::getInstance().getOrders();
    //获取当前登录的用户名
    const auto& currentUser = CurrentUser::getInstance().getUser().getUsername();
    //创建临时vector容器，按顺序存放总订单容器中，下单人为当前用户的订单
    vector<Order> userOrders;
    for (const auto& order : orders) {
        if (order.getUser().toStdString() == currentUser) {
            userOrders.push_back(order);
        }
    }
    //根据列表选中的行数，显示订单详情
    if (currentRow >= 0 && currentRow < userOrders.size()) {
        Order selectedOrder = userOrders.at(currentRow);
        for (const auto &food : selectedOrder.getFoods()) {
            ui->orderDetailsListWidget->addItem(QString::fromStdString(food.getName()) + " 小计 ¥ " + QString::number(food.getPrice()));
        }
    }
}

//刷新按钮
void usercheck::on_reloadButton_clicked()
{
    updateOrderList();
    updateProfileDisplay();
}

//返回页面跳转
void usercheck::on_backButton_clicked()
{
    this -> close();
    userdashboard *ud = new userdashboard;
    ud -> show();
}

//查看订单详情操作
void usercheck::on_detailsButton_clicked()
{
    updateDetailsDisplay();
}

//取消订单操作
void usercheck::on_cancelOrderButton_clicked()
{
    int currentRow = ui->orderListWidget->currentRow();
    if (currentRow >= 0 && currentRow < Database::getInstance().getOrders().size()) {
        Database::getInstance().removeOrder(currentRow);
        updateOrderList();
        QMessageBox::information(this, "我的订单", "退订成功！");
    } else {
        QMessageBox::warning(this, "我的订单", "请选择要操作的订单！");
    }
}

//订单评分操作
void usercheck::on_rateOrderButton_clicked()
{
    //获取要评分的订单在第几行
    int currentRow = ui->orderListWidget->currentRow();
    if (currentRow >= 0 && currentRow < Database::getInstance().getOrders().size()) {
        //在数字选择框中获取当前评分
        int rating = ui->ratingSpinBox->value();
        //Database::getInstance().getOrders().at(currentRow).setRating(rating);
        //消息框提示
        QMessageBox::information(this, "我的订单", "您已评分！");
    } else {
        QMessageBox::warning(this, "我的订单", "请选择要评分的订单！");
    }
}
