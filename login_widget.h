#ifndef LOGIN_WIDGET_H
#define LOGIN_WIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QApplication>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "mainwindow.h"

namespace Ui {
class Login_widget;
}

class Login_widget : public QWidget
{
    Q_OBJECT

public:
    explicit Login_widget(QWidget *parent = nullptr);
    ~Login_widget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Login_widget *ui;

    MainWindow* w;
};

#endif // LOGIN_WIDGET_H
