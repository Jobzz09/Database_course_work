#include "login_widget.h"
#include "ui_login_widget.h"

Login_widget::Login_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login_widget)
{
    ui->setupUi(this);

    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
}

Login_widget::~Login_widget()
{
    delete ui;
}

void Login_widget::on_pushButton_clicked()
{
    QString connName = "Connection_1";
    QSqlDatabase* database = new QSqlDatabase;
    *database = QSqlDatabase::addDatabase("QPSQL", connName);

    try
    {
        database->setHostName(ui->lineEdit_hostName->text());
        database->setDatabaseName(ui->lineEdit_databaseName->text());
        database->setUserName(ui->lineEdit_user->text());
        database->setPassword(ui->lineEdit_password->text());

        if (!database->open())
        {
            QString error = database->lastError().text();
            throw QSqlError("QPSQL", "CAN'T OPEN CONNECTION WITH DATABASE: " + error);
        }

        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Icon::NoIcon);
        msgBox.setText("Connected to database!");
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        if (ret == QMessageBox::Ok)
        {
            this->w = new MainWindow(this, database, connName);
            this->close();
            this->w->show();
        }

    }  catch (QSqlError& e)
    {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("CRITICAL ERROR!");
        msgBox.setInformativeText(e.text());
        msgBox.setDefaultButton(QMessageBox::Ok);

        int ret = msgBox.exec();
        if (ret == QMessageBox::Ok)
            msgBox.close();

    }

}

