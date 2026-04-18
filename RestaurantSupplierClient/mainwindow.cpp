#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chatclient.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_client(new ChatClient(this))
{
    ui->setupUi(this);

    ui->chatDisplay->setReadOnly(true);

    ui->senderLineEdit->setText("jana");
    ui->receiverLineEdit->setText("supplier_user");

    connect(ui->sendButton, &QPushButton::clicked,
            this, &MainWindow::onSendClicked);

    connect(m_client, &ChatClient::connected,
            this, &MainWindow::onConnected);

    connect(m_client, &ChatClient::disconnected,
            this, &MainWindow::onDisconnected);

    connect(m_client, &ChatClient::errorOccurred,
            this, &MainWindow::onErrorOccurred);

    connect(m_client, &ChatClient::chatMessageReceived,
            this, &MainWindow::onChatMessageReceived);

    connect(m_client, &ChatClient::rawJsonReceived,
            this, &MainWindow::onRawJsonReceived);

    updateConnectionStatus("Connecting...");
    m_client->connectToServer("127.0.0.1", 12345);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSendClicked()
{
    QString sender = ui->senderLineEdit->text().trimmed();
    QString receiver = ui->receiverLineEdit->text().trimmed();
    QString text = ui->messageLineEdit->text().trimmed();

    if (sender.isEmpty() || receiver.isEmpty() || text.isEmpty())
    {
        updateConnectionStatus("Validation Error");
        appendMessage("[Error] Sender, receiver, and message must not be empty.");
        return;
    }

    m_client->sendChatMessage(sender, receiver, text);
    appendMessage(sender + ": " + text);
    ui->messageLineEdit->clear();
}

void MainWindow::onConnected()
{
    updateConnectionStatus("Connected");
    appendMessage("[System] Connected to server.");
}

void MainWindow::onDisconnected()
{
    updateConnectionStatus("Disconnected");
    appendMessage("[System] Disconnected from server.");
}

void MainWindow::onErrorOccurred(const QString &message)
{
    updateConnectionStatus("Error");
    appendMessage("[Error] " + message);
}

void MainWindow::onChatMessageReceived(const QString &sender, const QString &text)
{
    appendMessage(sender + ": " + text);
}

void MainWindow::onRawJsonReceived(const QString &jsonText)
{
    appendMessage("[JSON] " + jsonText);
}

void MainWindow::appendMessage(const QString &message)
{
    ui->chatDisplay->append(message);
}

void MainWindow::updateConnectionStatus(const QString &statusText)
{
    ui->statusLabel->setText("Status: " + statusText);
}