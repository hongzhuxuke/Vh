#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <list>
#include <QString>

class DataBaseManager : public QObject
{
    Q_OBJECT

public:
    DataBaseManager(QObject *parent);
    ~DataBaseManager();

    void GetPlayFiles(std::list<QString>& playFileList);
    void InsertPlayFile(QString fileName);

private:
};
