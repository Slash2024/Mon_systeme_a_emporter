#include "adminconsole.h"
#include "ui_adminconsole.h"
#include "login.h"
//图表统计功能所需
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

adminconsole::adminconsole(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::adminconsole)
{
    ui->setupUi(this);
    connect(ui->merchantListWidget, &QListWidget::currentRowChanged, this, &adminconsole::on_merchantListWidget_currentIndexChanged);
    //信号与槽机制，实现即时相应
}

adminconsole::~adminconsole()
{
    delete ui;
}

//刷新用户列表
void adminconsole::updateUserList()
{
    //清屏操作
    ui->userListWidget->clear();
    //依次遍历vector容器中的各用户元素
    for (const auto &user : Database::getInstance().getUsers())
    {
        //将用户的信息转化为字符串
        QString userInfo = "Username: " + QString::fromStdString(user.getUsername()) + " - Password: " + QString::fromStdString(user.getPassword());
        //将信息字符串逐条显示在列表组件中
        ui->userListWidget->addItem(userInfo);
    }
}

//注销用户
void adminconsole::on_userLogOutButton_clicked()
{
    //获取当前列表组件，选定的是第几行
    int currentRow = ui->userListWidget->currentRow();
    //if检查是否选择了，而且选择是否在范围内
    if (currentRow >= 0 && currentRow < Database::getInstance().getUsers().size()) {
        //调用数据库的删除操作（传递参数是要删除第几行（也就是在容器中第几个）的用户
        Database::getInstance().logoutUser(currentRow);
        //删除后及时更新用户列表
        updateUserList();
        //弹出提示框
        QMessageBox::information(this, "用户管理", "用户注销成功！");
    } else {
        //未选择用户，弹出警告
        QMessageBox::warning(this, "用户管理", "请选定一位用户！");
    }
}

//刷新界面函数
void adminconsole::on_reloadButton_clicked()
{
    //调用各刷新函数
    updateUserList();
    updateOrderList();
    updateMerchantList();
    generateOrderStatistics();
}

//重置用户密码
void adminconsole::on_userResetPassword_clicked()
{
    int currentRow = ui->userListWidget->currentRow();
    //在行编辑器中获取要设置的密码（代替输入功能）
    QString newpassword = ui->newPasswordLineEdit->text();
    if (currentRow >= 0 && currentRow < Database::getInstance().getUsers().size()) {
        //调用数据库的修改操作，提供参数为修改哪一位用户以及新密码是是什么
        Database::getInstance().resetPassword(currentRow, newpassword);
        updateUserList();
        QMessageBox::information(this, "用户管理", "密码重置成功！");
    } else {
        QMessageBox::warning(this, "用户管理", "请选定一位用户！");
    }
}

//刷新订单列表
void adminconsole::updateOrderList()
{
    ui->orderListWidget->clear();
    for (const auto &order : Database::getInstance().getOrders()) {
        QString orderInfo = "下单人：" + order.getUser() + " 点单商家：" + order.getMerchant() + " 订单金额：¥" + QString::number(order.getTotalPrice());
        if (order.isDelivered()) {
            orderInfo += "（已配送）";
        } else {
            orderInfo += "（配送中）";
        }
        ui->orderListWidget->addItem(orderInfo);
    }
}

//刷新订单详情
void adminconsole::updateDetailsDisplay()
{
    ui->orderDetailsListWidget->clear();
    int currentRow = ui->orderListWidget->currentRow();
    if (currentRow >= 0 && currentRow < Database::getInstance().getOrders().size()) {
        Order selectedOrder = Database::getInstance().getOrders().at(currentRow);
        for (const auto &food : selectedOrder.getFoods()) {
            ui->orderDetailsListWidget->addItem(QString::fromStdString(food.getName()) + " 小计 ¥" + QString::number(food.getPrice()));
    }
    }
}

//按下按钮时刷新
void adminconsole::on_detailsButton_clicked()
{
    updateDetailsDisplay();
}

//删除订单
void adminconsole::on_cancelOrderButton_clicked()
{
    int currentRow = ui->orderListWidget->currentRow();
    if (currentRow >= 0 && currentRow < Database::getInstance().getOrders().size()) {
        Database::getInstance().removeOrder(currentRow);
        updateOrderList();
        QMessageBox::information(this, "订单管理", "订单取消成功！");
    } else {
        QMessageBox::warning(this, "订单管理", "请先选定订单！");
    }
}

//刷新商家列表
void adminconsole::updateMerchantList()
{
    ui->merchantListWidget->clear();
    for (const auto &merchant : Database::getInstance().getMerchants())
    {
        ui->merchantListWidget->addItem(merchant);
    }
}

//添加商家
void adminconsole::on_addMerchantButton_clicked()
{
    //在行编辑器中获取输入的新商家名称
    QString newmerchant = ui->newMerchantLineEdit->text();
    //非空排除
    if(!newmerchant.isEmpty())
    {
        //调用数据库的添加函数，把这个商家的名字添加到QString的Vector容器中
        Database::getInstance().addMerchant(newmerchant);
        //刷新商家列表
        updateMerchantList();
        //消息框提示
        QMessageBox::information(this, "商家 & 菜品管理", "商家入驻成功！");
    }
    else
    {
        //报错
        QMessageBox::warning(this, "商家 & 菜品管理", "请输入入驻商家的名称！");
    }
}

//刷新菜品列表
void adminconsole::updateFoodDisplay()
{
    ui->foodListWidget->clear();
    int currentRow = ui->merchantListWidget->currentRow();
    if (currentRow >= 0 && currentRow < Database::getInstance().getMerchants().size()) {
        QString selectedMerchant = Database::getInstance().thisMerchant(currentRow);
        for (const auto &food : Database::getInstance().getFoods(selectedMerchant)) {
            ui->foodListWidget->addItem(QString::fromStdString(food.getName()) + " 售价 ¥ " + QString::number(food.getPrice()));
        }
    }
}

//信号与槽机制。接收到选择的商家发生改变时，调用这个刷新菜品列表的函数，更新目前选定商家的菜品有哪些
void adminconsole::on_merchantListWidget_currentIndexChanged(int index)
{
    updateFoodDisplay();
}

//添加菜品
void adminconsole::on_addFoodButton_clicked()
{
    int currentRow = ui->merchantListWidget->currentRow();
    QString newfood = ui->newFoodLineEdit->text();
    double newprice = ui->newPriceLineEdit->text().toDouble(); //格式转换
    if ((currentRow >= 0 && currentRow < Database::getInstance().getMerchants().size()) && (!newfood.isEmpty())) {
        QString selectedMerchant = Database::getInstance().thisMerchant(currentRow);
        //调用Food类的构造函数，生成一个新的Food对象并插入容器末尾
        Database::getInstance().addFood(selectedMerchant, Food(newfood.toStdString(), newprice));
        updateFoodDisplay();
        QMessageBox::information(this, "商家 & 菜品管理", "菜品添加成功！");
    }
    else
    {
        QMessageBox::warning(this, "商家 & 菜品管理", "请输入新增菜品的名称与价格！");
    }
}

//页面跳转函数
void adminconsole::on_backButton_clicked()
{
    this -> close(); //关闭当前页面
    login *l = new login; //建立新页面并返回指针（动态内存）
    l -> show(); //展示新页面
}

//生成统计图表
void adminconsole::generateOrderStatistics()
{
    using namespace QtCharts;
    //商家营业额柱状图
    //创建条形数据集合mincomeseries和mincomeset.
    QBarSeries *mincomeseries = new QBarSeries();
    QBarSet *mincomeset = new QBarSet("收入");
    //制表前的数据准备工作；统计各商家的营业额，形成不同商家营业总额的map数据表
    QMap<QString, double> mincome;
    for (const auto &order : Database::getInstance().getOrders()) {
        //把订单册中每个对应商家名称的收入，累计在map容器中对应商家的value值中
        mincome[order.getMerchant()] += order.getTotalPrice();
    }
    //X轴标签的制作
    QStringList mnames;
    for (auto it = mincome.begin(); it != mincome.end(); ++it) { //迭代器
        //从关联容器中分离收集键值对，分别计入商家收入与商家名称，也就是X轴的标签与对应的数据值
        *mincomeset << it.value();
        mnames << it.key();
    }
    //把set放到series里面。数据处理先进set，然后进series
    mincomeseries->append(mincomeset);
    //创建Chart对象
    QChart *merchantChart = new QChart();
    //把series再放入chart对象里面。set - series - chart
    merchantChart->addSeries(mincomeseries);
    merchantChart->setTitle("商家收入统计");
    //启用数据系列动画效果
    merchantChart->setAnimationOptions(QChart::SeriesAnimations);
    //设置X轴
    QBarCategoryAxis *merchantAxisX = new QBarCategoryAxis(); //创建一个X轴对象
    merchantAxisX->append(mnames); //填充X轴内容
    merchantChart->addAxis(merchantAxisX, Qt::AlignBottom); //把X轴放在图表的底部
    mincomeseries->attachAxis(merchantAxisX); //数据根据X轴布局与展示
    //设置Y轴
    QValueAxis *merchantAxisY = new QValueAxis();
    merchantAxisY->setRange(0, *std::max_element(mincome.begin(), mincome.end())); //设置数据范围
    merchantChart->addAxis(merchantAxisY, Qt::AlignLeft); //把Y轴放在图表的左侧
    mincomeseries->attachAxis(merchantAxisY); //条形高度根据Y轴缩放
    //创建表格显示框
    QChartView *merchantChartView = new QChartView(merchantChart);
    //平滑渲染
    merchantChartView->setRenderHint(QPainter::Antialiasing);
    //显示窗体
    merchantChartView->show();

    // 消费者消费额柱状图
    QBarSeries *ccostseries = new QBarSeries();
    QBarSet *ccostset = new QBarSet("消费");
    QMap<QString, double> ccost;
    for (const auto &order : Database::getInstance().getOrders()) {
        ccost[order.getUser()] += order.getTotalPrice();
    }
    QStringList cnames;
    for (auto it = ccost.begin(); it != ccost.end(); ++it) {
        *ccostset << it.value();
        cnames << it.key();
    }
    ccostseries->append(ccostset);
    QChart *consumerChart = new QChart();
    consumerChart->addSeries(ccostseries);
    consumerChart->setTitle("顾客消费统计");
    consumerChart->setAnimationOptions(QChart::SeriesAnimations);
    QBarCategoryAxis *consumerAxisX = new QBarCategoryAxis();
    consumerAxisX->append(cnames);
    consumerChart->addAxis(consumerAxisX, Qt::AlignBottom);
    ccostseries->attachAxis(consumerAxisX);
    QValueAxis *consumerAxisY = new QValueAxis();
    consumerAxisY->setRange(0, *std::max_element(ccost.begin(), ccost.end()));
    consumerChart->addAxis(consumerAxisY, Qt::AlignLeft);
    ccostseries->attachAxis(consumerAxisY);
    QChartView *consumerChartView = new QChartView(consumerChart);
    consumerChartView->setRenderHint(QPainter::Antialiasing);
    consumerChartView->show();
}
