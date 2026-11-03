#include "Instruction.h"
// ==========================================
// Abstract Base Class
// ==========================================
instruction::instruction(int word) {
    this->word = word;
}
// ==========================================
// Logical Instructions
// ==========================================
orinstruction::orinstruction(int word)
    : instruction(word) {
}
std::string orinstruction::describe() const {
    return "OR Instruction";
}
andinstruction::andinstruction(int word)
    : instruction(word) {
}
std::string andinstruction::describe() const {
    return "AND Instruction";
}
xorinstruction::xorinstruction(int word)
    : instruction(word) {
}
std::string xorinstruction::describe() const {
    return "XOR Instruction";
}
// ==========================================
// Control Instructions
// ==========================================
rotate::rotate(int word)
    : instruction(word) {
}
std::string rotate::describe() const {
    return "Rotate Instruction";
}
jumpifEqual::jumpifEqual(int word)
    : instruction(word) {
}
std::string jumpifEqual::describe() const {
    return "Jump If Equal Instruction";
}
halt::halt(int word)
    : instruction(word) {
}
std::string halt::describe() const {
    return "Halt Instruction";
}
// ==========================================
// Memory & Data Transfer Instructions
// ==========================================
loadfrommemory::loadfrommemory(int word)
    : instruction(word) {
}
std::string loadfrommemory::describe() const {
    return "Load From Memory Instruction";
}
loadimmediate::loadimmediate(int word)
    : instruction(word) {
}
std::string loadimmediate::describe() const {
    return "Load Immediate Instruction";
}
store::store(int word)
    : instruction(word) {
}
std::string store::describe() const {
    return "Store Instruction";
}
move::move(int word)
    : instruction(word) {
}
std::string move::describe() const {
    return "Move Instruction";
}
// ==========================================
// Arithmetic Instructions
// ==========================================
addinteger::addinteger(int word)
    : instruction(word) {
}
std::string addinteger::describe() const {
    return "Add Integer Instruction";
}
addfloat::addfloat(int word)
    : instruction(word) {
}
std::string addfloat::describe() const {
    return "Add Float Instruction";
}