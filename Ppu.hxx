#ifndef PPU_H
#define PPU_H

#include <emscripten/emscripten.h>
#include <emscripten/bind.h>

using namespace emscripten;

class Ppu
{
private:
    /* data */
public:
    Ppu(/* args */);
    ~Ppu();
};

EMSCRIPTEN_BINDINGS(ppu)
{
    class_<Ppu>("Ppu")
    .constructor()
    ;
}
#endif