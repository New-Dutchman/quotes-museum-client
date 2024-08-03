#ifndef SINGLEQUOTEMODEL_H
#define SINGLEQUOTEMODEL_H

#include <QString>
#include <QWidget>
#include <QList>


class SingleQuoteModel
{

public:
    SingleQuoteModel();
    SingleQuoteModel(int id, QString quote, bool is_obscene, int cites, QList<QString>*, QMap<QString, QString>*, QString owner);
    SingleQuoteModel(const SingleQuoteModel& other);
    SingleQuoteModel(SingleQuoteModel&& moved) noexcept;
    SingleQuoteModel& operator=(const SingleQuoteModel& right);
    SingleQuoteModel& operator=(SingleQuoteModel&& right) noexcept;

    // getters only
    QString const &quote() const;
    int cites() const;
    QList<QString> const &features() const;
    QMap<QString, QString> const  &attrs() const;
    int id() const;
    bool is_obscene() const;
    QString const &owner() const;
    virtual ~SingleQuoteModel();

    static SingleQuoteModel* buildFromQVariant(QVariantMap);
    QJsonDocument serializeToQJsonDocument();

    // for debug
    static int CONSTRUCTOR_COUNTER;
    static int COPY_CONSTRUCTOR_COUNTER;
    static int DESCTRUCTOR_COUNTER;
    static int MOVE_CONSTRUCTOR_COUNTER;

private:
    int _id;
    QString _quote;
    bool _is_obscene;
    int _cites;
    QList<QString>* _features;
    QMap<QString, QString>* _attrs;
    QString _owner;
    // int _whoAdded; to be added in future
};

#endif // SINGLEQUOTEMODEL_H
