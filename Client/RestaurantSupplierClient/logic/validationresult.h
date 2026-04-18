#ifndef VALIDATIONRESULT_H
#define VALIDATIONRESULT_H

#include <QString>

struct ValidationResult
{
    bool success;
    QString errorMessage;

    static ValidationResult ok()
    {
        return {true, ""};
    }

    static ValidationResult fail(const QString& message)
    {
        return {false, message};
    }
};

#endif // VALIDATIONRESULT_H