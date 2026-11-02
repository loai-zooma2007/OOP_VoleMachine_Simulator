#include "Machine.hpp"

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
