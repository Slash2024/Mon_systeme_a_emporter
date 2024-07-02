//用户结算台界面

#ifndef USERCHECK_H
#define USERCHECK_H

#include <QWidget>
#include <QString>
#include "database.h"
#include "currentuser.h"

namespace Ui {
class usercheck;
}

class usercheck : public QWidget
{
    Q_OBJECT

public:
    explicit usercheck(QWidget *parent = 0);
    ~usercheck();

private slots:
    //刷新与页面跳转
    void on_reloadButton_clicked();
    void on_backButton_clicked();
    //订单详情、取消以及评分
    void on_detailsButton_clicked();
    void on_cancelOrderButton_clicked();
    void on_rateOrderButton_clicked();

private:
    Ui::usercheck *ui;
    void updateProfileDisplay();
    void updateOrderList();
    void updateDetailsDisplay();
};

#endif // USERCHECK_H
