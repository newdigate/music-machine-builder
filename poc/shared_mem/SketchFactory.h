#ifndef SKETCHFACTORY_H
#define SKETCHFACTORY_H

#include <dlfcn.h>
#include <memory>
#include <stdexcept>

using SketchBase_creator_t = void (*)();

class SketchFactory {
public:
    SketchFactory() {
        handler = dlopen("libTestSketch.dylib", RTLD_NOW);
        if (! handler) {
            throw std::runtime_error(dlerror());
        }
        Reset_dlerror();
        setup_loader = reinterpret_cast<SketchBase_creator_t>(dlsym(handler, "setup"));
        Check_dlerror();
    }

    void setup() const {
        setup_loader();
    }

    ~SketchFactory() {
        if (handler) {
            dlclose(handler);
        }
    }

private:
    void * handler = nullptr;
    SketchBase_creator_t setup_loader = nullptr;

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
