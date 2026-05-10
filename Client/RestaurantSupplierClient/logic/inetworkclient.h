#ifndef INETWORKCLIENT_H
#define INETWORKCLIENT_H

#include <QByteArray>

class INetworkClient
{
public:
    virtual ~INetworkClient() = default;
    virtual bool sendMessage(const QByteArray& payload) = 0;
};

#endif