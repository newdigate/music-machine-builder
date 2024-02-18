//
// Created by Nicholas Newdigate on 18/02/2024.
//

#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#else
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#include "../model/machinemodel.h"

namespace newdigate {
    namespace machine {
        namespace view {

            class KeyArrayViewController {
            public:
                void Draw(std::vector<machinekey> keys) {
                };
            private:
                static unsigned char tr909_key_intermediate_obj[];
            };

            class LEDArrayViewController {

            };

            class DisplayViewController {

            };

            class ViewController {
            public:
                explicit ViewController(GLFWwindow *window) : _window(window) {
                }
            private:
                GLFWwindow* _window;
                KeyArrayViewController _keyArrayViewController;
                LEDArrayViewController _ledArrayViewController;
                DisplayViewController _displayViewController;
            };

        }
    }

} // newdigate

#endif //VIEWCONTROLLER_H
