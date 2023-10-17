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

two_byte Cpu::fetch()
{
    two_byte currentOpcode = memory->readMemory(programCounter);
    programCounter++;
    return currentOpcode;
}
