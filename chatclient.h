#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include "inetworkclient.h" // Add this!
#include <QTcpSocket>
#include <QJsonDocument>

// Change QObject to INetworkClient
class ChatClient : public INetworkClient
{
    Q_OBJECT
public:
    explicit ChatClient(QObject *parent = nullptr);

    // Override the interface functions
    void connectToServer(const QString &host, quint16 port) override;
    void sendMessage(const QJsonObject &json) override;

private slots:
    void onReadyRead();

private:
    QTcpSocket *m_socket;
};

#endif // CHATCLIENT_H