#ifndef PROGRAMLOADER_H
#define PROGRAMLOADER_H
#include <vector>
#include "Instruction.h"
class Program
{
private:
    std::vector<int> words;

public:
    explicit Program(const std::vector<int> &words) : words(words) {}
    const std::vector<int> &getWords() const { return words; }
    int Size() const { return static_cast<int>(words.size()); }
};
class ProgramLoader
{
public:
    Program load(const std::string &path) const;           // throws VoleException subclasses
    void validateStartAddress(int start, int count) const; // throws InvalidAddressException

private:
    std::string parseLine(const std::string &line) const; // strips comments / whitespace
    InstructionDecoder decoder;
};
#endif // PROGRAMLOADER_H