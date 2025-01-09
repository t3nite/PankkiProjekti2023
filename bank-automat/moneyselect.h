#ifndef MONEYSELECT_H
#define MONEYSELECT_H

#include <QWidget>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>

QT_BEGIN_NAMESPACE
namespace Ui {
class moneySelect;
}
QT_END_NAMESPACE

class moneySelect : public QWidget
{
    Q_OBJECT

public:
    explicit moneySelect(QByteArray& token, QString cardNumber, QWidget *parent = nullptr);
    ~moneySelect();

private:
    Ui::moneySelect *ui;

    // Debit

    QNetworkAccessManager *getManager;
    QNetworkReply *getReply;
    QByteArray getResponse_data;

    QNetworkAccessManager *putManager;
    QNetworkReply *putReply;
    QByteArray putResponse_data;

    QNetworkAccessManager *postManager;
    QNetworkReply *postReply;
    QByteArray postResponse_data;

    // Credit

    QNetworkAccessManager *getManagerCredit;
    QNetworkReply *getReplyCredit;
    QByteArray getResponse_dataCredit;

    QNetworkAccessManager *putManagerCredit;
    QNetworkReply *putReplyCredit;
    QByteArray putResponse_dataCredit;

    QNetworkAccessManager *postManagerCredit;
    QNetworkReply *postReplyCredit;
    QByteArray postResponse_dataCredit;

    QByteArray webToken;

    QString cardNumber;

private slots:
    void handleBackToMenu();
    void handleOtherAmount();

    // Debit

    void twentyEuroClickedPut();
    void put20Slot(QNetworkReply *putReply);
    void twentyEuroClickedGet();
    void twentyEuroClickedPost(QNetworkReply *getReply);
    void post20Slot(QNetworkReply *postReply);

    void fortyEuroClickedPut();
    void put40Slot(QNetworkReply *putReply);
    void fortyEuroClickedGet();
    void fortyEuroClickedPost(QNetworkReply *getReply);
    void post40Slot(QNetworkReply *postReply);

    void fiftyEuroClickedPut();
    void put50Slot(QNetworkReply *putReply);
    void fiftyEuroClickedGet();
    void fiftyEuroClickedPost(QNetworkReply *getReply);
    void post50Slot(QNetworkReply *postReply);

    void hundredEuroClickedPut();
    void put100Slot(QNetworkReply *putReply);
    void hundredEuroClickedGet();
    void hundredEuroClickedPost(QNetworkReply *getReply);
    void post100Slot(QNetworkReply *postReply);

    // Credit

    void twentyEuroClickedPutCredit();
    void put20SlotCredit(QNetworkReply *putReplyCredit);
    void twentyEuroClickedGetCredit();
    void twentyEuroClickedPostCredit(QNetworkReply *getReplyCredit);
    void post20SlotCredit(QNetworkReply *postReplyCredit);

    void fortyEuroClickedPutCredit();
    void put40SlotCredit(QNetworkReply *putReplyCredit);
    void fortyEuroClickedGetCredit();
    void fortyEuroClickedPostCredit(QNetworkReply *getReplyCredit);
    void post40SlotCredit(QNetworkReply *postReplyCredit);

    void fiftyEuroClickedPutCredit();
    void put50SlotCredit(QNetworkReply *putReplyCredit);
    void fiftyEuroClickedGetCredit();
    void fiftyEuroClickedPostCredit(QNetworkReply *getReplyCredit);
    void post50SlotCredit(QNetworkReply *postReplyCredit);

    void hundredEuroClickedPutCredit();
    void put100SlotCredit(QNetworkReply *putReplyCredit);
    void hundredEuroClickedGetCredit();
    void hundredEuroClickedPostCredit(QNetworkReply *getReplyCredit);
    void post100SlotCredit(QNetworkReply *postReplyCredit);

    // Functions below for testing purposes

   /* // Debit

    void insertHundredClickedPut();
    void putInsertedMoneySlot(QNetworkReply *putReply);
    void insertHundredClickedPost();
    void postInsertedMoneySlot(QNetworkReply *postReply);

    // Credit

    void insertHundredClickedPutCredit();
    void putInsertedMoneySlotCredit(QNetworkReply *putReplyCredit);
    void insertHundredClickedPostCredit();
    void postInsertedMoneySlotCredit(QNetworkReply *postReplyCredit); */
};

#endif // MONEYSELECT_H
