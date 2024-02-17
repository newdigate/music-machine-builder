#ifndef SKETCHFACTORY_H
#define SKETCHFACTORY_H

#include <dlfcn.h>
#include <iostream>
#include <memory>
#include <stdexcept>

using SketchBase_creator_t = void (*)();

class SketchFactory {
public:
    SketchFactory() : handler(nullptr), setup_loader(nullptr) {
    }

    bool loadSketch(const char* compiledSketchSOfilename) {
        if (handler) dlclose(handler);
        handler = dlopen(compiledSketchSOfilename, RTLD_NOW);
        if (! handler) {
            throw std::runtime_error(dlerror());
        }
        Reset_dlerror();
        setup_loader = reinterpret_cast<SketchBase_creator_t>(dlsym(handler, "setup"));
        Check_dlerror();

        return (setup_loader != nullptr);
    }

    void setup() const {
        if (handler && setup_loader)
            setup_loader();
        else
            std::cout << " ERROR: Sketch not loaded" << std::endl;
    }

    ~SketchFactory() {
        if (handler) {
            dlclose(handler);
        }
    }

private:
    void * handler;
    SketchBase_creator_t setup_loader;

    static void Reset_dlerror() {
        dlerror();
    }

    static void Check_dlerror() {
        const char * dlsym_error = dlerror();
        if (dlsym_error) {
            throw std::runtime_error(dlsym_error);
        }
    }
};



#endif //SKETCHFACTORY_H
