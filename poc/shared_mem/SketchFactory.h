#ifndef SKETCHFACTORY_H
#define SKETCHFACTORY_H

#include <dlfcn.h>
#include <iostream>
#include <stdexcept>

typedef void (*fp_void_no_parameters)();

class SketchFactory {
public:
    SketchFactory() : handler(nullptr), fp_setup(nullptr) {
    }

    bool loadSketch(const char* compiledSketchSOfilename) {
        if (handler) dlclose(handler);
        handler = dlopen(compiledSketchSOfilename, RTLD_NOW);
        if (! handler) {
            throw std::runtime_error(dlerror());
        }
        Reset_dlerror();
        fp_setup = reinterpret_cast<fp_void_no_parameters>(dlsym(handler, "setup"));
        Check_dlerror();

        return (fp_setup != nullptr);
    }

    void setup() const {
        if (handler && fp_setup)
            fp_setup();
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
    fp_void_no_parameters fp_setup;

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
