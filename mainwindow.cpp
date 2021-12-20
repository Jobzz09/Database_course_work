#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, QSqlDatabase* database, QString connName)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      database(database),
      connName(connName)
{
    ui->setupUi(this);

    this->init_comboboxes();
    this->init_tableViews();
    this->fill_queryComboBox();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::init_comboboxes()
{
    for (int i = 0; ui->comboBox_editDatabase->count() > 0;)
    {
        ui->comboBox_editDatabase->removeItem(i);
        ui->comboBox_addToDatabase->removeItem(i);
        ui->comboBox_editRecord->removeItem(i);
    }


    QSqlQuery query(*database);

    QString queryText = "SELECT table_name FROM information_schema.tables WHERE table_schema = 'public' AND table_type = 'BASE TABLE';";

    if (!query.exec(queryText))
    {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("ERROR!");
        msgBox.setInformativeText("Can't load data from database");
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        if (ret == QMessageBox::Ok)
        {
            msgBox.close();
        }
    }


    while (query.next())
    {
        QString elem = query.value(0).toString();
        ui->comboBox_addToDatabase->addItem(elem);
        ui->comboBox_editDatabase->addItem(elem);
        ui->comboBox_editRecord->addItem(elem);
    }
}

void MainWindow::init_tableViews()
{
    QString table = ui->comboBox_editDatabase->currentText();

    QSqlQuery query (*this->database);

    QSqlTableModel* model = new QSqlTableModel(this, *this->database);

    model->setTable(table);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    bool status = model->select();

    QSqlRecord record = model->record(model->rowCount() - 1);


    ui->tableView_editDatabase->setModel(model);
}

void MainWindow::update_tableView (QTableView* tableView)
{

}

void MainWindow::notify_succes(QString mainText, QString InfoText)
{

}
void MainWindow::notify_critical(QString mainText, QString InfoText)
{

}
void MainWindow::notify_warning (QString mainText, QString InfoText)
{

}


void MainWindow::on_comboBox_addToDatabase_currentIndexChanged(const QString &data)
{
    QString table = data;

    QSqlQuery query (*this->database);
    QSqlTableModel* model = new QSqlTableModel(this, *this->database);

    model->setTable(data);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    QSqlRecord record = model->record(model->rowCount() - 1);
    for (int i = 0; i < model->columnCount(); ++i)
    {
        record.setNull(i);
    }
    bool status = model->insertRecord(model->rowCount(), record);

    ui->tableView_addToDatabase->setModel(model);
}


void MainWindow::on_comboBox_editDatabase_currentIndexChanged(const QString &data)
{
    QString table = data;
    QSqlTableModel* model = new QSqlTableModel(this, *this->database);

    model->setTable(table);

    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    bool status = model->select();

    connect(model, &QSqlTableModel::dataChanged,
            this, &MainWindow::item_data_changed);

    ui->tableView_editDatabase->setModel(model);
    ui->tableView_editDatabase->setEditTriggers(QAbstractItemView::NoEditTriggers);



}

void MainWindow::item_data_changed(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
    qDebug() << "Data changed " << topLeft << bottomRight;
}


void MainWindow::on_pushButton_add_new_data_clicked()
{
    QString table = ui->comboBox_addToDatabase->currentText();
    QString fields;
    QSqlQuery query (*this->database);

    bool status = query.exec("SELECT column_name FROM information_schema.columns WHERE table_name = '" + table + "' ORDER BY ordinal_position ASC;");

    QString queryText = "INSERT INTO ";
    queryText += table + "(";
    while (query.next())
    {
        queryText += query.value(0).toString() + ", ";
    }
    queryText.remove(queryText.length() - 2, 2);
    queryText += ") VALUES(";


    QSqlTableModel* model = (QSqlTableModel*)ui->tableView_addToDatabase->model();
    QSqlRecord record = model->record(0);
    for (int i = 0; i < model->columnCount(); ++i)
    {
        QString tmp_str = "'" + record.value(i).toString() + "'";
        queryText += tmp_str + ", ";
    }
    queryText.remove(queryText.length() - 2, 2);
    queryText += ");";

    if (!query.exec(queryText))
    {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("ERROR!");
        msgBox.setInformativeText("Can't load data from database" + query.lastError().text());
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        if (ret == QMessageBox::Ok)
        {
            msgBox.close();
        }
    }

   // this->on_comboBox_addToDatabase_currentIndexChanged(table);

}


void MainWindow::on_pushButton_custom_query_enter_clicked()
{
    try {
        QSqlQuery query (*this->database);

        std::string tmp = ui->lineEdit_customQuery->text().toStdString();

        std::string select = "";
        for (int i = 0; i < 6; ++i)
        {
            char c = std::toupper(tmp[i]);
            select += c;
        }
        if (select == "SELECT")
        {
            QSqlQueryModel* queryModel = new QSqlQueryModel();

            query.exec(ui->lineEdit_customQuery->text());
            queryModel->setQuery(query);
            QTableView * currTableView = new QTableView();
            currTableView->setModel(queryModel);
            currTableView->show();
        }

        if (tmp.substr(0, 3) == "--i")
        {
            std::vector<std::string>* queries = this->execute_sql_file(tmp.substr(4, tmp.length() - 1));

            for (auto elem : *queries)
            {
                if  (!query.exec(QString::fromStdString(elem)))
                    qDebug() << query.lastError();
            }


            delete queries;
            QMessageBox msgBox(this);
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setText("Table created");
            msgBox.setDefaultButton(QMessageBox::Ok);
            int ret = msgBox.exec();

            this->init_comboboxes();
            this->init_tableViews();
            return;
        }

        if (!query.exec(ui->lineEdit_customQuery->text()))
        {
            QMessageBox msgBox(this);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText("ERROR!");
            msgBox.setInformativeText("Can't load data from database" + query.lastError().text());
            msgBox.setDefaultButton(QMessageBox::Ok);
            int ret = msgBox.exec();
            this->init_comboboxes();
            this->init_tableViews();
            return;
        }
        query.clear();



    }  catch (std::exception& e) {


        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("ERROR!");
        msgBox.setInformativeText(e.what());
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        this->init_comboboxes();

        qDebug() << "Smth went wrong";
    }



    }


void MainWindow::on_pushButton_find_in_database_clicked()
{
    QString target = ui->lineEdit_search_in_database->text();

    QSqlTableModel* model = (QSqlTableModel*)ui->tableView_editDatabase->model();
    int rown = 0;
    for (int k = 0; k < model->rowCount(); ++k)
    {
        QSqlRecord record = model->record(k);
        for (int i = 0; i < model->columnCount(); ++i)
        {
            if (record.value(i).toString() == target)
            {
                ui->tableView_editDatabase->selectRow(rown);
                qDebug() << rown;
            }
        }
        rown++;
    }
}



void MainWindow::on_tableView_editDatabase_pressed(const QModelIndex &index)
{
    QSqlTableModel* model = (QSqlTableModel*)ui->tableView_editDatabase->model();
    int n = index.row();
    qDebug() << n;

}

void MainWindow::on_pushButton_getRecord_clicked()
{
   QString table = ui->comboBox_editRecord->currentText();

   QSqlTableModel* tmp_model = new QSqlTableModel(this, *this->database);
   tmp_model->setTable(table);
   tmp_model = (QSqlTableModel*) ui->tableView_editDatabase->model();

   qDebug() << ui->lineEdit_recordNumber->text().toInt();
   QSqlRecord* tmp_record = new QSqlRecord(tmp_model->record(ui->lineEdit_recordNumber->text().toInt() -1));

   QSqlTableModel* currentModel = new QSqlTableModel(this, *this->database);
   currentModel->setTable(table);
   currentModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

   qDebug() << currentModel->tableName();

   qDebug() << "Insert: " << currentModel->insertRecord(-1, *tmp_record);

   ui->tableView->setModel(currentModel);

}


void MainWindow::on_pushButton_saveEdit_clicked()
{
    QSqlTableModel* model = (QSqlTableModel*)ui->tableView_editDatabase->model();
    QSqlRecord oldRecord = model->record(ui->lineEdit_recordNumber->text().toUInt() - 1);

    model = (QSqlTableModel*)ui->tableView->model();
    QSqlRecord newRecord = model->record(0);

    QString queryText = "UPDATE " + ui->comboBox_editRecord->currentText()
                        + " SET ";
    QSqlQuery query(*this->database);
    for (int i = 0; i < newRecord.count(); ++i)
    {
        queryText += newRecord.fieldName(i) + " = '";
        queryText += newRecord.value(i).toString() + "', ";
    }

    queryText.remove(queryText.length() - 2, 2);
    queryText += " WHERE " + oldRecord.fieldName(0) + " = '"
            + (oldRecord.value(0).toString()) + "';";

    qDebug() << queryText;
    bool status = query.exec(queryText);
    {
        if (status)
        {
            qDebug() << "Good";
        }
    }
    if (!status)
    {

        QMessageBox msgBox(this);
        msgBox.setText("Can't update data!");
        msgBox.setInformativeText(query.lastError().text());
        msgBox.setDefaultButton(QMessageBox::Ok);
        qDebug() << "Bad" << query.lastError();
        int a = msgBox.exec();
    }

    query.clear();

    init_tableViews();
}





void MainWindow::on_comboBox_sqlQueries_currentTextChanged(const QString &arg1)
{
    ui->lineEdit_customQuery->setText(arg1);
}

void MainWindow::fill_queryComboBox()
{
    QStringList queries;
    queries.append("--i [input file name]");
    queries.append("SELECT * FROM table1;");
    queries.append("CREATE VIEW view AS SELECT  FROM ;");
    queries.append("DELETE FROM table_name WHERE field = 'value';");
    queries.append("DROP TABLE table_name;");

    for (auto elem : queries)
        ui->comboBox_sqlQueries->addItem(elem);
}

std::vector<std::string>* MainWindow::execute_sql_file(const std::string sql_file)
{
    try
    {
        std::string s;
        std::ifstream f(sql_file);
        if (f.is_open())
        {
            std::string line;
            while(getline(f, line))
            {
                boost::trim_all(line);
                if (strncmp(line.c_str(), "--", 2))
                    s+=line+"";
            }
            f.close();

            boost::trim_all(s);

            std::vector<std::string> sql_commands;

            for(string_split_iterator It=make_split_iterator(s, first_finder(";", boost::is_iequal()));
                It!=string_split_iterator();
                ++It)
            {
                std::string c=boost::copy_range<std::string>(*It);
                if (!c.empty())
                {
                    c += ";";
                    sql_commands.push_back(c);
                }
            }

            std::vector<std::string>* result = new std::vector<std::string>;
            for (auto elem : sql_commands)
            {
                result->push_back(elem);
            }

            return result;
        }

    }
    catch (std::exception& e)
    {
        return nullptr;
    }
}
