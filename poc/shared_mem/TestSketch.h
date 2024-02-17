#ifndef TESTSKETCH_H
#define TESTSKETCH_H

#include <iostream>

#include "SketchBase.h"

extern "C" int magicNumber;
int i = 10;

void setup() {
    std::cout << "setup();" << std::endl;
}

class Derived: public SketchBase {
public:
    explicit Derived() {
        std::cout << magicNumber << std::endl;
    }

    void foo() const override {
        std::cout << "Hello World" << " " << i << std::endl;
        setup();
    }
};

extern "C" {
    SketchBase * create() {
        return new Derived();
    }
}

#endif //TESTSKETCH_H
