#ifndef INPUTVALIDATOR_H
#define INPUTVALIDATOR_H

#include <QString>
#include "validationresult.h"

class InputValidator
{
public:
    static ValidationResult validateRegister(const QString& username,
                                             const QString& password,
                                             const QString& role);

    static ValidationResult validateOrderRequest(const QString& sender,
                                                 const QString& receiver,
                                                 const QString& product,
                                                 int quantity,
                                                 const QString& unit);

    static ValidationResult validateProductInquiry(const QString& sender,
                                                   const QString& receiver,
                                                   const QString& product,
                                                   const QString& question);
};

#endif // INPUTVALIDATOR_H