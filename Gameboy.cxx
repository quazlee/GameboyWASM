#include "Gameboy.hxx"
#include <fstream>
#include <iostream>
#include <vector>
#include <emscripten/val.h>
#include <string>
#include <emscripten.h>
using emscripten::val;
using namespace emscripten;

Gameboy::Gameboy()
{
}

Gameboy::~Gameboy()
{
}

void Gameboy::readRom(uintptr_t arrayBuffer, int size)
{
    auto romContents = reinterpret_cast<unsigned_two_byte *>(arrayBuffer);
    memory->initialize(romContents, size);
}

void Gameboy::initialize()
{
    cpu->setMemory(memory);
    cpu->setPpu(ppu);

    ppu->setMemory(memory);
}

void Gameboy::mainLoop()
{
    while (!ppu->getIsFrameReady())
    {
        cpu->interrupt();

        // this.debug.logger();

        cpu->execute();
        // this.controls.updateInputState();
    }
}

uintptr_t Gameboy::getBackground()
{
    // EM_JS(EM_VAL, find_myinput, (), {
    //     let input = document.getElementById('myinput');
    //     return Emval.toHandle(input);
    // });
    // const val document = val::global("document");
    // val canvas = document.call<val>("getElementById", "background-canvas");
    // val ctx = canvas.call<val>("getContext", "2d");
    // return
    std::vector<int> data = ppu->populateBackgroundWindowMaps();
    int* arr = new int[65536];
    std::copy(data.begin(), data.end(), arr);
    return uintptr_t(arr);
}

void Gameboy::setBackgroundSettings(std::string backgroundAddress, std::string tilemapAddress)
{
    unsigned_four_byte backgroundAddressConverted = std::stoi(backgroundAddress, 0, 16);
    unsigned_four_byte tilemapAddressConverted = std::stoi(tilemapAddress, 0, 16);

    ppu->setDebugAddresses(backgroundAddressConverted, tilemapAddressConverted);
}
