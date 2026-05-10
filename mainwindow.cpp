#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonObject>
#include <QJsonDocument>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. Initialize Jana's Chat Client
    chatClient = new ChatClient(this);

    // 2. CONNECT SIGNALS: Listen for the "Connected" event
    // This updates your UI when the networking brain finds the server
    connect(chatClient, &ChatClient::connected, this, [this]() {
        // Change the "Connecting..." label to Green and show success
        ui->statusLabel->setText("Status: Connected ✅");
        ui->statusLabel->setStyleSheet("color: green; font-weight: bold;");
        ui->chatDisplay_1->addItem("System: Link to server established.");
    });

    // Handle disconnection or server failure
    connect(chatClient, &ChatClient::disconnected, this, [this]() {
        ui->statusLabel->setText("Status: Disconnected ❌");
        ui->statusLabel->setStyleSheet("color: red;");
        ui->chatDisplay_1->addItem("System: Connection lost.");
    });

    // 3. RECEIVE MESSAGES: Put incoming server data into Chat History
    connect(chatClient, &ChatClient::messageReceived, this, [this](const QJsonObject &json) {
        QString sender = json["sender"].toString();
        QString text = json["data"].toObject()["text"].toString();

        // This adds the message from the server to your big left box
        ui->chatDisplay_1->addItem(sender + ": " + text);
    });

    // 4. START CONNECTION: Tell the socket where to go
    // Make sure Malak's server is running on this port!
    chatClient->connectToServer("10.40.41.183", 1234);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 5. SEND BUTTON: The core task for Milestone 3
void MainWindow::on_sendButton_clicked()
{
    QString senderName = ui->senderLineEdit->text();
    QString receiverName = ui->receiverLineEdit->text();
    QString msgText = ui->messageLineEdit->text();

    // Don't send empty messages
    if (msgText.isEmpty() || senderName.isEmpty()) return;

    // Build the JSON based on Joudy's schema
    QJsonObject message;
    message["type"] = "chat_message";
    message["sender"] = senderName;
    message["receiver"] = receiverName;

    QJsonObject data;
    data["text"] = msgText;
    message["data"] = data;

    // Send it through the socket
    chatClient->sendMessage(message);

    // Update local history so user sees their own text
    ui->chatDisplay_1->addItem(senderName + " (Me): " + msgText);

    // Clear the message box for the next text
    ui->messageLineEdit->clear();
}