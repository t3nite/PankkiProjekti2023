#ifndef ADDPIN_H
#define ADDPIN_H

#include <QMainWindow>
#include <QLineEdit>

#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>

QT_BEGIN_NAMESPACE
namespace Ui {
class addPin;
}
QT_END_NAMESPACE

class addPin : public QWidget{
    Q_OBJECT

public:
    explicit addPin(QString cardNumber, QWidget *parent = nullptr);
    ~addPin();

private:
    Ui::addPin *ui;
    QString  enteredPin;
    QTimer *timer;
    QString correctPin = "";
    int cardtype = 0;

    QNetworkAccessManager *pgetManager;
    QNetworkReply *preply;
    QByteArray response_data;

    QNetworkAccessManager *pgetManagerPin;
    QNetworkReply *preplyPin;
    QByteArray response_dataPin;

    QNetworkAccessManager *loginManager;
    QNetworkReply *loginReply;
    QByteArray loginResponse_data;

    // Reset the incorrect PIN attempts counter
    int wrongPinAttempts = 0;

    // Maximum number of incorrect PIN attempts
    const int maxWrongAttempts = 3;

    QByteArray webToken;

    QString cardNumber;

private slots:
    void handlePinInsert();
    void numberClickedHandler();
    void clearLineEdit();
    void timerTimeout();

    void getNamesSlot (QNetworkReply *preply);
    void loginSlot(QNetworkReply *loginReply);

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // ADDPIN_H
