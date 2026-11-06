#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>

class CPU;

// Abstract Base Class

class Instruction
{
protected:
    int word;

public:
    Instruction(int word);

    virtual void execute(CPU &cpu) = 0;

    virtual std::string describe() const = 0;

    virtual ~Instruction() = default;
};
// ==========================================
// Logical Instructions
// ==========================================
class orInstruction : public Instruction
{
public:
    orInstruction(int word);
    void execute(CPU &cpu) override;
    std::string describe() const override;
};
class andInstruction : public Instruction
{
public:
    andInstruction(int word);
    void execute(CPU &cpu) override;
    std::string describe() const override;
};
class xorInstruction : public Instruction
{
public:
    xorInstruction(int word);
    void execute(CPU &cpu) override;
    std::string describe() const override;
};
// ==========================================
// Control Instructions
// ==========================================
class rotate : public Instruction
{
public:
    rotate(int word);
    void execute(CPU &cpu) override;
    std::string describe() const override;
};
class jumpifEqual : public Instruction
{
public:
    jumpifEqual(int word);
    void execute(CPU &cpu) override;
    std::string describe() const override;
};
class halt : public Instruction
{
public:
    halt(int word);
    void execute(CPU &cpu) override;
    std::string describe() const override;
};
// ==========================================
// Memory & Data Transfer Instructions
// ==========================================
class loadfrommemory : public Instruction
{
public:
    loadfrommemory(int word);
    void execute(CPU &cpu) override;
    std::string describe() const override;
};
class loadimmediate : public Instruction
{
public:
    loadimmediate(int word);
    void execute(CPU &cpu) override;
    std::string describe() const override;
};
class store : public Instruction
{
public:
    store(int word);
    void execute(CPU &cpu) override;
    std::string describe() const override;
};
class moveinstruction : public Instruction
{
public:
    moveinstruction(int word);
    void execute(CPU &cpu) override;
    std::string describe() const override;
};
// ==========================================
// Arithmetic Instructions
// ==========================================
class addinteger : public Instruction
{
public:
    addinteger(int word);
    void execute(CPU &cpu) override;
    std::string describe() const override;
};
class addfloat : public Instruction
{
public:
    addfloat(int word);
    void execute(CPU &cpu) override;
    std::string describe() const override;
};
class InstructionDecoder
{
public:
    Instruction *decode(int word);
    bool validate(int word) const;
};
#endif // INSTRUCTION_H