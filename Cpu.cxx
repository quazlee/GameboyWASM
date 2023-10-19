#include "Cpu.hxx"

void Cpu::setMemory(Memory *value)
{
    memory = value;
}

void Cpu::setPpu(Ppu *value)
{
    ppu = value;
}

Cpu::Cpu()
{
}

Cpu::~Cpu()
{
}

unsigned_two_byte Cpu::fetch()
{
    unsigned_two_byte currentOpcode = memory->readMemory(programCounter);
    programCounter++;
    return currentOpcode;
}

void Cpu::execute()
{
    unsigned_two_byte currentOpcode = fetch();
    switch (currentOpcode >> 4)
    {
    case 0x0:
        switch (currentOpcode & 0xF)
        {
        case 0x0:
        {
            tickClock(4);
            break;
        }
        case 0x1:
        {
            unsigned_two_byte low = fetch();
            unsigned_two_byte high = fetch();
            registers.setRegisterDouble(registerID::B, registerID::C, high, low);
            tickClock(12);
            break;
        }
        case 0x2:
        {
            int location = registers.getRegisterDouble(registerID::B, registerID::C);
            unsigned_two_byte value = registers.getRegister(registerID::A);
            memory->writeMemory(location, value);
            tickClock(8);
            break;
        }
        case 0x3:
        {
            registers.incRegisterDouble(registerID::B, registerID::C);
            tickClock(8);
            break;
        }
        case 0x4:
        {
            registers.incRegister(registerID::B);
            tickClock(4);
            break;
        }
        case 0x5:
        {
            registers.decRegister(registerID::B);
            tickClock(4);
            break;
        }
        case 0x6:
        {
            registers.setRegister(registerID::B, fetch());
            tickClock(8);
            break;
        }
        case 0x7:
        {
            registers.rotateLeftCircularA();
            tickClock(4);
            break;
        }
        case 0x8:
        {
            unsigned_two_byte low = fetch();
            unsigned_two_byte high = fetch();
            unsigned_four_byte location = (high << 8) | low;
            unsigned_two_byte lowStack = stackPointer & 0xFF;
            unsigned_two_byte highStack = stackPointer >> 8;
            memory.writeMemory(location, lowStack);
            memory.writeMemory(location + 1, highStack);
            tickClock(20);
            break;
        }
        case 0x9:
        {
            registers.addHL(registers.getRegisterDouble(registerID::B, registerID::C));
            tickClock(8);
            break;
        }
        case 0xA:
        {
            unsigned_two_byte value = memory.readMemory(registers.getRegisterDouble(registerID::B, registerID::C));
            registers.setRegister(registerID::A, value);
            tickClock(8);
            break;
        }
        case 0xB:
        {
            registers.decRegisterDouble(registerID::B, registerID::C);
            tickClock(8);
            break;
        }
        case 0xC:
        {
            registers.incRegister(registerID::C);
            tickClock(4);
            break;
        }
        case 0xD:
        {
            registers.decRegister(registerID::C);
            tickClock(4);
            break;
        }
        case 0xE:
        {
            registers.setRegister(registerID::C, fetch());
            tickClock(8);
            break;
        }
        case 0xF:
        {
            registers.rotateRightCircularA();
            tickClock(4);
            break;
        }
        }
        break;
    case 0x1:
        switch (currentOpcode & 0xF)
        {
        case 0x0:
        { // TODO: STOP
            tickClock(4);
            break;
        }
        case 0x1:
        {
            unsigned_two_byte low = fetch();
            unsigned_two_byte high = fetch();
            registers.setRegisterDouble(registerID::D, registerID::E, high, low);
            tickClock(12);
            break;
        }
        case 0x3:
        {
            registers.incRegisterDouble(registerID::D, registerID::E);
            tickClock(8);
            break;
        }
        case 0x4:
        {
            registers.incRegister(registerID::D);
            tickClock(4);
            break;
        }
        case 0x5:
        {
            registers.decRegister(registerID::D);
            tickClock(4);
            break;
        }
        case 0x6:
        {
            registers.setRegister(registerID::D, fetch());
            tickClock(8);
            break;
        }
        case 0x7:
        {
            registers.rotateLeftA();
            tickClock(4);
            break;
        }
        case 0x8:
        {
            unsigned_two_byte jumpOffset = fetch();
            programCounter += static_cast<two_byte>(jumpOffset);
            tickClock(12);
            break;
        }
        case 0x9:
        {
            registers.addHL(registers.getRegisterDouble(registerID::D, registerID::E));
            tickClock(8);
            break;
        }
        case 0xA:
        {
            unsigned_two_byte value = memory.readMemory(registers.getRegisterDouble(registerID::D, registerID::E));
            registers.setRegister(registerID::A, value);
            tickClock(8);
            break;
        }
        case 0xB:
        {
            registers.decRegisterDouble(registerID::D, registerID::E);
            tickClock(8);
            break;
        }
        case 0xC:
        {
            registers.incRegister(registerID::E);
            tickClock(4);
            break;
        }
        case 0xD:
        {
            registers.decRegister(registerID::E);
            tickClock(4);
            break;
        }
        case 0xE:
        {
            registers.setRegister(registerID::E, fetch());
            tickClock(8);
            break;
        }
        case 0xF:
        {
            registers.rotateRightA();
            tickClock(4);
            break;
        }
        }
        break;
    case 0x2:
        switch (low)
        {
        }
        break;
    case 0x3:
        break;
    case 0x4:
        break;
    case 0x5:
        break;
    case 0x6:
        break;
    case 0x7:
        break;
    case 0x8:
        break;
    case 0x9:
        break;
    case 0xA:
        break;
    case 0xB:
        break;
    case 0xC:
        break;
    case 0xD:
        break;
    case 0xE:
        break;
    case 0xF:
        break;
    }
}

void Cpu::interrupt()
{
    if (ime && memory->readMemory(0xFFFF))
    {
        if (memory->readMemory(0xFFFF) & 1 && memory->readMemory(0xFF0F) & 1)
        {
            interruptJump(0x40, 0);
        }
        if (memory->readMemory(0xFFFF) & 2 && memory->readMemory(0xFF0F) & 2)
        {
            interruptJump(0x48, 1);
        }
        if (memory->readMemory(0xFFFF) & 4 && memory->readMemory(0xFF0F) & 4)
        {
            interruptJump(0x50, 2);
        }
        if (memory->readMemory(0xFFFF) & 8 && memory->readMemory(0xFF0F) & 8)
        {
            interruptJump(0x58, 3);
        }
        if (memory->readMemory(0xFFFF) & 16 && memory->readMemory(0xFF0F) & 16)
        {
            interruptJump(0x60, 4);
        }
    }
}

void Cpu::tickClock(int numTicks)
{
}

void Cpu::interruptJump(unsigned_four_byte location, int flag)
{
    stackPointer--;
    memory->writeMemory(stackPointer, (programCounter >> 8));
    stackPointer--;

    memory->writeMemory(stackPointer, (programCounter & 0xff));
    programCounter = location;
    memory->writeMemory(0xFF0F, memory->readMemory(0xFF0F) & ~(1 << flag));
    tickClock(20);
}
