#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 1234);

    qDebug() << "Connecting to server...";

    connect(socket, &QTcpSocket::connected, this, [=]() {
        ui->connectionLabel->setText("Connected");
        ui->connectionLabel->setStyleSheet("color: green;");
    });

    connect(socket, &QTcpSocket::disconnected, this, [=]() {
        ui->connectionLabel->setText("Disconnected");
        ui->connectionLabel->setStyleSheet("color: red;");
    });

    connect(socket, &QTcpSocket::readyRead, this, [=]() {
        QByteArray responseData = socket->readAll();

        QJsonDocument doc = QJsonDocument::fromJson(responseData);
        QJsonObject response = doc.object();

        handleResponse(response);
    });

    connect(ui->loginButton, &QPushButton::clicked,
            this, &MainWindow::handleLogin);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleLogin()
{
    QString username = ui->usernameInput->text();
    QString password = ui->passwordInput->text();

    if (username.isEmpty() || password.isEmpty()) {
        ui->statusLabel->setText("Please fill all fields");
        return;
    }

    ui->statusLabel->setText("Logging in...");
    ui->loginButton->setEnabled(false);

    sendLoginRequest(username, password);
}

void MainWindow::sendLoginRequest(const QString &username, const QString &password)
{
    QJsonObject data;
    data["username"] = username;
    data["password"] = password;

    QJsonObject request;
    request["type"] = "login_request";
    request["sender"] = username;
    request["receiver"] = "server";
    request["data"] = data;

    QJsonDocument doc(request);
    socket->write(doc.toJson(QJsonDocument::Compact));

    qDebug() << "Sent login request";
}

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

void MainWindow::updateConnectionStatus(bool connected)
{
    if (connected) {
        ui->connectionLabel->setText("Connected");
        ui->connectionLabel->setStyleSheet("color: green;");
    } else {
        ui->connectionLabel->setText("Disconnected");
        ui->connectionLabel->setStyleSheet("color: red;");
    }
}
