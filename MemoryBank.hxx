#ifndef MEMORYBANK_H
#define MEMORYBANK_H

#include <vector>
#include "Types.hxx"

#include <emscripten/emscripten.h>
#include <emscripten/bind.h>

using namespace emscripten;

class MemoryBank
{
private:
    /* data */
    int size;
    std::vector<unsigned_two_byte>* data;
public:
    MemoryBank(/* args */);
    ~MemoryBank();
    unsigned_two_byte getData(int location);
    void setData(int location, unsigned_two_byte value);
};

EMSCRIPTEN_BINDINGS(memorybank)
{
    class_<MemoryBank>("MemoryBank")
    .constructor()
    .function("getData", &Cpu::getData)
    .function("setData", &Cpu::setData)
    ;
}
#endif