#pragma once

#include <QtCore/QSharedPointer>
#include <QtTest>

#include <orm/db.hpp>
#include <orm/schema.hpp>

namespace MyNamespace
{
    struct MyTinyTest : QObject
    {
        Q_OBJECT
            
    public:
        MyTinyTest();
        ~MyTinyTest() Q_DECL_OVERRIDE;
    protected:
        static void demoTable(Orm::SchemaNs::Blueprint &table);

    private Q_SLOTS:
        void initTestCase();
        void cleanupTestCase();
        
        void init();
        void cleanup();

        void helloWorld();
        void invalidInsert();
        void columnAlias();

    private:
        std::shared_ptr<Orm::DatabaseManager> m_db;
    };
}
