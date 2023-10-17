#include "Cpu.hxx"

void Cpu::setRegisters(RegisterCollection *value)
{
    registers = value;
}

void Cpu::setMemory(Memory *value)
{
    memory = value;
}

void Cpu::setPpu(Ppu *value)
{
    ppu = value;
}

unsigned_four_byte Cpu::fetch()
{
    unsigned_two_byte currentOpcode = memory->readMemory(programCounter);
    programCounter++;
    return currentOpcode;
}

void Cpu::interrupt()
{
    if (ime && memory->readMemory(0xFFFF)) {
            if (memory->readMemory(0xFFFF) & 1 && memory->readMemory(0xFF0F) & 1) {
                interruptJump(0x40, 0);
            }
            if (memory->readMemory(0xFFFF) & 2 && memory->readMemory(0xFF0F) & 2) {
                interruptJump(0x48, 1);
            }
            if (memory->readMemory(0xFFFF) & 4 && memory->readMemory(0xFF0F) & 4) {
                interruptJump(0x50, 2);
            }
            if (memory->readMemory(0xFFFF) & 8 && memory->readMemory(0xFF0F) & 8) {
                interruptJump(0x58, 3);
            }
            if (memory->readMemory(0xFFFF) & 16 && memory->readMemory(0xFF0F) & 16) {
                interruptJump(0x60, 4);
            }
        }
}
