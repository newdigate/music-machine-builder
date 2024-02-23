//
// Created by Nicholas Newdigate on 23/02/2024.
//

#ifndef SKETCHENGINE_H
#define SKETCHENGINE_H

#include <map>
#include <unistd.h>

#include "SketchFactory.h"

namespace newdigate {
    namespace machine {
        namespace sketch {

            extern volatile bool shouldClose;
            extern void *arduinoThread(void *threadid);

            class SketchEngine {
            public:
                SketchEngine() : _isRunning(false), _isError(false) {
                }

                bool IsRunning() const {
                    return _isRunning;
                }

                bool IsError() const {
                    return _isError;
                }

                void Unload() {
                    if (_isRunning) {
                        shouldClose = true;
                        int rc = pthread_join(thread, NULL);
                        _isRunning = false;
                    }
                }

                bool LoadSketch(const char *filename) {
                    auto loaded = _factory.loadSketch(filename);
                    if (loaded) {
                        const int rc = pthread_create(&thread, NULL, arduinoThread, (void *)0);
                        _isRunning = rc == 0;
                        return true;
                    }
                    return false;
                }
                static std::map<int, SketchEngine*> _instances;
            private:
                pthread_t thread;

                SketchFactory _factory;
                bool _isRunning;
                bool _isError;


            };

        }
    }
}
#endif //SKETCHENGINE_H
