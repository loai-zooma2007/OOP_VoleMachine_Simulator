#include "Machine.hpp"

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
            cells[i + start] = data[i];
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

// InstructionDecoder
Instruction InstructionDecoder::decode(int word)
{
}
bool InstructionDecoder::validate(int word)
{
    return word >= 0 && word <= 65535;
}

// register classes
Register::Register() { value = 0; }
int Register::get() { return value; }
void Register::set(int v) { value = v; }

int RegisterFile::get(int n)
{
    if (n < 0 || n > 16)
        return 0;
    return regs[n].get();
}
void RegisterFile::set(int n, int v)
{
    if (n < 0 || n > 16)
        return;
    return regs[n].set(v);
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

    int sign = (bits & 0x80) ? 1 : -1;
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
        else if (Manti<0.5 & exp> - 4)
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

// vole machine class
VoleMachine::VoleMachine() { memory.mapDevice(0xff, &screen); }
void VoleMachine::loadProgram(std::string path, int start)
{
    // loading package

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
}
