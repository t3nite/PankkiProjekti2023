#ifndef SELECTAMOUNT_H
#define SELECTAMOUNT_H

#include <QWidget>

#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>

namespace Ui {
class selectAmount;
}

class selectAmount : public QWidget
{
    Q_OBJECT

public:
    explicit selectAmount(QByteArray& token, QString cardNumber, QWidget *parent = nullptr);
    ~selectAmount();

private:
    Ui::selectAmount *ui;
    QString enteredNum;

    // Debit
    QNetworkAccessManager *putManager;
    QNetworkReply *putReply;
    QByteArray putResponse_data;

    QNetworkAccessManager *postManager;
    QNetworkReply *postReply;
    QByteArray postResponse_data;

    QNetworkAccessManager *getManager;
    QNetworkReply *getReply;
    QByteArray getResponse_data;

    // Credit
    QNetworkAccessManager *putManagerCredit;
    QNetworkReply *putReplyCredit;
    QByteArray putResponse_dataCredit;

    QNetworkAccessManager *postManagerCredit;
    QNetworkReply *postReplyCredit;
    QByteArray postResponse_dataCredit;

    QNetworkAccessManager *getManagerCredit;
    QNetworkReply *getReplyCredit;
    QByteArray getResponse_dataCredit;

    QByteArray webToken;
    QString cardNumber;
    double newAccountBalance;

private slots:
    void numClickedHandler();
    void clearLe();
    void backToMoneySelect();

    // Debit

    void putAmount();
    void putSelectAnyAmount (QNetworkReply *putReply);

    void getAmount();
    void postSelectAnyAmount(QNetworkReply *getReply);
    void postAnyAmount(QNetworkReply *postReply);

    // Credit

    void putAmountCredit();
    void putSelectAnyAmountCredit (QNetworkReply *putReplyCredit);

    void getAmountCredit();
    void postSelectAnyAmountCredit(QNetworkReply *getReplyCredit);
    void postAnyAmountCredit(QNetworkReply *postReplyCredit);
};

#endif // SELECTAMOUNT_H
