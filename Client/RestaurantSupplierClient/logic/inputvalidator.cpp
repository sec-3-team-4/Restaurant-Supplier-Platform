#include "inputvalidator.h"

ValidationResult InputValidator::validateRegister(const QString& username,
                                                  const QString& password,
                                                  const QString& role)
{
    if (username.trimmed().isEmpty())
    {
        return ValidationResult::fail("Username cannot be empty.");
    }

    if (password.trimmed().isEmpty())
    {
        return ValidationResult::fail("Password cannot be empty.");
    }

    if (role != "restaurant" && role != "supplier")
    {
        return ValidationResult::fail("Role must be either restaurant or supplier.");
    }

    return ValidationResult::ok();
}

ValidationResult InputValidator::validateOrderRequest(const QString& sender,
                                                      const QString& receiver,
                                                      const QString& product,
                                                      int quantity,
                                                      const QString& unit)
{
    if (sender.trimmed().isEmpty())
    {
        return ValidationResult::fail("Sender cannot be empty.");
    }

    if (receiver.trimmed().isEmpty())
    {
        return ValidationResult::fail("Receiver cannot be empty.");
    }

    if (product.trimmed().isEmpty())
    {
        return ValidationResult::fail("Product cannot be empty.");
    }

    if (quantity <= 0)
    {
        return ValidationResult::fail("Quantity must be greater than 0.");
    }

    if (unit.trimmed().isEmpty())
    {
        return ValidationResult::fail("Unit cannot be empty.");
    }

    return ValidationResult::ok();
}

ValidationResult InputValidator::validateProductInquiry(const QString& sender,
                                                        const QString& receiver,
                                                        const QString& product,
                                                        const QString& question)
{
    if (sender.trimmed().isEmpty())
    {
        return ValidationResult::fail("Sender cannot be empty.");
    }

    if (receiver.trimmed().isEmpty())
    {
        return ValidationResult::fail("Receiver cannot be empty.");
    }

    if (product.trimmed().isEmpty())
    {
        return ValidationResult::fail("Product cannot be empty.");
    }

    if (question.trimmed().isEmpty())
    {
        return ValidationResult::fail("Question cannot be empty.");
    }

    return ValidationResult::ok();
}