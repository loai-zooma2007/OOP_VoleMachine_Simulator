#ifndef _MACHINE_HPP_
#define _MACHINE_HPP_
#include "ProgramLoader.h"
#include <string>
#include <map>
#include <cstdint>
#include <cmath>

class Instruction;

class Register
{
private:
    std::uint8_t value;

public:
    Register();
    int get();
    void set(int v);
};
class RegisterFile
{
private:
    Register regs[16];

public:
    int get(int n);
    void set(int n, int v);
    void clear();
};

class FloatingPoint8
{
public:
    double decode(int bits);
    int encode(double value);
};
class ALU
{
private:
    FloatingPoint8 f8;

public:
    int addTwosComplement(int a, int b);
    int addFloat(int a, int b);
    int bitOr(int a, int b);
    int bitAnd(int a, int b);
    int bitXor(int a, int b);
    int rotateRight(int v, int times);
};

class MemoryMappedDevice
{
public:
    virtual void onWrite(int value) = 0;
};
class Screen : public MemoryMappedDevice
{
private:
    std::string buffer;

public:
    Screen();
    virtual void onWrite(int value);
    std::string getText();
    void clear();
};

class Memory
{
private:
    std::uint8_t cells[256]{0};
    std::map<int, MemoryMappedDevice *> device;

public:
    int read(int addr);
    void write(int addr, int v);
    void writeBlock(int start, int data[], int Size);
    void mapDevice(int addr, MemoryMappedDevice *d);
    void clear();
};
class CPU
{
private:
    int PC;
    int IR;
    bool halted;

    RegisterFile &regfl;
    Memory &memory;

    ALU alu;
    InstructionDecoder decoder;

public:
    CPU(RegisterFile &reg, Memory &mem);
    void fetch();
    Instruction *decode();
    void execute(Instruction *i);
    void step();
    RegisterFile &getRegisters();
    Memory &getMemory();
    ALU &getALU();
    void setPC(int addr);
    int getPC();
    int getIR();
    bool isHalted();
    void halt(bool h);
    void jumpTo(int addr);
};
class VoleMachine
{
private:
    Memory memory;
    RegisterFile registers;
    Screen screen;
    CPU cpu;

public:
    VoleMachine();
    void loadProgram(std::string path, int start);
    void setRegister(int n, int v);
    void setMemory(int addr, int v);
    bool step();
    void runAll();
    void reset();

    RegisterFile &getRegisters();
    Memory &getMemory();
    Screen &getScreen();
    CPU &getCPU();
};

#endif