#include "userdashboard.h"
#include "ui_userdashboard.h"
#include "usercheck.h"
#include "login.h"

userdashboard::userdashboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::userdashboard),
    orderTimer(new QTimer(this)), //计时操作模拟配送
    currentOrder(4, User(), "") //显式调用构造函数
{
    ui->setupUi(this);
    //信号与槽机制
    //时间到后调用消息窗体
    connect(orderTimer, &QTimer::timeout, this, &userdashboard::checkOrderStatus);
    //更换商家后调用更新菜品列表
    connect(ui->merchantComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &userdashboard::on_merchantComboBox_currentIndexChanged);
    //对商家选择框进行初始化（读取merchant vector中的商家并逐一显示）
    updateMerchantComboBox();
}

userdashboard::~userdashboard()
{
    delete ui;
}

//更新商家选择框
void userdashboard::updateMerchantComboBox()
{
    //先清楚框中的选项
    ui->merchantComboBox->clear();
    //获取merchants容器的只读备份
    const auto& merchants = Database::getInstance().getMerchants();
    //遍历容器中的每一个元素
    for (const auto& merchant : merchants) {
        //把商家名称添加到选择框
        ui->merchantComboBox->addItem(merchant);
    }
}

//接受商家选定信号，更新菜单列表并记录
void userdashboard::on_merchantComboBox_currentIndexChanged(int index)
{
    //更新菜品显示
    updateFoodDisplay();
    //重置当前订单，把商家成员设定为当前选定的成员
    currentOrder = Order(4, CurrentUser::getInstance().getUser(), ui->merchantComboBox->currentText());
}

//浏览食品操作
void userdashboard::on_browseFoodButton_clicked()
{
    updateFoodDisplay();
}

//更新菜品显示
void userdashboard::updateFoodDisplay()
{
    ui->foodListWidget->clear();
    //在商家选择框中获取当前选定的商家名称
    QString selectedMerchant = ui->merchantComboBox->currentText();
    //根据给定的商家名称（key），在菜单册map容器中寻找相应的菜品（value），详细操作在database.cpp中实现
    for (const auto &food : Database::getInstance().getFoods(selectedMerchant)) {
        ui->foodListWidget->addItem(QString::fromStdString(food.getName()) + " 售价 ¥ " + QString::number(food.getPrice()));
    }
}

//选购菜品操作
void userdashboard::on_addButton_clicked()
{
    //获取选定的食品在第几行
    int currentRow = ui->foodListWidget->currentRow();
    //获取商家名称
    QString selectedMerchant = ui->merchantComboBox->currentText();
    //选择范围合法排查
    if (currentRow >= 0 && currentRow < Database::getInstance().getFoods(selectedMerchant).size()) {
        //把选择的菜品拷贝到selectedFood临时对象中
        Food selectedFood = Database::getInstance().getFoods(selectedMerchant).at(currentRow);
        //获取当前选购的数量
        int quantity = ui->quantitySpinBox->value();
        //按数量把菜品添加到currentOrder对象的菜品中（通过接口函数）
        for (int i = 0; i < quantity; ++i) {
            currentOrder.addFood(selectedFood);
        }
        //更新购物车显示
        updateOrderDisplay();
        //更新订单总价
        updateTotalAmount();
    } else {
        //报错信息
        QMessageBox::warning(this, "未选菜品", "请先选择菜品再加购！");
    }
}

//确认订单操作
void userdashboard::on_confirmOrderButton_clicked()
{
    //非空排除，如果为空结束函数
    if (currentOrder.getFoods().empty()) {
        QMessageBox::warning(this, "Order", "Your order is empty.");
        return;
    }
    //把currentorder，通过database的接口添加到order容器末尾
    Database::getInstance().addOrder(currentOrder);
    //消息框提示
    QMessageBox::information(this, "订单", "订单提交成功！请耐心等待配送~");
    //模拟订单配送时间
    int deliveryTime = rand() % 31 + 30; //生成30到60之间的随机数
    orderTimer->start(deliveryTime * 1000); //将随机数转换为毫秒并开始计时
}

//更新购物车显示
void userdashboard::updateOrderDisplay()
{
    ui->currentOrderListWidget->clear();
    for (const auto &food : currentOrder.getFoods()) {
        ui->currentOrderListWidget->addItem(QString::fromStdString(food.getName()) + " 小计 ¥ " + QString::number(food.getPrice()));
    }
}

//更新总价显示
void userdashboard::updateTotalAmount()
{
    //初始化总价为0元
    double totalAmount = 0;
    //遍历当前订单的菜品，并累计价格
    for (const auto &food : currentOrder.getFoods()) {
        totalAmount += food.getPrice();
    }
    //把总价显示在标签组件中
    ui->totalAmountLabel->setText("共计 ¥ " + QString::number(totalAmount));
}

//订单状态更新
void userdashboard::checkOrderStatus()
{
    //计时时间到
    orderTimer->stop();
    //弹出对话框
    QMessageBox::information(this, "订单", "您的订单已配送到，请查收！");
}

//跳转到管理订单界面
void userdashboard::on_manageOrderButton_clicked()
{
    this->close();
    usercheck *uc = new usercheck;
    uc->show();
}

//登出，跳转回起始界面
void userdashboard::on_logOutButton_clicked()
{
    this->close();
    login *l = new login;
    l->show();
}
