#include "Machine.h"

// screen class
Screen::Screen() { buffer = ""; }
void Screen::onWrite(int value)
{
    buffer += static_cast<char>(value);
}
std::string Screen::getText() { return buffer; }
void Screen::clear() { buffer = ""; }

// memory class
int Memory::read(int addr)
{
    if (addr < 0 || addr > 255)
        return 0;

    return cells[addr];
}
void Memory::write(int addr, int v)
{
    if (addr < 0 || addr > 255)
        return;

    auto it = device.find(addr);

    if (it != device.end() && it->second != nullptr)
    {
        it->second->onWrite(v);
    }
    cells[addr] = v;
}
void Memory::writeBlock(int start, int data[], int Size)
{
    for (int i = 0; i < Size; i++)
    {
        if (start + i < 256)
            write(start + i, data[i]);
    }
}
void Memory::mapDevice(int addr, MemoryMappedDevice *d)
{
    if (addr < 0 || addr > 255)
        return;

    device[addr] = d;
}
void Memory::clear()
{
    for (int i = 0; i < 256; i++)
    {
        cells[i] = 0;
    }
}

// register classes
Register::Register() { value = 0; }
int Register::get() { return value; }
void Register::set(int v) { value = v; }

int RegisterFile::get(int n)
{
    if (n < 0 || n >= 16)
        return 0;
    return regs[n].get();
}
void RegisterFile::set(int n, int v)
{
    if (n < 0 || n >= 16)
        return;
    regs[n].set(v);
}
void RegisterFile::clear()
{
    for (int i = 0; i < 16; i++)
    {
        regs[i].set(0);
    }
}

// ALU classes
double FloatingPoint8::decode(int bits)
{
    if (bits == 0)
        return 0.0;

    int sign = (bits & 0x80) ? -1 : 1;
    int exp = ((bits & 0x70) >> 4) - 4;
    double Manti = (bits & 0x0f) / 16.0;

    return sign * Manti * std::pow(2.0, exp);
}
int FloatingPoint8::encode(double value)
{
    if (value == 0.0)
        return 0x00;

    int sign = (value > 0) ? 0 : 1;
    int exp = 0;
    double Manti = std::abs(value);

    while (true)
    {
        if (Manti >= 1.0)
        {
            Manti /= 2.0;
            exp++;
        }
        else if (Manti < 0.5 && exp > -4)
        {
            Manti *= 2.0;
            exp--;
        }
        else
            break;
    }
    int bitsExp = (exp + 4) & 0x07;
    int bitsManti = (static_cast<int>(Manti * 16.0)) & 0x0f;

    return (sign << 7) | (bitsExp << 4) | bitsManti;
}
int ALU::addTwosComplement(int a, int b)
{
    return (a + b) & 0xff;
}
int ALU::addFloat(int a, int b)
{
    double valA = f8.decode(a);
    double valB = f8.decode(b);

    return f8.encode(valA + valB);
}
int ALU::bitOr(int a, int b)
{
    return (a | b) & 0xff;
}
int ALU::bitAnd(int a, int b)
{
    return (a & b) & 0xff;
}
int ALU::bitXor(int a, int b)
{
    return (a ^ b) & 0xff;
}
int ALU::rotateRight(int v, int times)
{
    times %= 8;
    return ((v >> times) | (v << (8 - times))) & 0xff;
}

// CPU class
CPU::CPU(RegisterFile &reg, Memory &mem) : regfl(reg), memory(mem), PC(0), IR(0), halted(false) {}
void CPU::fetch()
{
    if (halted)
        return;
    int byte1 = memory.read(PC);
    int byte2 = memory.read(PC + 1);
    IR = (byte1 << 8) | byte2;
    PC = (PC + 2) & 0xff;
}
Instruction *CPU::decode()
{
    return decoder.decode(IR);
}
void CPU::execute(Instruction *i)
{
    if (i != nullptr)
    {
        i->execute(*this);
        delete i;
    }
}
void CPU::step()
{
    if (!halted)
    {
        fetch();
        Instruction *i = decode();
        execute(i);
    }
}
RegisterFile &CPU::getRegisters()
{
    return regfl;
}
Memory &CPU::getMemory()
{
    return memory;
}
ALU &CPU::getALU()
{
    return alu;
}
void CPU::setPC(int addr)
{
    PC = addr & 0xff;
}
int CPU::getPC()
{
    return PC;
}
int CPU::getIR()
{
    return IR;
}
bool CPU::isHalted()
{
    return halted;
}
void CPU::halt(bool h)
{
    halted = h;
}
void CPU::jumpTo(int addr)
{
    setPC(addr);
}

// vole machine class
VoleMachine::VoleMachine() : memory(), registers(), screen(), cpu(registers, memory) { memory.mapDevice(0xff, &screen); }
void VoleMachine::loadProgram(std::string path, int start)
{
    ProgramLoader loader;
    Program program = loader.load(path);

    loader.validateStartAddress(start, program.Size());

    int addr = start;
    for (int word : program.getWords())
    {
        int highByte = (word >> 8) & 0xff;
        int lowByte = word & 0xff;

        memory.write(addr, highByte);
        memory.write(addr + 1, lowByte);
        addr += 2;
    }

    cpu.setPC(start);
    cpu.halt(false);
}
void VoleMachine::setRegister(int n, int v)
{
    registers.set(n, v);
}
void VoleMachine::setMemory(int addr, int v)
{
    memory.write(addr, v);
}
bool VoleMachine::step()
{
    if (cpu.isHalted())
        return false;
    cpu.step();
    return !cpu.isHalted();
}
void VoleMachine::runAll()
{
    while (!cpu.isHalted())
        cpu.step();
}
void VoleMachine::reset()
{
    memory.clear();
    registers.clear();
    screen.clear();
    cpu.setPC(0);
    cpu.halt(false);
}

RegisterFile &VoleMachine::getRegisters()
{
    return registers;
}
Memory &VoleMachine::getMemory()
{
    return memory;
}
Screen &VoleMachine::getScreen()
{
    return screen;
}
CPU &VoleMachine::getCPU()
{
    return cpu;
}
