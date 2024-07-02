//管理员登入界面

#ifndef ADMINLOGIN_H
#define ADMINLOGIN_H

#include <QWidget>
#include <QMessageBox>

namespace Ui {
class adminlogin;
}

class adminlogin : public QWidget
{
    Q_OBJECT

public:
    explicit adminlogin(QWidget *parent = 0);
    ~adminlogin();

private slots:
    void on_logInBotton_clicked();

private:
    Ui::adminlogin *ui;
};

#endif // ADMINLOGIN_H
