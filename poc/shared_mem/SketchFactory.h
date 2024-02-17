#ifndef SKETCHFACTORY_H
#define SKETCHFACTORY_H

#include <dlfcn.h>
#include <memory>
#include <stdexcept>

#include "SketchBase.h"

class SketchFactory {
public:
    SketchFactory() {
        handler = dlopen("libTestSketch.dylib", RTLD_NOW);
        if (! handler) {
            throw std::runtime_error(dlerror());
        }
        Reset_dlerror();
        creator = reinterpret_cast<SketchBase_creator_t>(dlsym(handler, "create"));
        Check_dlerror();
    }

    std::unique_ptr<SketchBase> create() const {
        return std::unique_ptr<SketchBase>(creator());
    }

    ~SketchFactory() {
        if (handler) {
            dlclose(handler);
        }
    }

private:
    void * handler = nullptr;
    SketchBase_creator_t creator = nullptr;

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
