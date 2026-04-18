#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonDocument>
#include <QJsonObject>
<<<<<<< HEAD
=======
#include <QJsonDocument>
#include <QTcpSocket>
>>>>>>> 1f77c50 (Update login integration with socket communication)
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

<<<<<<< HEAD
    socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 1234);
=======
    // Create and connect socket (same as Jana's code)
    socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 1234);

    qDebug() << "Connecting to server...";

    // Update connection status
    connect(socket, &QTcpSocket::connected, this, [=]() {
        ui->connectionLabel->setText("Connected");
        ui->connectionLabel->setStyleSheet("color: green;");
        qDebug() << "Connected to server";
    });

    connect(socket, &QTcpSocket::disconnected, this, [=]() {
        ui->connectionLabel->setText("Disconnected");
        ui->connectionLabel->setStyleSheet("color: red;");
        qDebug() << "Disconnected from server";
    });

    // Receive server response
    connect(socket, &QTcpSocket::readyRead, this, [=]() {

        QByteArray responseData = socket->readAll();
        qDebug() << "Received:" << responseData;

        QJsonDocument doc = QJsonDocument::fromJson(responseData);
        QJsonObject response = doc.object();

        handleResponse(response);
    });

    // Connect login button
    connect(ui->loginButton, &QPushButton::clicked,
            this, &MainWindow::handleLogin);
>>>>>>> 1f77c50 (Update login integration with socket communication)

    qDebug() << "Connected to server";
}

MainWindow::~MainWindow()
{
    delete ui;
}

<<<<<<< HEAD
void MainWindow::on_searchButton_clicked()
=======
// Handle login button
void MainWindow::handleLogin()
>>>>>>> 1f77c50 (Update login integration with socket communication)
{
    QJsonObject obj;
    obj["type"] = "search_request";
    obj["sender"] = "malak";

<<<<<<< HEAD
=======
    if (username.isEmpty() || password.isEmpty()) {
        ui->statusLabel->setText("Please fill all fields");
        return;
    }

    ui->statusLabel->setText("Logging in...");
    ui->loginButton->setEnabled(false);

    sendLoginRequest(username, password);
}

//Send login request to server
void MainWindow::sendLoginRequest(const QString &username, const QString &password)
{
>>>>>>> 1f77c50 (Update login integration with socket communication)
    QJsonObject data;
    data["category"] = ui->lineEditCategory->text();

<<<<<<< HEAD
    obj["data"] = data;

    QJsonDocument doc(obj);
    socket->write(doc.toJson(QJsonDocument::Compact));

    qDebug() << "Sent:" << doc.toJson(QJsonDocument::Compact);
}
=======
    QJsonObject request;
    request["type"] = "login_request";
    request["sender"] = username;
    request["receiver"] = "server";
    request["data"] = data;

    QJsonDocument doc(request);

    socket->write(doc.toJson(QJsonDocument::Compact));

    qDebug() << "Sent login request:" << doc.toJson(QJsonDocument::Compact);
}

// Handle server response
void MainWindow::handleResponse(const QJsonObject &response)
{
    QString status = response["status"].toString();

    if (status == "success") {
        ui->statusLabel->setText("Login successful");
        ui->statusLabel->setStyleSheet("color: green;");
    } else {
        QString message = response["message"].toString();
        if (message.isEmpty())
            message = "Login failed";

        ui->statusLabel->setText(message);
        ui->statusLabel->setStyleSheet("color: red;");
    }

    ui->loginButton->setEnabled(true);
}
>>>>>>> 1f77c50 (Update login integration with socket communication)
