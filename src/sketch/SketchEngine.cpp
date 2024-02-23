//
// Created by Nicholas Newdigate on 23/02/2024.
//

#include "SketchEngine.h"

#include <iostream>
#include <map>


std::map<int, newdigate::machine::sketch::SketchEngine*> newdigate::machine::sketch::SketchEngine:: _instances;
volatile bool newdigate::machine::sketch::shouldClose = false;

void * newdigate::machine::sketch::arduinoThread(void *threadid) {
    long tid = (long)threadid;
    while(!shouldClose) {
        //loop();
        //delay(1);
        std::cout << "one second!!!";
        sleep(1);
    }
    //    cout << "Hello World! Thread ID, " << tid << endl;
    pthread_exit(NULL);
}