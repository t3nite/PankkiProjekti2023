#ifndef LAST5TRANSACTIONS_H
#define LAST5TRANSACTIONS_H

#include <QString>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>

class last5Transactions
{

public:
    explicit last5Transactions();

    QString getIdaccount() const;
    void setIdaccount(const QString &value);

    QString getTransactions() const;
    void setTransactions(const QString &value);

    QString getAmount() const;
    void setAmount(const QString &value);

    QString getDate() const;
    void setDate(const QString &value);

private:

    QString idaccount;
    QString transactions;
    QString amount;
    QString date;

    QNetworkAccessManager *getManager;
    QNetworkReply *reply;
    QByteArray response_data;
};

#endif // LAST5TRANSACTIONS_H
