#include "Ppu.hxx"

Ppu::Ppu()
{
}

Ppu::~Ppu()
{
}

bool Ppu::getIsFrameReady()
{
    return isFrameReady;
}

void Ppu::setIsFrameReady(bool isReady)
{
    isFrameReady = isReady;
}

void Ppu::cycle()
{
    // if (mode == 2)
    // {
    //     modeTwo();
    // }
    // else if (mode == 3)
    // {
    //     modeThree();
    // }
    // else if (mode == 0)
    // {
    //     modeZero();
    // }
    // else if (mode == 1)
    // {
    //     modeOne();
    // }
}
