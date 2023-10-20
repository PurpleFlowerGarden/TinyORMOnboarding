#include <orm/schema.hpp>

#include "BookModel.hpp"
#include "PublisherModel.hpp"

const QString BookModel::g_property_id = "id";
const QString BookModel::g_property_publisher("publisher");
const QString BookModel::g_property_author = "author";
const QString BookModel::g_property_title = "title";
const QString BookModel::g_property_releaseDate = "released";
const QString BookModel::g_property_isbn = "isbn";
const QString BookModel::g_table("books");

QStringList BookModel::u_fillable {
	g_property_id,
	g_property_author,
	g_property_title,
	g_property_releaseDate,
	g_property_isbn
};

const bool BookModel::u_snakeAttributes(false);

BookModel::BookModel(QString author, QString title, QDate releaseDate, QString isbn)
	: BookModel()
{
	setAuthor(author);
	setTitle(title);
	setReleaseDate(releaseDate);
	setIsbn(isbn);
}

BookModel::BookModel()
	: Model()
	, u_relations(relations())
	, u_table(table())
{
}

BookModel::~BookModel()
{
}

BookModel::BookModel(const BookModel &other)
	: Model(other)
	, u_relations(relations())
	, u_table(table())
{
}

BookModel::BookModel(Orm::Tiny::DontFillDefaultAttributes attributes)
	: Model(attributes)
	, u_relations(relations())
	, u_table(table())
{
}

BookModel::BookModel(const QVector<AttributeItem> &attributes)
	: Model(attributes)
	, u_relations(relations())
	, u_table(table())
{
}

BookModel::BookModel(std::initializer_list<AttributeItem> attributes)
	: Model(attributes)
	, u_relations(relations())
	, u_table(table())
{
}

BookModel &BookModel::operator=(const BookModel &other)
{
	Model::operator=(other);
	// do not overwrite u_relations and u_table
	return *this;
}

QHash<QString, BookModel::RelationVisitor> BookModel::relations() const
{
	return {
		{PublisherModel::g_table, [](RelationStore& visitor) { visitor(&BookModel::publisher); }}
	};
}

QString BookModel::table() const
{
	return g_table;
}

std::unique_ptr<BookModel::BelongsTo<BookModel, PublisherModel>>
BookModel::publisher()
{
	return belongsTo<PublisherModel>();
}

void BookModel::createTableCallback(Orm::SchemaNs::Blueprint& table)
{
	table.id();
	table.foreignId(g_property_publisher).nullable().constrained().cascadeOnDelete().cascadeOnUpdate();
	table.string(g_property_author);
	table.string(g_property_title);
	table.string(g_property_isbn);
	table.date(g_property_releaseDate);
	table.timestamps();
}

void BookModel::createTable()
{
    Orm::Schema::create(g_table, BookModel::createTableCallback);
}

void BookModel::dropTable()
{
    Orm::Schema::drop(g_table);
}

qlonglong BookModel::id() const
{
	return getAttribute(g_property_id).toLongLong();
}
void BookModel::setId(qlonglong id)
{
	setAttribute(g_property_id, id);
}

QString BookModel::author() const
{
	return getAttribute(g_property_author).toString();
}
void BookModel::setAuthor(QString author)
{
	setAttribute(g_property_author, author);
}

QString BookModel::title() const
{
	return getAttribute(g_property_title).toString();
}
void BookModel::setTitle(QString title)
{
	setAttribute(g_property_title, title);
}

QString BookModel::isbn() const
{
	return getAttribute(g_property_isbn).toString();
}
void BookModel::setIsbn(QString isbn)
{
	setAttribute(g_property_isbn, isbn);
}

QDate BookModel::releaseDate() const
{
	return getAttribute(g_property_releaseDate).toDate();
}
void BookModel::setReleaseDate(QDate released)
{
	setAttribute(g_property_releaseDate, released);
}
