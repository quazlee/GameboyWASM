#pragma once

class Ppu
{
private:
    /* data */
    bool isFrameReady = false;

public:
    Ppu(/* args */);
    ~Ppu();

    bool getIsFrameReady();
    void setIsFrameReady(bool isReady);
};
