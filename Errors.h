#ifndef ERRORS_H
#define ERRORS_H

#include <stdexcept>
#include <string>

// Base class for every error the simulator can raise.
class VoleException : public std::runtime_error
{
public:
    explicit VoleException(const std::string &msg) : std::runtime_error(msg) {}
};

class InvalidInstructionException : public VoleException
{
public:
    explicit InvalidInstructionException(const std::string &msg) : VoleException(msg) {}
};

class InvalidAddressException : public VoleException
{
public:
    explicit InvalidAddressException(const std::string &msg) : VoleException(msg) {}
};

class ProgramFormatException : public VoleException
{
public:
    explicit ProgramFormatException(const std::string &msg) : VoleException(msg) {}
};
#endif // ERRORS_H
