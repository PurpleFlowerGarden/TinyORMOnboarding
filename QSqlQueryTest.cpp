#include <cstdio>
#include <cstdlib>

#include <QtCore/QCoreApplication>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include "QSqlQueryTest.hpp"

QSqlQueryTest::QSqlQueryTest()
: QObject()
, QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"))
{
	QSqlDatabase::setDatabaseName(":memory:");
}

QSqlQueryTest::~QSqlQueryTest()
{
}

void QSqlQueryTest::initTestCase()
{
	QVERIFY(QSqlDatabase::open());
}

void QSqlQueryTest::cleanupTestCase()
{
	QSqlDatabase::close();
}

void QSqlQueryTest::init()
{
	QSqlQuery query(*this);
	bool success = query.exec("CREATE TABLE foobar(id INTEGER PRIMARY KEY AUTOINCREMENT, message TEXT)");
	if (!success) {
		qCritical() << query.lastError().text().toStdString();
	}
	QVERIFY(success);
}

void QSqlQueryTest::cleanup()
{
	QSqlQuery query(*this);
	bool success = query.exec("DROP TABLE foobar");
	if (!success) {
		qCritical() << query.lastError().text().toStdString();
	}
	QVERIFY(success);
}

void QSqlQueryTest::helloWorld()
{
}

QTEST_MAIN(QSqlQueryTest)
