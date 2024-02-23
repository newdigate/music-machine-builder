//
// Created by Nicholas Newdigate on 23/02/2024.
//

#include "SketchEngine.h"

#include <iostream>
#include <map>

using namespace newdigate::machine::sketch;

std::map<int, SketchEngine *> SketchEngine::_instances;
unsigned SketchEngine::_instanceCount = 0;
volatile bool newdigate::machine::sketch::shouldClose = false;

void * newdigate::machine::sketch::arduinoThread(void *threadid) {
    long tid = (long)threadid;
    if (SketchEngine::_instances.count(tid) == 0) return nullptr;
    SketchEngine *engine = SketchEngine::_instances[tid];
    engine->Setup();
    while(!shouldClose) {
        engine->Loop();
    }
    //    cout << "Hello World! Thread ID, " << tid << endl;
    pthread_exit(NULL);
}