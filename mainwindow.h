#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMessageBox>
#include <QApplication>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSizePolicy>
#include <QAbstractTableModel>
#include <QSqlField>
#include <QDebug>
#include <QTableView>
#include <QModelIndex>
#include <QModelIndexList>
#include <QSortFilterProxyModel>
#include "mymodel.h"
#include <cctype>

#include <boost/algorithm/string/trim_all.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/find_iterator.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string/find_iterator.hpp>
typedef boost::split_iterator<std::string::iterator> string_split_iterator;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent, QSqlDatabase* database, QString connName);
    ~MainWindow();

private slots:
    void on_comboBox_addToDatabase_currentIndexChanged(const QString &arg1);

    void on_comboBox_editDatabase_currentIndexChanged(const QString &arg1);

    void on_pushButton_add_new_data_clicked();

    void on_pushButton_custom_query_enter_clicked();

    void on_pushButton_find_in_database_clicked();

    void on_tableView_editDatabase_pressed(const QModelIndex &index);

    void item_data_changed(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);

    void on_pushButton_getRecord_clicked();

    void on_pushButton_saveEdit_clicked();

    void on_comboBox_sqlQueries_currentTextChanged(const QString &arg1);

private:

    void init_comboboxes();
    void init_tableViews();

    void update_tableView (QTableView* tableView);

    void notify_succes(QString mainText, QString InfoText);
    void notify_critical(QString mainText, QString InfoText);
    void notify_warning (QString mainText, QString InfoText);

    void fill_queryComboBox();
    std::vector<std::string>* execute_sql_file(const std::string sql_file);



    Ui::MainWindow *ui;

    QSqlDatabase* database;
    QString connName;
};
#endif // MAINWINDOW_H
