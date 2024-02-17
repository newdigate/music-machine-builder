#include "SketchFactory.h"

int main()
{
    SketchFactory factory;

    if (!factory.loadSketch("libTestSketch.dylib") ) {
        std::cout << "Failed to load sketch!" << std::endl;
        return -1;
    }

    factory.setup();

    return 0;
    //base->foo();
}