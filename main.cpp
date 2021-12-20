#include "login_widget.h"

#include <QApplication>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Login_widget* loginWidget = new Login_widget();
    loginWidget->show();

    return a.exec();
}
