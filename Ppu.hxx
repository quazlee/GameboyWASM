#pragma once

class Ppu
{
private:
    //Determines if the next frame is ready to be drawn. Will be true after scanline 153 is complete.
    bool isFrameReady = false;

public:
    Ppu(/* args */);
    ~Ppu();

    bool getIsFrameReady();
    void setIsFrameReady(bool isReady);

    //General function that drives the ppu. One pass of each mode lasts two clock cycles.
    void cycle();
};
