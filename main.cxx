#include "Gameboy.hxx"
#include <iostream>

class Cpu;
int main(){
    Gameboy* gameboy = new Gameboy();
    gameboy->readRom();
    
    std::cout << "test";
    return 0;
}