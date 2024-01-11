#include "Gameboy.hxx"
#include <iostream>

class Cpu;

int main(int argc, char const *argv[]){
    std::cout << argv[1];
    Gameboy* gameboy = new Gameboy();
    gameboy->readRom();
    
    std::cout << "test";
    return 0;
}