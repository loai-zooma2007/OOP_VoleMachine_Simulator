#include <iostream>
#include <vector>
#include <set>
#include <map>
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
    Instruction decode(int word){}
    bool validate(int word){
        return word >= 0 && word <= 65535; // FFFF (Hex) = 65535 (Dec)
    }

};

int main()
{
    
}