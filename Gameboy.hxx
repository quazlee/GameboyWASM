#pragma once

#include "Cpu.hxx"
#include "Ppu.hxx"
#include "Memory.hxx"
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>

using namespace emscripten;

class Gameboy
{
private:
    Cpu* cpu = new Cpu();
    Ppu* ppu = new Ppu();
    Memory* memory = new Memory();
public:
    Gameboy();
    ~Gameboy();

    void readRom();
    void initialize();
    void mainLoop();
};

EMSCRIPTEN_BINDINGS(gameboy)
{
    class_<Gameboy>("Gameboy")
    .constructor()
    .function("readRom", &Gameboy::readRom)
    .function("initialize", &Gameboy::initialize)
    .function("mainLoop", &Gameboy::mainLoop);
}