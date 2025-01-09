#ifndef ACCWITHDRAWALS_H
#define ACCWITHDRAWALS_H

#include <QWidget>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QStandardItem>


namespace Ui {
class accWithdrawals;
}

class accWithdrawals : public QWidget
{
    Q_OBJECT

public:
    explicit accWithdrawals(QByteArray& token, QString cardNumber, QWidget *parent = nullptr);
    ~accWithdrawals();

    // Public member function transTableWidget
    void transTableWidget(QStandardItemModel *model);

private:
    Ui::accWithdrawals *ui;

    QNetworkAccessManager *deleteManager;
    QNetworkReply *deleteReply;
    QByteArray deleteResponse_data;

    QByteArray webToken;
    QString cardNumber;

private slots:
    void handleBack();

   // For testing purposes
   // void deleteResource();
};

#endif // ACCWITHDRAWALS_H
