#include "Instruction.h"
#include "Machine.hpp"
// ==========================================
// Abstract Base Class
// ==========================================
Instruction::Instruction(int word)
{
    this->word = word;
}
// ==========================================
// Logical Instructions
// ==========================================
orInstruction::orInstruction(int word)
    : Instruction(word)
{
}
std::string orInstruction::describe() const
{
    return "OR Instruction";
}
void orInstruction::execute(CPU &cpu)
{
    int r = (word >> 8) & 0xF;
    int s = (word >> 4) & 0xF;
    int t = word & 0xF;

    int value1 = cpu.getRegisters().get(s);
    int value2 = cpu.getRegisters().get(t);

    int result = cpu.getALU().bitOr(value1, value2);

    cpu.getRegisters().set(r, result);
}
andInstruction::andInstruction(int word)
    : Instruction(word)
{
}
std::string andInstruction::describe() const
{
    return "AND Instruction";
}
void andInstruction::execute(CPU &cpu)
{
    int r = (word >> 8) & 0xF;
    int s = (word >> 4) & 0xF;
    int t = word & 0xF;

    int value1 = cpu.getRegisters().get(s);
    int value2 = cpu.getRegisters().get(t);

    int result = cpu.getALU().bitAnd(value1, value2);

    cpu.getRegisters().set(r, result);
}
xorInstruction::xorInstruction(int word)
    : Instruction(word)
{
}
std::string xorInstruction::describe() const
{
    return "XOR Instruction";
}
void xorInstruction::execute(CPU &cpu)
{
    int r = (word >> 8) & 0xF;
    int s = (word >> 4) & 0xF;
    int t = word & 0xF;

    int value1 = cpu.getRegisters().get(s);
    int value2 = cpu.getRegisters().get(t);

    int result = cpu.getALU().bitXor(value1, value2);

    cpu.getRegisters().set(r, result);
}
// ==========================================
// Control Instructions
// ==========================================
rotate::rotate(int word)
    : Instruction(word)
{
}
std::string rotate::describe() const
{
    return "Rotate Instruction";
}
void rotate::execute(CPU &cpu)
{
    int r = (word >> 8) & 0xF;
    int times = word & 0xF;

    int value = cpu.getRegisters().get(r);

    int result = cpu.getALU().rotateRight(value, times);

    cpu.getRegisters().set(r, result);
}
jumpifEqual::jumpifEqual(int word)
    : Instruction(word)
{
}
std::string jumpifEqual::describe() const
{
    return "Jump If Equal Instruction";
}
void jumpifEqual::execute(CPU &cpu)
{
    int r = (word >> 8) & 0xF;
    int address = word & 0xFF;

    int value1 = cpu.getRegisters().get(r);
    int value2 = cpu.getRegisters().get(0);

    if (value1 == value2)
    {
        cpu.jumpTo(address);
    }
}
halt::halt(int word)
    : Instruction(word)
{
}
std::string halt::describe() const
{
    return "Halt Instruction";
}
void halt::execute(CPU &cpu)
{
    cpu.halt(true);
}
// ==========================================
// Memory & Data Transfer Instructions
// ==========================================
loadfrommemory::loadfrommemory(int word)
    : Instruction(word)
{
}

std::string loadfrommemory::describe() const
{
    return "Load From Memory Instruction";
}

void loadfrommemory::execute(CPU &cpu)
{
    int r = (word >> 8) & 0xF;
    int address = word & 0xFF;

    int value = cpu.getMemory().read(address);
    cpu.getRegisters().set(r, value);
}
loadimmediate::loadimmediate(int word)
    : Instruction(word)
{
}
std::string loadimmediate::describe() const
{
    return "Load Immediate Instruction";
}
void loadimmediate::execute(CPU &cpu)
{
    int r = (word >> 8) & 0xF;
    int value = word & 0xFF;

    cpu.getRegisters().set(r, value);
}
store::store(int word)
    : Instruction(word)
{
}
std::string store::describe() const
{
    return "Store Instruction";
}
void store::execute(CPU &cpu)
{
    int r = (word >> 8) & 0xF;
    int address = word & 0xFF;

    int value = cpu.getRegisters().get(r);

    cpu.getMemory().write(address, value);
}
moveinstruction::moveinstruction(int word)
    : Instruction(word)
{
}
std::string moveinstruction::describe() const
{
    return "Move Instruction";
}
void moveinstruction::execute(CPU &cpu)
{
    int r = (word >> 4) & 0xF;
    int s = word & 0xF;

    int value = cpu.getRegisters().get(s);

    cpu.getRegisters().set(r, value);
}
// ==========================================
// Arithmetic Instructions
// ==========================================
addinteger::addinteger(int word)
    : Instruction(word)
{
}
std::string addinteger::describe() const
{
    return "Add Integer Instruction";
}
void addinteger::execute(CPU &cpu)
{
    int r = (word >> 8) & 0xF;
    int s = (word >> 4) & 0xF;
    int t = word & 0xF;

    int value1 = cpu.getRegisters().get(s);
    int value2 = cpu.getRegisters().get(t);

    int result = cpu.getALU().addTwosComplement(value1, value2);

    cpu.getRegisters().set(r, result);
}
addfloat::addfloat(int word)
    : Instruction(word)
{
}
std::string addfloat::describe() const
{
    return "Add Float Instruction";
}
void addfloat::execute(CPU &cpu)
{
    int r = (word >> 8) & 0xF;
    int s = (word >> 4) & 0xF;
    int t = word & 0xF;

    int value1 = cpu.getRegisters().get(s);
    int value2 = cpu.getRegisters().get(t);

    int result = cpu.getALU().addFloat(value1, value2);

    cpu.getRegisters().set(r, result);
}