#include "MyTinyTest.hpp"

namespace MyNamespace
{
	MyTinyTest::MyTinyTest()
	: QObject()
	, m_db{
		Orm::DB::create(
			QVariantHash{
				{"driver",                  "QSQLITE"},
				{"database",                qEnvironmentVariable("DB_DATABASE", "example.sqlite3")},
				{"foreign_key_constraints", qEnvironmentVariable("DB_FOREIGN_KEYS", "true")},
				 /*
				  * check_database_exists defaults to true and preempts implicit database creation
				  */
				{"check_database_exists",   false},
				{"qt_timezone",             QVariant::fromValue(Qt::UTC)},
				{"return_qdatetime",        true},
				{"prefix",                  ""}
			}
		)
	}
	{
	}
	
	MyTinyTest::~MyTinyTest()
	{
		m_db.reset();
	}
	
	void MyTinyTest::initTestCase()
	{
		qDebug() << "init";
	}

	void MyTinyTest::cleanupTestCase()
	{
		qDebug() << "cleanup";
	}

	void MyTinyTest::demoTable(Orm::SchemaNs::Blueprint &table)
	{
		table.id();
		table.integer("a").unique();
		table.Float("b");
	}

	void MyTinyTest::init()
	{
		try {
			Orm::Schema::create("demoTable", demoTable);
		} catch (const std::exception& ex) {
			qCritical() << ex.what();
		}
	}

	void MyTinyTest::cleanup()
	{
		try {
			Orm::Schema::drop("demoTable");
		} catch (const std::exception& ex) {
			qCritical() << ex.what();
		}
	}

	void MyTinyTest::helloWorld()
	{
		int rowCount(-1);
		float b[4]={0};
		try {
			Orm::DB::table("demoTable")->insert({
				QVariantMap{{"a", 2}, {"b", 5}},
				QVariantMap{{"a", 1}, {"b", 4.5}}
			});
			Orm::DB::table("demoTable")->insert({
				QVariantMap{{"a", 3}, {"b", 15}},
				QVariantMap{{"a", 4}, {"b", 14.5}}
			});
			
			rowCount = static_cast<int>(Orm::DB::table("demoTable")->count());
			b[0] = Orm::DB::table("demoTable")->whereEq("a", 1).value("b").toFloat();
			b[1] = Orm::DB::table("demoTable")->whereEq("a", 2).value("b").toFloat();
			b[2] = Orm::DB::table("demoTable")->whereEq("a", 3).value("b").toFloat();
			b[3] = Orm::DB::table("demoTable")->whereEq("a", 4).value("b").toFloat();

		} catch (const std::exception& ex) {
			qCritical() << ex.what();
		}
		QCOMPARE(rowCount, 4);
		QCOMPARE(b[0], 4.5);
		QCOMPARE(b[1], 5);
		QCOMPARE(b[2], 15);
		QCOMPARE(b[3], 14.5);
	}
	
	void MyTinyTest::invalidInsert()
	{
		bool exceptionReceived(false);
		try {
			Orm::DB::table("demoTable")->insert({
				QVariantMap{{"a", 2}, {"b", 5}},
				QVariantMap{{"a", 1}, {"b", 4.5}}
			});
			Orm::DB::table("demoTable")->insert({
				QVariantMap{{"a", 3}, {"b", 15}},
				QVariantMap{{"a", 1}, {"b", 14.5}}
			});
		} catch (const std::exception& ex) {
			exceptionReceived = true;
		}
		
		
		int rowCount(-1);
		try {
			rowCount = static_cast<int>(Orm::DB::table("demoTable")->count());
		} catch (const std::exception& ex) {
			qCritical() << ex.what();
		}
		
		QVERIFY(exceptionReceived);
		QCOMPARE(rowCount, 2);
	}
}

QTEST_MAIN(MyNamespace::MyTinyTest);
