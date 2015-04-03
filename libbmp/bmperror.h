#ifndef BMPERROR_H
#define BMPERROR_H

#include <string>

class BMPError
{
    std::string msg;
    std::string desc;

public:
    BMPError();
    BMPError(const char *msg, const char *desc = NULL);
    BMPError(const std::string &msg, const std::string &desc = std::string());
    std::string text();
    std::string description();
};

#endif // BMPERROR_H
