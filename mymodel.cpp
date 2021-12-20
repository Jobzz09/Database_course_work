#include "mymodel.h"

MyModel::MyModel(QObject *parent, QSqlDatabase db) : QSqlTableModel(parent)
{
    QSqlTableModel(parent, db);

    connect(this, &QSqlTableModel::dataChanged,
            this, &MyModel::DataChanged);
}

void MyModel::DataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
 qDebug() << "Data Changed";

 qDebug() << topLeft << bottomRight;
}
