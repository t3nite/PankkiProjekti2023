#ifndef MAINUSERINTERFACE_H
#define MAINUSERINTERFACE_H

#include <QWidget>
#include <QMainWindow>
#include <mainwindow.h>

#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>

QT_BEGIN_NAMESPACE
namespace Ui {
class mainUserInterface;
}
QT_END_NAMESPACE

class mainUserInterface : public QWidget
{
    Q_OBJECT

public:
    explicit mainUserInterface(QByteArray& token, QString cardNumber, QWidget *parent = nullptr);
    ~mainUserInterface();

private:
    Ui::mainUserInterface *ui;

    QNetworkAccessManager *getManager;
    QNetworkReply *reply;
    QByteArray transResponse_data;

    QNetworkAccessManager *pgetManager;
    QNetworkReply *preply;
    QByteArray response_data;

    QNetworkAccessManager *getCreditManager;
    QNetworkReply *creditReply;
    QByteArray transCreditResponse_data;

    QNetworkAccessManager *pgetCreditManager;
    QNetworkReply *pCreditreply;
    QByteArray creditResponse_data;

    QByteArray webToken;

    QString cardNumber;

private slots:
   void logoutClicked();
   void withdrawMoneyClicked();

   void getDebitBalance();
   void debitRequestFinished(QNetworkReply *preply);
   void getCreditBalance();
   void creditRequestFinished(QNetworkReply *pCreditreply);

   void handleTransactionsClicked();
   void transactionsNetworkReqFin(QNetworkReply *reply);
   void handleCreditTransactionsClicked();
   void transactionsCreditNetworkReqFin(QNetworkReply *creditReply);
};

#endif // MAINUSERINTERFACE_H
