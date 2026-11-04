#include <iostream>
#include <vector>
#include <set>
#include <map>
#include "Instruction.h"
#include <cstring>
using namespace std;



class MemoryMappedDevice
{
    private:
    
    
    public:
    virtual void onWrite(int value) = 0;
};

class Screen :public MemoryMappedDevice
{
private:
    string buffer;
public:
    void onWrite(int value){
        buffer += char(value);
    }
    string getText(){return buffer;}
    void clear(){
        buffer = "";
    }
};


class Memory{
private:
    int cells[256];
    map <int, MemoryMappedDevice*> device;

public:
    void writeBlock(int start, int data[], int size){
        for (int i = 0; i < size; i++)
        {
            cells[i + start] = data[i];
            
        }
        

    }

    void write(int addr, int v){
        if (device.find(addr) != device.end())
        {
            device[addr]->onWrite(v);
        }
        else
        {
            cells[addr] = v;
        }
        

    }

    int read(int addr){
        return cells[addr];
    }

    void mapDevice(int addr, MemoryMappedDevice* d){
        device[addr] = d;
    }

    void clear(){
        for (int i = 0; i < 256; i++)
        {
            cells[i] = 0;
        }
        
    }

};

class InstructionDecoder
{
public:
    instruction* decode(int word){
        if (!validate(word))
            return nullptr;
        int opcode = word / 4096;
        if (opcode == 1)
            return new loadfrommemory(word);
        if (opcode == 2)
            return new loadimmediate(word);
        if (opcode == 3)
            return new store(word);
        if (opcode == 4)
            return new moveinstruction(word);
        if (opcode == 5)
            return new addinteger(word);
        if (opcode == 6)
            return new addfloat(word);
        if (opcode == 7)
            return new orinstruction(word);
        if (opcode == 8)
            return new andinstruction(word);
        if (opcode == 9)
            return new xorinstruction(word);
        if (opcode == 10)
            return new rotate(word);
        if (opcode == 11)
            return new jumpifEqual(word);
        if (opcode == 12)
            return new halt(word);
        return nullptr;
    }
    bool validate(int word){
        return word >= 0 && word <= 65535; // FFFF (Hex) = 65535 (Dec)
    }

};

int main()
{
    
}