#ifndef MESSAGESERVICE_H
#define MESSAGESERVICE_H

#include <QString>

#include "inetworkclient.h"
#include "inputvalidator.h"
#include "messagebuilder.h"

class MessageService
{
public:
    explicit MessageService(INetworkClient* networkClient);

    bool sendOrderRequest(const QString& sender,
                          const QString& receiver,
                          const QString& product,
                          int quantity,
                          const QString& unit,
                          double requestedPrice);

private:
    INetworkClient* networkClient;
};

#endif