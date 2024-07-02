//用户登入 & 起始界面

#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "currentuser.h"

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();

private slots:
    void on_signUpButton_clicked();
    void on_logInBotton_clicked();
    void on_administratorButton_clicked();

private:
    Ui::login *ui;
};

#endif // LOGIN_H
