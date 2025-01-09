#include "addPin.h"
#include "ui_addPin.h"
#include "mainuserinterface.h"
#include "ui_mainUserInterface.h"
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QProcess>
#include <QCloseEvent>
#include <QtSql>
#include <QPixmap>
#include <QLabel>

addPin::addPin(QString cardNumber, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::addPin)
    , cardNumber(cardNumber)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QImage("C:/bank.background.jpg")));
    this->setPalette(palette);

    // Join QPushButton::clicked-signal to  handlePinInsert-slot
    connect(ui->pinSubmit,SIGNAL(clicked(bool)),
        this,SLOT(handlePinInsert()));

    // Connect signals and slots
    connect(ui->n0, &QPushButton::clicked, this, &addPin::numberClickedHandler);
    connect(ui->n1, &QPushButton::clicked, this, &addPin::numberClickedHandler);
    connect(ui->n2, &QPushButton::clicked, this, &addPin::numberClickedHandler);
    connect(ui->n3, &QPushButton::clicked, this, &addPin::numberClickedHandler);
    connect(ui->n4, &QPushButton::clicked, this, &addPin::numberClickedHandler);
    connect(ui->n5, &QPushButton::clicked, this, &addPin::numberClickedHandler);
    connect(ui->n6, &QPushButton::clicked, this, &addPin::numberClickedHandler);
    connect(ui->n7, &QPushButton::clicked, this, &addPin::numberClickedHandler);
    connect(ui->n8, &QPushButton::clicked, this, &addPin::numberClickedHandler);
    connect(ui->n9, &QPushButton::clicked, this, &addPin::numberClickedHandler);
    connect(ui->clear, &QPushButton::clicked, this, &addPin::clearLineEdit);

    // Create timer which closes addPin.ui and opens  MainWindow.ui after 60 seconds
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &addPin::timerTimeout);
    timer->start(60000);  
}

addPin::~addPin()
{
    delete ui;
}

void addPin::clearLineEdit()
{
    ui->pinLine->clear();
}

void addPin::timerTimeout()
{
    qDebug() << "Time has run out";

    // Close the app
    qApp->quit();

    // Restarts the application
    QProcess::startDetached(QApplication::applicationFilePath());
}

void addPin::closeEvent(QCloseEvent *event)
{
    // Stop the timer when addPin.ui closes
    timer->stop();
    event->accept();
}

void addPin::numberClickedHandler()
{
    // Cast the clicked numbers into the pinLine text field
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString number = button->text();
        ui->pinLine->setText(ui->pinLine->text() + number);
    }
}

void addPin::handlePinInsert()
{
    QString correctCardNumber = cardNumber;
    QString pinNumber = ui->pinLine->text();
    QJsonObject jsonObj;
    jsonObj.insert("card_number",correctCardNumber);
    jsonObj.insert("pin",pinNumber);

    QString site_url="http://localhost:3000/login";
    QNetworkRequest request((site_url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    loginManager = new QNetworkAccessManager(this);
    connect(loginManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(loginSlot(QNetworkReply*)));

    loginReply = loginManager->post(request, QJsonDocument(jsonObj).toJson());
}

void addPin::loginSlot(QNetworkReply *loginReply)
{
    loginResponse_data = loginReply->readAll();
    // The value of webToken is set here
    webToken = loginResponse_data;
    QMessageBox msgBox;

    if(loginResponse_data.length()==0){

        msgBox.setText("Error in the connection");
        msgBox.exec();
    }
    else{
        if(loginResponse_data!="false"){

        qDebug() << "Right PIN";

        //If REST-API changes url, the change is needed here also
        QString site_url="http://localhost:3000/users/user";
        QNetworkRequest request((site_url));

        // WEBTOKEN START
        // Upon successful login, the variable webToken is assigned a value,
        // which is of type QByteArray, prefixed with the string "Bearer".
        QByteArray myToken="Bearer "+loginResponse_data;
        request.setRawHeader(QByteArray("Authorization"),(myToken));
        // WEBTOKEN END

        pgetManager = new QNetworkAccessManager(this);
        connect(pgetManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getNamesSlot(QNetworkReply*)));
        preply = pgetManager->get(request);

    } else {
        ui->insertPinLabel->setText("Väärä PIN-koodi");
        // Counter for incorrectly entered PIN codes
        wrongPinAttempts++;

        // Check if the maximum number of incorrect PIN code attempts has been reached
        if (wrongPinAttempts >= maxWrongAttempts) {
            // Prevent the submitting of PIN
            // By disabling "OK" button
            ui->insertPinLabel->setText("Liian monta väärää yritystä.");

            ui->pinSubmit->setEnabled(false);
        }
        // Print the number of incorrectly entered PIN codes as a debug message
        qDebug() << "Number of incorrectly entered PIN codes: " << wrongPinAttempts;
    }
    loginReply->deleteLater();
    loginManager->deleteLater();
 }
}

void addPin::getNamesSlot(QNetworkReply *preply)
{
    response_data=preply->readAll();
    //qDebug()<<"DATA : "+response_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonArray json_array = json_doc.array();

    QString cName;
    foreach (const QJsonValue &value, json_array) {
        QJsonObject json_obj = value.toObject();

        if (cardNumber == "-0600062093") {
            QString fname = json_obj["fname"].toString();
            QString lname = json_obj["lname"].toString();
            cName = "Hei " + fname + " " + lname + "!";
            break; // The loop is terminated once the desired user is found
        } else if (cardNumber == "-06000621FE")
        {
            QString fname = json_obj["fname"].toString();
            QString lname = json_obj["lname"].toString();
            cName = "Hei " + fname + " " + lname + "!";
        }
    }

    // Create a new window and user interface object
    mainUserInterface *mainUserInterfaceWindow = new mainUserInterface(webToken, cardNumber);

    QLabel *customerName = mainUserInterfaceWindow->findChild<QLabel*>("customerName");
    if (customerName) {
        customerName->setText(cName);
    }

    mainUserInterfaceWindow->show();

    // Close the current window
    this->close();

    preply->deleteLater();
    pgetManager->deleteLater();
}
