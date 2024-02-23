#include <dlfcn.h>
#include <iostream>
#include <stdexcept>
#include "SketchFactory.h"

namespace newdigate {
    namespace machine {
        namespace sketch {

            SketchFactory::SketchFactory(): handler(nullptr), fp_setup(nullptr), fp_loop(nullptr) {
            }

            bool SketchFactory::loadSketch(const char *compiledSketchSOfilename) {
                if (handler)
                    dlclose(handler);
                fp_loop = nullptr;
                fp_setup = nullptr;

                handler = dlopen(compiledSketchSOfilename, RTLD_NOW);
                if (! handler) {
                    throw std::runtime_error(dlerror());
                }
                Reset_dlerror();
                fp_setup = reinterpret_cast<fp_void_no_parameters>(dlsym(handler, "setup"));
                Check_dlerror();

                if (!fp_setup) return false;

                Reset_dlerror();
                fp_loop = reinterpret_cast<fp_void_no_parameters>(dlsym(handler, "loop"));
                Check_dlerror();

                if (!fp_loop) return false;
                return true;
            }

            void SketchFactory::setup() const {
                if (handler && fp_setup)
                    fp_setup();
                else
                    std::cout << " ERROR: Sketch not loaded" << std::endl;
            }

            void SketchFactory::loop() const {
                if (handler && fp_loop)
                    fp_loop();
                else
                    std::cout << " ERROR: Sketch not loaded" << std::endl;
            }

            SketchFactory::~SketchFactory() {
                if (handler) {
                    dlclose(handler);
                }
            }

            void SketchFactory::Reset_dlerror() {
                dlerror();
            }

            void SketchFactory::Check_dlerror() {
                const char * dlsym_error = dlerror();
                if (dlsym_error) {
                    throw std::runtime_error(dlsym_error);
                }
            }

        }
    }
}