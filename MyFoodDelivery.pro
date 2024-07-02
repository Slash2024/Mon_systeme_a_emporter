#-------------------------------------------------
#
# Project created by QtCreator 2024-06-27T22:20:51
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyFoodDelivery
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    user.cpp \
    food.cpp \
    order.cpp \
    database.cpp \
    login.cpp \
    signup.cpp \
    currentuser.cpp \
    userdashboard.cpp \
    usercheck.cpp \
    adminconsole.cpp \
    adminlogin.cpp

HEADERS += \
    user.h \
    food.h \
    order.h \
    database.h \
    login.h \
    signup.h \
    currentuser.h \
    userdashboard.h \
    usercheck.h \
    adminconsole.h \
    adminlogin.h

FORMS += \
    login.ui \
    signup.ui \
    userdashboard.ui \
    usercheck.ui \
    adminconsole.ui \
    adminlogin.ui
