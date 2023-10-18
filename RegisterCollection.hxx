#ifndef REGISTERCOLLECTION_H
#define REGISTERCOLLECTION_H

#include "Types.hxx"

#include <emscripten/emscripten.h>
#include <emscripten/bind.h>

using namespace emscripten;

class RegisterCollection
{
private:
    /* data */
public:
    RegisterCollection(/* args */);
    ~RegisterCollection();  
};

EMSCRIPTEN_BINDINGS(registercollection)
{
    class_<RegisterCollection>("RegisterCollection")
    .constructor()
    ;
}
#endif