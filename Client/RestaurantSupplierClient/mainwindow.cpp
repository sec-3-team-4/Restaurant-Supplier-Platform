#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonObject>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect login button (MAKE SURE objectName is loginButton in .ui)
    connect(ui->loginButton, &QPushButton::clicked,
            this, &MainWindow::handleLogin);

    ui->statusLabel->setText("");
    ui->connectionLabel->setText("Disconnected");
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

    // FAKE SERVER RESPONSE (until networking is ready)
    QTimer::singleShot(1000, this, [=]() {

        QJsonObject response;
        response["type"] = "login_response";

        if (username == "admin" && password == "1234") {
            response["status"] = "success";
        } else {
            response["status"] = "error";
            response["message"] = "Invalid username or password";
        }

        ui->loginButton->setEnabled(true);
        handleResponse(response);
    });
}

void MainWindow::sendLoginRequest(const QString &username, const QString &password)
{
    QJsonObject data;
    data["username"] = username;
    data["password"] = password;

    QJsonObject request;
    request["type"] = "login_request";
    request["sender"] = "client";
    request["receiver"] = "server";
    request["data"] = data;

    qDebug() << "Login request created";
}

void MainWindow::handleResponse(const QJsonObject &response)
{
    QString status = response["status"].toString();

    if (status == "success") {
        ui->statusLabel->setText("Login successful");
        ui->statusLabel->setStyleSheet("color: green;");
    } else {
        ui->statusLabel->setText(response["message"].toString());
        ui->statusLabel->setStyleSheet("color: red;");
    }
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