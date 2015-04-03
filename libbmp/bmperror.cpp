#include "bmperror.h"

BMPError::BMPError()
{
}

BMPError::BMPError(const char *msg, const char *desc)
{
    this->msg = std::string(msg);
    if(desc) {
        this->desc = std::string(desc);
    }
}

BMPError::BMPError(const std::string &msg, const std::string &desc)
{
    this->msg = msg;
    this->desc = desc;
}

std::string BMPError::text()
{
    return this->msg;
}

std::string BMPError::description()
{
    return this->desc;
}
