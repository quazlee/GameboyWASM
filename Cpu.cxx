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