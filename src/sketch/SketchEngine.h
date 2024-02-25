//
// Created by Nicholas Newdigate on 23/02/2024.
//

#ifndef SKETCHENGINE_H
#define SKETCHENGINE_H

#include <map>
#include <unistd.h>

#include "hardware_serial.h"
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

                        // drain the serial buffer before loading a new sketch
                        while (Serial.available()) {
                            Serial.read();
                        }

                        auto id = _instanceCount++;
                        shouldClose = false;
                        const int rc = pthread_create(&thread, NULL, arduinoThread, (void *)id);
                        _instances[id] = this;
                        _isRunning = rc == 0;
                        return true;
                    }
                    return false;
                }

                void Setup() {
                    _factory.setup();
                }

                void Loop() {
                    _factory.loop();
                }

                static std::map<int, SketchEngine*> _instances;
                static unsigned _instanceCount;
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
