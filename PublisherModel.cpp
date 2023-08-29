#include <orm/schema.hpp>

#include "BookModel.hpp"
#include "PublisherModel.hpp"

const QString PublisherModel::g_property_id("id");
const QString PublisherModel::g_property_name("name");
const QString PublisherModel::g_property_founded("founded");
const QString PublisherModel::g_property_website("website");
const QString PublisherModel::g_table("publishers");

const QVector<QString> PublisherModel::u_with {
    BookModel::g_table
};

const QStringList PublisherModel::u_fillable {
    g_property_id,
    g_property_name,
    g_property_founded,
    g_property_website
};

const QVector<PublisherModel::AttributeItem> PublisherModel::u_attributes {
    { g_property_name, "<null>" },
    { g_property_founded, QDate(1970,1,1).toString(Qt::ISODate) },
    { g_property_website, "<null>" }
};

const bool PublisherModel::u_snakeAttributes(false);

PublisherModel::PublisherModel()
    : Model()
    , u_relations(relations())
    , u_table(table())
{
}
PublisherModel::~PublisherModel()
{
}

PublisherModel::PublisherModel(const PublisherModel &other)
    : Model(other)
    , u_relations(relations())
    , u_table(table())
{
}

PublisherModel::PublisherModel(QString name, QDate founded, QUrl website)
    : PublisherModel()
{
    setName(name);
    setFounded(founded);
    setWebsite(website);
}

PublisherModel::PublisherModel(Orm::Tiny::DontFillDefaultAttributes attributes)
    : Model(attributes)
    , u_relations(relations())
    , u_table(table())
{
}

PublisherModel::PublisherModel(const QVector<AttributeItem> &attributes)
    : Model(attributes)
    , u_relations(relations())
    , u_table(table())
{
}

PublisherModel::PublisherModel(std::initializer_list<AttributeItem> attributes)
    : Model(attributes)
    , u_relations(relations())
    , u_table(table())
{
}

PublisherModel &PublisherModel::operator=(const PublisherModel &other) {
    Model::operator=(other);
    // do not overwrite u_relations or u_table
    return *this;
}

void PublisherModel::createTableCallback(Orm::SchemaNs::Blueprint& table)
{
    table.id();
    table.string(g_property_name);
	table.date(g_property_founded);
	table.string(g_property_website);
	table.timestamps();
}

void PublisherModel::createTable()
{
    Orm::Schema::create(g_table, PublisherModel::createTableCallback);
}

void PublisherModel::dropTable()
{
    Orm::Schema::drop(g_table);
}


qlonglong PublisherModel::id() const
{
    return getAttribute(g_property_id).toLongLong();
}
void PublisherModel::setId(qlonglong id)
{
    setAttribute(g_property_id, id);
}
    
QString PublisherModel::name() const
{
    return getAttribute(g_property_name).toString();
}
void PublisherModel::setName(QString name)
{
    setAttribute(g_property_name, name);
}

QDate PublisherModel::founded() const
{
    return getAttribute(g_property_founded).toDate();
}
void PublisherModel::setFounded(QDate founded)
{
    setAttribute(g_property_founded, founded);
}

QUrl PublisherModel::website() const
{
    return getAttribute(g_property_website).toUrl();
}
void PublisherModel::setWebsite(QUrl website)
{
    setAttribute(g_property_website, website.toString());
}

std::unique_ptr<PublisherModel::HasMany<PublisherModel, BookModel>>
PublisherModel::books()
{
    return hasMany<BookModel>(BookModel::g_property_publisher);
}

QHash<QString, PublisherModel::RelationVisitor> PublisherModel::relations() const
{
    return {
        { BookModel::g_table, [](RelationStore& visitor) { visitor(&PublisherModel::books); } }
    };
}

QString PublisherModel::table() const
{
    return g_table;
}
