#include "Gameboy.hxx"
#include <fstream>
#include <iostream>
#include <vector>
#include <emscripten/val.h>
#include <string>
#include <emscripten.h>
#include "Types.hxx"
#include <sstream>
using emscripten::val;
using namespace emscripten;

#define formatter2 std::uppercase << std::setfill('0') << std::setw(2) << std::hex
#define formatter4 std::uppercase << std::setfill('0') << std::setw(4) << std::hex

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
        getNextInstructionDebugLine();
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
    int *arr = new int[65536];
    std::copy(data.begin(), data.end(), arr);
    return uintptr_t(arr);
}

void Gameboy::setBackgroundSettings(std::string backgroundAddress, std::string tilemapAddress)
{
    unsigned_four_byte backgroundAddressConverted = std::stoi(backgroundAddress, 0, 16);
    unsigned_four_byte tilemapAddressConverted = std::stoi(tilemapAddress, 0, 16);

    ppu->setDebugAddresses(backgroundAddressConverted, tilemapAddressConverted);
}

void Gameboy::getNextInstructionDebugLine()
{
    std::stringstream debugString;
    debugString << "A: " << formatter2 << static_cast<int>(cpu->getRegisters()->getRegister(registerID::A)) << " "
                << "F: " << formatter2 << static_cast<int>(cpu->getRegisters()->getRegister(registerID::F)) << " "
                << "B: " << formatter2 << static_cast<int>(cpu->getRegisters()->getRegister(registerID::B)) << " "
                << "C: " << formatter2 << static_cast<int>(cpu->getRegisters()->getRegister(registerID::C)) << " "
                << "D: " << formatter2 << static_cast<int>(cpu->getRegisters()->getRegister(registerID::D)) << " "
                << "E: " << formatter2 << static_cast<int>(cpu->getRegisters()->getRegister(registerID::E)) << " "
                << "H: " << formatter2 << static_cast<int>(cpu->getRegisters()->getRegister(registerID::H)) << " "
                << "L: " << formatter2 << static_cast<int>(cpu->getRegisters()->getRegister(registerID::L)) << " "
                << "SP: " << formatter4 << static_cast<int>(cpu->getStackPointer()) << " "
                << "PC: 00:" << formatter4 << static_cast<int>(cpu->getProgramCounter()) << " "
                << "(" << formatter2 << static_cast<int>(memory->readMemory(cpu->getProgramCounter())) << " "
                << formatter2 << static_cast<int>(memory->readMemory(cpu->getProgramCounter() + 1)) << " "
                << formatter2 << static_cast<int>(memory->readMemory(cpu->getProgramCounter() + 2)) << " "
                << formatter2 << static_cast<int>(memory->readMemory(cpu->getProgramCounter() + 3)) << ")"
                << std::endl;

    std::string oldString = std::string(debugStringFull);
    delete[] debugStringFull;
    std::string currentString = debugString.str();
    std::string newString = oldString + currentString;
    debugStringFull = new char[newString.length() + 1];
    strcpy(debugStringFull, newString.c_str());
}

uintptr_t Gameboy::getDebugStringFull()
{
    return uintptr_t(debugStringFull);
}
