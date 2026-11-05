#include "ProgramLoader.h"
#include "Errors.h"
#include "Memory.h"
#include <cctype>
#include <cstdlib>
#include <fstream>

std::string ProgramLoader::parseLine(const std::string &line) const
{
    std::string s = line.substr(0, line.find(';'));
    std::size_t b = 0, e = s.size();
    while (b < e && std::isspace(static_cast<unsigned char>(s[b])))
        ++b;
    while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1])))
        --e;
    return s.substr(b, e - b);
}

Program ProgramLoader::load(const std::string &path) const
{
    std::ifstream in(path.c_str());
    if (!in)
        throw ProgramFormatException("Cannot open file: " + path);

    std::vector<int> words;
    std::string line;
    int lineNo = 0;
    while (std::getline(in, line))
    {
        ++lineNo;
        std::string text = parseLine(line);
        if (text.empty())
            continue;

        bool ok = text.size() == 4;
        for (std::size_t i = 0; ok && i < text.size(); ++i)
            ok = std::isxdigit(static_cast<unsigned char>(text[i])) != 0;
        if (!ok)
            throw ProgramFormatException("Line " + std::to_string(lineNo) +
                                         ": expected exactly 4 hex digits, found '" + text + "'");

        int word = static_cast<int>(std::strtol(text.c_str(), nullptr, 16));
        if (!decoder.validate(word))
            throw InvalidInstructionException("Line " + std::to_string(lineNo) +
                                              ": invalid instruction " + text);
        words.push_back(word);
    }
    if (words.empty())
        throw ProgramFormatException("The program file contains no instructions");
    return Program(words);
}

void ProgramLoader::validateStartAddress(int start, int count) const
{
    if (start < 0 || start > 0xF2 || start % 2 != 0)
        throw InvalidAddressException("Start address must be even and between 00 and F2");
    if (start + 2 * count > Memory::SIZE)
        throw InvalidAddressException("The program does not fit in memory at that address");
}
