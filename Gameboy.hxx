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

    int *viewport = new int[160 * 144];
    char *debugStringFull = new char[0];
    int debugStringLength = 0;
    int *debugBackgroundMap = new int[65536];
    int *debugTileMap = new int[24576];

public:
    Gameboy();
    ~Gameboy();

    void readRom(uintptr_t arrayBuffer, int size);
    void initialize();
    void mainLoop();

    uintptr_t getViewPort();

    uintptr_t getBackground();
    uintptr_t getTileMap();
    void setBackgroundSettings(std::string backgroundAddress, std::string tilemapAddress);

    void getNextInstructionDebugLine();
    uintptr_t getDebugStringFull();
    void emptyDebugString();
    int getDebugStringLength();
};

EMSCRIPTEN_BINDINGS(gameboy)
{
    class_<Gameboy>("Gameboy")
        .constructor()
        .function("readRom", &Gameboy::readRom)
        .function("initialize", &Gameboy::initialize)
        .function("mainLoop", &Gameboy::mainLoop)
        .function("getViewPort", &Gameboy::getViewPort)
        .function("getBackground", &Gameboy::getBackground)
        .function("getTileMap", &Gameboy::getTileMap)
        .function("setBackgroundSettings", &Gameboy::setBackgroundSettings)
        .function("getDebugStringFull", &Gameboy::getDebugStringFull)
        .function("getDebugStringLength", &Gameboy::getDebugStringLength)
        .function("emptyDebugString", &Gameboy::emptyDebugString);
}