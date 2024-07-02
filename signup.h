//用户注册界面

#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>
#include "database.h"

namespace Ui {
class signup;
}

class signup : public QWidget
{
    Q_OBJECT

public:
    explicit signup(QWidget *parent = 0);
    ~signup();

private slots:
    void on_sureButton_clicked();
    void on_returnButton_clicked();

private:
    Ui::signup *ui;
};

#endif // SIGNUP_H
