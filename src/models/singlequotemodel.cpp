#include "singlequotemodel.h"
#include "qjsonarray.h"
#include "qjsondocument.h"
#include "qjsonobject.h"

int SingleQuoteModel::CONSTRUCTOR_COUNTER = 0;
int SingleQuoteModel::COPY_CONSTRUCTOR_COUNTER = 0;
int SingleQuoteModel::DESCTRUCTOR_COUNTER = 0;
int SingleQuoteModel::MOVE_CONSTRUCTOR_COUNTER = 0;

SingleQuoteModel::SingleQuoteModel(int id, QString quote, bool is_obscene, int cites, QList<QString>* f, QMap<QString, QString>* a, QString owner):
    _id(id), _quote(quote), _is_obscene(is_obscene), _cites(cites), _features(f), _attrs(a), _owner(owner)
{
    CONSTRUCTOR_COUNTER++;
    //qDebug() << "SingleQuoteModel object created";
}

SingleQuoteModel::SingleQuoteModel() {}

SingleQuoteModel::SingleQuoteModel(const SingleQuoteModel& other)
{
    COPY_CONSTRUCTOR_COUNTER++;
    //qDebug("SingleQuoteModel object copy constructor");
    this->_quote = other._quote;
    this->_cites = other._cites;
    this->_id = other._id;
    this->_is_obscene = other._is_obscene;
    this->_owner = other._owner;

    this->_features = new QList<QString>;
    this->_attrs = new QMap<QString, QString>;

    QString f;
    foreach(f, *other._features)
    {
        this->_features->append(f);
    }

    for (auto [key, value] : other._attrs->asKeyValueRange())
    {
        this->_attrs->insert(key, value);
    }
}

SingleQuoteModel::SingleQuoteModel(SingleQuoteModel&& moved) noexcept
{
    MOVE_CONSTRUCTOR_COUNTER++;
    _id = moved._id;
    _quote = moved._quote;
    _is_obscene = moved._is_obscene;
    _cites = moved._cites;
    _features = moved._features;
    _attrs = moved._attrs;
    _owner = moved._owner;
    moved._attrs = nullptr;
    moved._features = nullptr;
}

SingleQuoteModel& SingleQuoteModel::operator=(const SingleQuoteModel& right)
{
    if (this == &right) return *this;

    delete _features;
    delete _attrs;

    this->_quote = right._quote;
    this->_cites = right._cites;
    this->_id = right._id;
    this->_is_obscene = right._is_obscene;
    this->_owner = right._owner;

    this->_features = new QList<QString>;
    this->_attrs = new QMap<QString, QString>;

    QString f;
    foreach(f, *right._features)
    {
        this->_features->append(f);
    }

    for (auto [key, value] : right._attrs->asKeyValueRange())
    {
        this->_attrs->insert(key, value);
    }

    return *this;
}

SingleQuoteModel& SingleQuoteModel::operator=(SingleQuoteModel&& right) noexcept
{
    if (this == &right) return *this;

    delete _features;
    delete _attrs;

    this->_quote = right._quote;
    this->_cites = right._cites;
    this->_id = right._id;
    this->_is_obscene = right._is_obscene;
    this->_owner = right._owner;
    _features = right._features;
    _attrs = right._attrs;
    _owner = right._owner;
    right._attrs = nullptr;
    right._features = nullptr;

    return *this;
}

SingleQuoteModel::~SingleQuoteModel()
{
    DESCTRUCTOR_COUNTER++;
    //qDebug() << "SingleQuoteModel object destructor";
    delete _features;
    delete _attrs;

    _features = nullptr;
    _attrs = nullptr;
}

SingleQuoteModel *SingleQuoteModel::buildFromQVariant(QVariantMap rawQuote)
{
    int id = rawQuote.value("id").toInt();
    QString quote = rawQuote.value("quote").toString();
    bool is_obscene = rawQuote.value("obscene").toBool();
    int cites = rawQuote.value("cites").toInt();
    QList<QString>* features = new QStringList(rawQuote.value("features").toStringList());

    QVariantMap attrsMap = rawQuote.value("attrs").toMap();
    QMap<QString, QString>* attrs = new QMap<QString, QString>();
    for (auto it = attrsMap.begin(); it != attrsMap.end(); ++it) {
        QString key = it.key();
        QString value = it.value().toString();
        attrs->insert(key, value);
    }

    QString owner = rawQuote.value("owner").toString();

    return new SingleQuoteModel(id, quote, is_obscene, cites, features, attrs, owner);
}

QJsonDocument SingleQuoteModel::serializeToQJsonDocument()
{
    QJsonObject jobject;
    jobject["id"] = _id;
    jobject["quote"] = _quote;
    jobject["cites"] = _cites;
    jobject["features"] = QJsonArray::fromStringList(*_features);

    QVariantMap attrs;
    for (auto [key, value] : _attrs->asKeyValueRange())
    {
        attrs.insert(key, value);
    }
    jobject["attrs"] = QJsonObject::fromVariantMap(attrs);
    jobject["owner"] = _owner;
    jobject["obscene"] = _is_obscene;
    QJsonDocument doc( jobject );

    return doc;
}

QString const &SingleQuoteModel::quote() const {return _quote;}
int SingleQuoteModel::cites() const {return _cites;}
QList<QString> const & SingleQuoteModel::features() const {return *_features;}
QMap<QString, QString> const & SingleQuoteModel::attrs() const {return *_attrs;}
int SingleQuoteModel::id() const {return _id;}
bool SingleQuoteModel::is_obscene() const {return _is_obscene;}
QString const & SingleQuoteModel::owner() const {return _owner;}
