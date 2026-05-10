#ifndef INETWORKCLIENT_H
#define INETWORKCLIENT_H

#include <QObject>
#include <QJsonObject>

// This is an Abstract Interface
class INetworkClient : public QObject {
    Q_OBJECT
public:
    explicit INetworkClient(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~INetworkClient() {}

    // Pure virtual functions (the "Contract")
    virtual void connectToServer(const QString &host, quint16 port) = 0;
    virtual void sendMessage(const QJsonObject &json) = 0;

signals:
    void messageReceived(const QJsonObject &json);
    void connected();
    void disconnected();
};

#endif // INETWORKCLIENT_H