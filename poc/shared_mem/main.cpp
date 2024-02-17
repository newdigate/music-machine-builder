#include "SketchBase.h"
#include "SketchFactory.h"

int main()
{
    SketchFactory factory;
    uint16_t framebuffer[128][126];

    std::unique_ptr<SketchBase> base = factory.create();
    base->foo();
}