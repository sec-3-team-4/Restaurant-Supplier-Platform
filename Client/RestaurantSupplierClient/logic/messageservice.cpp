#include "messageservice.h"

MessageService::MessageService(INetworkClient* networkClient)
    : networkClient(networkClient)
{
}

bool MessageService::sendOrderRequest(const QString& sender,
                                      const QString& receiver,
                                      const QString& product,
                                      int quantity,
                                      const QString& unit,
                                      double requestedPrice)
{
    ValidationResult result =
        InputValidator::validateOrderRequest(sender, receiver, product, quantity, unit);

    if (!result.success)
    {
        return false;
    }

    QJsonObject json =
        MessageBuilder::buildOrderRequest(sender,
                                          receiver,
                                          product,
                                          quantity,
                                          unit,
                                          requestedPrice);

    QByteArray payload = MessageBuilder::toByteArray(json);

    return networkClient->sendMessage(payload);
}