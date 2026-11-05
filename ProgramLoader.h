#ifndef PROGRAMLOADER_H
#define PROGRAMLOADER_H
#include <string>
#include "InstructionDecoder.h"
#include "Program.h"

// Reads a program file and validates it. Never touches memory itself.
class ProgramLoader
{
public:
    Program load(const std::string &path) const;           // throws VoleException subclasses
    void validateStartAddress(int start, int count) const; // throws InvalidAddressException

private:
    std::string parseLine(const std::string &line) const; // strips comments / whitespace
    InstructionDecoder decoder;
};
endif // PROGRAMLOADER_H