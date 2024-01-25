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
    Cpu *cpu = new Cpu();
    Ppu *ppu = new Ppu();
    Memory *memory = new Memory();

    char* debugStringFull = new char [0];

public:
    Gameboy();
    ~Gameboy();

    void readRom(uintptr_t arrayBuffer, int size);
    void initialize();
    void mainLoop();

    uintptr_t getBackground();
    void setBackgroundSettings(std::string backgroundAddress, std::string tilemapAddress);

    void getNextInstructionDebugLine();
    uintptr_t getDebugStringFull();
    void emptyDebugString();
};

EMSCRIPTEN_BINDINGS(gameboy)
{
    class_<Gameboy>("Gameboy")
        .constructor()
        .function("readRom", &Gameboy::readRom)
        .function("initialize", &Gameboy::initialize)
        .function("mainLoop", &Gameboy::mainLoop)
        .function("getBackground", &Gameboy::getBackground)
        .function("setBackgroundSettings", &Gameboy::setBackgroundSettings)
        .function("getDebugStringFull", &Gameboy::getDebugStringFull)
        .function("emptyDebugString", &Gameboy::emptyDebugString);
}