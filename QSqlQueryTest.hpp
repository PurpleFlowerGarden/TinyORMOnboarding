#pragma once

#include <QtSql/QSqlDatabase>
#include <QtTest>

struct QSqlQueryTest : QObject, QSqlDatabase
{
    Q_OBJECT
            
public:
    QSqlQueryTest();
    ~QSqlQueryTest() Q_DECL_OVERRIDE;
protected:
    void demoTable();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
        
    void init();
    void cleanup();

    void helloWorld();
};
