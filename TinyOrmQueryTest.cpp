#include "BookModel.hpp"
#include "PublisherModel.hpp"

#include "TinyOrmQueryTest.hpp"

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
			PublisherModel::createTable();
			BookModel::createTable();
		} catch (const std::exception& ex) {
			qCritical() << ex.what();
		}
	}

	void MyTinyTest::cleanup()
	{
		try {
			BookModel::dropTable();
		} catch (const std::exception& ex) {
			qCritical() << ex.what();
		}
		
		try {
			PublisherModel::dropTable();
		} catch (const std::exception& ex) {
			qCritical() << ex.what();
		}
		
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

	void MyTinyTest::ormSimple()
	{
		qlonglong id(-1);
		try {
			PublisherModel pub("Chilton Book Company", QDate(1904, 3, 31), QUrl("https://www.cengagegroup.com/"));
			pub.save();
			
			std::optional<PublisherModel> readback(PublisherModel::find(pub.id()));
			QVERIFY(readback);
			
			PublisherModel pub2(readback.value());
			QCOMPARE(pub.name(), pub2.name());
			QCOMPARE(pub.founded(), pub2.founded());
			QCOMPARE(pub.website(), pub2.website());
		} catch (const std::exception& ex) {
			qCritical() << ex.what();
			QFAIL(ex.what());
		}
	}
	
	void MyTinyTest::ormOneToMany()
	{
		try {
			PublisherModel chilton("Chilton Book Company", QDate(1904,  3, 31), QUrl("https://www.cengagegroup.com/"));
			PublisherModel penguin("Penguin Random House", QDate(2013,  7,  1), QUrl("https://penguinrandomhouse.com/"));
			PublisherModel bloomsbury("Bloomsbury Publishing", QDate(1986,  9,  26), QUrl("http://bloomsbury.com/"));
			chilton.save();
			penguin.save();
			bloomsbury.save();
			
			QVector<BookModel> dune{
				BookModel("Frank Herbert", "Dune",                QDate(1965,  1,  1),  "978-0801950773"),
				BookModel("Frank Herbert", "Dune Messiah",        QDate(1969,  1,  1),  "978-1940884677"),
				BookModel("Frank Herbert", "Children of Dune",    QDate(1976,  4, 21),  "978-1940884677"),
				BookModel("Frank Herbert", "God Emperor of Dune", QDate(1981,  5,  1),  "978-0399125935"),
				BookModel("Frank Herbert", "Heretics of Dune",    QDate(1984,  3,  1),  "978-0575034235"),
				
				BookModel("Brian Herbert", "House Attreides",     QDate(1999, 10,  5),  "978-0553110616"),
				BookModel("Brian Herbert", "House Harkonnen",     QDate(2000, 10,  3),  "978-0553110722"),
				BookModel("Brian Herbert", "House Corrino",       QDate(2001, 10,  2),  "978-0553110845")
			};
			QVector<BookModel> harryPotter{
				BookModel("J.K. Rowling",  "Harry Potter and the Philosopher's Stone",   QDate(1997,  6,  7),  "978-0747532699"),
				BookModel("J.K. Rowling",  "Harry Potter and the Chamber of Secrets",    QDate(1998,  7,  2),  "978-0747538493"),
				BookModel("J.K. Rowling",  "Harry Potter and the Prisoner of Azkaban",   QDate(1999,  7,  8),  "978-0747542155"),
				BookModel("J.K. Rowling",  "Harry Potter and the Goblet of Fire",        QDate(2000,  7,  8),  "978-0747546245"),
				BookModel("J.K. Rowling",  "Harry Potter and the Order of the Phoenix",  QDate(2003,  6, 21),  "978-0747551003"),
				BookModel("J.K. Rowling",  "Harry Potter and the Half-Blood Prince",     QDate(2005,  7, 16),  "978-0747581086"),
				BookModel("J.K. Rowling",  "Harry Potter and the Deathly Hallows",       QDate(2007,  7, 21),  "978-0747591054")
			};
			
			chilton.books()->save(dune[0]);
			penguin.books()->saveMany(QVector<BookModel>(dune.constBegin()+1, dune.constEnd()));
			bloomsbury.books()->saveMany(harryPotter);
		}
		catch (const std::exception& ex) {
			qCritical() << ex.what();
			QFAIL(ex.what());
		}
	}
}

QTEST_MAIN(MyNamespace::MyTinyTest);
