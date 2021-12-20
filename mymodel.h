#ifndef MYMODEL_H
#define MYMODEL_H

#include <QSqlTableModel>
#include <QDebug>

class MyModel : public QSqlTableModel
{
public:
    explicit MyModel(QObject *parent, QSqlDatabase db);

public slots:
    void DataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);
};

#endif // MYMODEL_H
