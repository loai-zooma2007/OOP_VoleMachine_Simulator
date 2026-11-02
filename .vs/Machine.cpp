#include "Machine.hpp"

// screen class
void Screen::onWrite(int value)
{
    buffer += static_cast<char>(value);
}
std::string Screen::getText() { return buffer; }
void Screen::clear() { buffer = ""; }

// memory class
int Memory::read(int addr)
{
    return cells[addr];
}
void Memory::write(int addr, int v)
{
    if (device.find(addr) != device.end())
    {
        device[addr]->onWrite(v);
    }
    else
    {
        cells[addr] = v;
    }
}
void Memory::writeBlock(int start, int data[], int Size)
{
    for (int i = 0; i < Size; i++)
    {
        cells[i + start] = data[i];
    }
}
void Memory::mapDevice(int addr, MemoryMappedDevice *d)
{
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
Instruction instructionDecoder::decode(int word)
{
}
bool instructionDecoder::validate(int word)
{
    return word >= 0 && word <= 65535;
}

// vole machine class
void VoleMachine::loadProgram(std::string path, int start)
{
    // Load program into memory
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
