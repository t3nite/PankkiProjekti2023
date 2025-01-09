#ifndef CDCHOICE_H
#define CDCHOICE_H

#include <QWidget>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>

namespace Ui {
class cdChoice;
}

class cdChoice : public QWidget
{
    Q_OBJECT

public:
    explicit cdChoice(QString cardNumber,QWidget *parent = nullptr);
    ~cdChoice();

private:
    Ui::cdChoice *ui;
    QTimer *timer2;

    QString cardNumber;

private slots:
    void creditClicked();
    void debitClicked();
    void timerOut();

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // CDCHOICE_H
