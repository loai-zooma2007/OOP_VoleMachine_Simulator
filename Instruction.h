#pragma once
#include <string>
class CPU;
// ==========================================
// Abstract Base Class
// ==========================================
class instruction {
protected:
    int word;
public:
    instruction(int word);
    virtual void execute(CPU& cpu) = 0;
    virtual std::string describe() const = 0;
    virtual ~instruction() = default;
};
// ==========================================
// Logical Instructions
// ==========================================
class orinstruction : public instruction {
public:
    orinstruction(int word);
    void execute(CPU& cpu) override;
    std::string describe() const override;
};
class andinstruction : public instruction {
public:
    andinstruction(int word);
    void execute(CPU& cpu) override;
    std::string describe() const override;
};
class xorinstruction : public instruction {
public:
    xorinstruction(int word);
    void execute(CPU& cpu) override;
    std::string describe() const override;
};
// ==========================================
// Control Instructions
// ==========================================
class rotate : public instruction {
public:
    rotate(int word);
    void execute(CPU& cpu) override;
    std::string describe() const override;
};
class jumpifEqual : public instruction {
public:
    jumpifEqual(int word);
    void execute(CPU& cpu) override;
    std::string describe() const override;
};
class halt : public instruction {
public:
    halt(int word);
    void execute(CPU& cpu) override;
    std::string describe() const override;
};
// ==========================================
// Memory & Data Transfer Instructions
// ==========================================
class loadfrommemory : public instruction {
public:
    loadfrommemory(int word);
    void execute(CPU& cpu) override;
    std::string describe() const override;
};
class loadimmediate : public instruction {
public:
    loadimmediate(int word);
    void execute(CPU& cpu) override;
    std::string describe() const override;
};
class store : public instruction {
public:
    store(int word);
    void execute(CPU& cpu) override;
    std::string describe() const override;
};
class move : public instruction {
public:
    move(int word);
    void execute(CPU& cpu) override;
    std::string describe() const override;
};
// ==========================================
// Arithmetic Instructions
// ==========================================
class addinteger : public instruction {
public:
    addinteger(int word);
    void execute(CPU& cpu) override;
    std::string describe() const override;
};
class addfloat : public instruction {
public:
    addfloat(int word);
    void execute(CPU& cpu) override;
    std::string describe() const override;
};