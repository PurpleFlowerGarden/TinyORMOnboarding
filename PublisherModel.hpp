#pragma once

#include <orm/tiny/model.hpp>

#include "BookModel.hpp"

class BookModel;

class PublisherModel final : public Orm::Tiny::Model<PublisherModel, BookModel>
{
    friend Model;
    using Model::Model;

public:
    PublisherModel(QString name, QDate founded, QUrl website);
    PublisherModel();
    ~PublisherModel();
    PublisherModel(const PublisherModel &other);
    PublisherModel(Orm::Tiny::DontFillDefaultAttributes);
    PublisherModel(const QVector<AttributeItem> &attributes);
    PublisherModel(std::initializer_list<AttributeItem> attributes);
    PublisherModel &operator=(const PublisherModel &other);
    Model &operator=(Model &&) noexcept = delete;
    
    
    static void createTable();
    static void dropTable();
    
    qlonglong id() const;
    void setId(qlonglong id);
    
    QString name() const;
    void setName(QString name);
    
    QDate founded() const;
    void setFounded(QDate founded);
    
    QUrl website() const;
    void setWebsite(QUrl website);
    
public:
    std::unique_ptr<HasMany<PublisherModel, BookModel>> books();

private:
    static const QVector<QString> u_with;
    static const QStringList u_fillable;
    static const QVector<AttributeItem> u_attributes;
    static const bool u_snakeAttributes;

    using RelationStore = Orm::Tiny::Support::Stores::BaseRelationStore<PublisherModel, BookModel>;
    QHash<QString, RelationVisitor> u_relations;
    QString u_table;

protected:
    static void createTableCallback(Orm::SchemaNs::Blueprint& table);
    QHash<QString, RelationVisitor> relations() const;
    QString table() const;
    
public:
    static const QString g_property_id;
    static const QString g_property_name;
    static const QString g_property_founded;
    static const QString g_property_website;
    static const QString g_table;
};
