#pragma once

#include <orm/tiny/model.hpp>

#include "PublisherModel.hpp"

class PublisherModel;

class BookModel final : public Orm::Tiny::Model<BookModel, PublisherModel>
{
    friend Model;
    using Model::Model;

public:
    BookModel(QString author, QString title, QDate releaseDate, QString isbn);
    BookModel();
    ~BookModel();
    BookModel(const BookModel &other);
    BookModel(Orm::Tiny::DontFillDefaultAttributes);
    BookModel(const QVector<AttributeItem> &attributes);
    BookModel(std::initializer_list<AttributeItem> attributes);
    BookModel &operator=(const BookModel &other);
    Model &operator=(Model &&) noexcept = delete;
    
    static void createTable();
    static void dropTable();

    qlonglong id() const;
    void setId(qlonglong id);

    QString author() const;
    void setAuthor(QString author);

    QString title() const;
    void setTitle(QString title);

    QDate releaseDate() const;
    void  setReleaseDate(QDate released);

    QString isbn() const;
    void setIsbn(QString isbn);

public:
    std::unique_ptr<BelongsTo<BookModel, PublisherModel>> publisher();

private:
    using RelationStore = Orm::Tiny::Support::Stores::BaseRelationStore<BookModel, PublisherModel>;
    
    static QStringList u_fillable;
    static const bool u_snakeAttributes;

    QHash<QString, RelationVisitor> u_relations;
    QString u_table;

protected:
    static void createTableCallback(Orm::SchemaNs::Blueprint& table);
    QHash<QString, RelationVisitor> relations() const;
    QString table() const;

public:
    static const QString g_property_id;
    static const QString g_property_publisher;
    static const QString g_property_author;
    static const QString g_property_title;
    static const QString g_property_releaseDate;
    static const QString g_property_isbn;
    static const QString g_table;
};




