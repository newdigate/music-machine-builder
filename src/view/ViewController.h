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
#include "model.h"
#include "camera.h"

namespace newdigate {
    namespace machine {
        namespace view {

            extern unsigned char tr909_key_intermediate_obj[];
            extern unsigned int tr909_key_intermediate_obj_len;

            class KeyArrayViewController {
            public:

                explicit KeyArrayViewController(GLFWwindow *window, Shader *shader, uint16_t numKeys) : _window(window), _shader(shader), _numKeys(numKeys), _modelTransformGLBuffer(0) {
                    _keyModel = new Model(tr909_key_intermediate_obj, tr909_key_intermediate_obj_len);
                    _modelMatrices = new glm::mat4[MAX_KEYS];
                    _modelTextureIndex = new float[MAX_KEYS] {0};

                    /* Bind the modelTextureIndex instance array parameter */
                    glGenBuffers(1, &_modelTextIndexGLBuffer);
                    glBindBuffer(GL_ARRAY_BUFFER, _modelTextIndexGLBuffer);
                    glBufferData(GL_ARRAY_BUFFER, MAX_KEYS * sizeof(float), &_modelTextureIndex[0], GL_STATIC_DRAW);
                    for (unsigned int i = 0; i < _keyModel->meshes.size(); i++)
                    {
                        unsigned int VAO = _keyModel->meshes[i].VAO;
                        glBindVertexArray(VAO);
                        // set attribute pointers for matrix (4 times vec4)
                        glEnableVertexAttribArray(7);
                        glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(float),  (void*)0);

                        glVertexAttribDivisor(7, 1);

                        glBindVertexArray(0);
                    }

                    /* Bind the instance transform array */

                    glGenBuffers(1, &_modelTransformGLBuffer);
                    glBindBuffer(GL_ARRAY_BUFFER, _modelTransformGLBuffer);
                    glBufferData(GL_ARRAY_BUFFER, MAX_KEYS * sizeof(glm::mat4), &_modelMatrices[0], GL_STATIC_DRAW);


                    // set transformation matrices as an instance vertex attribute (with divisor 1)
                    // note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
                    // normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
                    // -----------------------------------------------------------------------------------------------------------------------------------
                    for (unsigned int i = 0; i < _keyModel->meshes.size(); i++)
                    {
                        unsigned int VAO = _keyModel->meshes[i].VAO;
                        glBindVertexArray(VAO);
                        // set attribute pointers for matrix (4 times vec4)
                        glEnableVertexAttribArray(3);
                        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
                        glEnableVertexAttribArray(4);
                        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
                        glEnableVertexAttribArray(5);
                        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
                        glEnableVertexAttribArray(6);
                        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

                        glVertexAttribDivisor(3, 1);
                        glVertexAttribDivisor(4, 1);
                        glVertexAttribDivisor(5, 1);
                        glVertexAttribDivisor(6, 1);
                        //glVertexAttribDivisor(7, 1);

                        glBindVertexArray(0);
                    }
                }

                void Draw(std::vector<machinekey> keys) {
                    unsigned i = 0;
                    for (auto && key : keys) {
                        glm::mat4 model = glm::mat4(1.0f);
                        _modelMatrices[i] =  glm::translate(model, glm::vec3(key.x, key.y, key.z));
                        i++;
                    }

                    _shader->use();


                    for (unsigned int i = 0; i < _keyModel->meshes.size(); i++)
                    {
                        glBindVertexArray(_keyModel->meshes[i].VAO);

                        glBindBuffer(GL_ARRAY_BUFFER, _modelTextIndexGLBuffer);
                        glBufferData(GL_ARRAY_BUFFER, keys.size() * sizeof(float), &_modelTextureIndex[0], GL_STATIC_DRAW);

                        glBindBuffer(GL_ARRAY_BUFFER, _modelTransformGLBuffer);
                        glBufferData(GL_ARRAY_BUFFER, keys.size() * sizeof(glm::mat4), &_modelMatrices[0], GL_STATIC_DRAW);

                        glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(_keyModel->meshes[i].indices.size()), GL_UNSIGNED_INT, 0, keys.size());

                        glBindVertexArray(0);
                    }
                    _keyModel->Draw(*_shader);
                }


                virtual ~KeyArrayViewController() {
                    delete [] _modelMatrices;
                    delete [] _modelTextureIndex;
                }

            private:
                GLFWwindow* _window;
                Shader *_shader;
                Model *_keyModel;
                uint16_t _numKeys;

                unsigned _modelTextIndexGLBuffer;
                unsigned _modelTransformGLBuffer;

                glm::mat4* _modelMatrices;
                float *_modelTextureIndex;
                const unsigned MAX_KEYS = 256;
            };

            class LEDArrayViewController {
            };

            class DisplayViewController {
            };

            class SceneController {
            public:
                SceneController(GLFWwindow *window, Shader *shader, unsigned awidth, unsigned aheight)
                    : _window(window),
                      _shader(shader),
                      lightPos(1.2f, 5.0f, 2.0f),
                      _width(awidth),
                      _height(aheight) {

                    windowSceneControllers[window] = this;

                    camera.Position = glm::vec3(6.0, 20.0, 5.0);
                    camera.Pitch = -84.0;
                    camera.Yaw = 270;
                    camera.ProcessMouseMovement(0, 0);

                    glfwMakeContextCurrent(window);
                    glfwSetFramebufferSizeCallback(window, delegate_framebuffer_size_callback);// this->framebuffer_size_callback);
                    glfwSetCursorPosCallback(window, delegate_mouse_callback);
                    glfwSetScrollCallback(window, delegate_scroll_callback);

                    glEnable(GL_DEPTH_TEST);
                }

                void Update() {

                    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    //float aspectRatio = static_cast<float>(_width) / static_cast<float>(_height);
                    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)640 / (float)480, 0.1f, 100.0f);
                    glm::mat4 view = camera.GetViewMatrix();
                    _shader->use();
                    _shader->setMat4("projection", projection);
                    _shader->setMat4("view", view);
                    _shader->setVec4("aColor1",0.7f, 0.7f, 0.7f, 0.8f);
                    _shader->setVec4("aColor2",1.0f, 0.7f, 0.7f, 1.0f);
                    _shader->setVec3("lightPos", lightPos);
                    _shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
                    _shader->setVec3("viewPos", camera.Position);
                    // draw planet
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
                    model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
                    _shader->setMat4("model", model);

                }

            private:
                GLFWwindow* _window;
                Shader *_shader;
                static std::map<GLFWwindow*, SceneController*> windowSceneControllers;

                Camera camera;
                glm::vec3 lightPos;
                float lastX;
                float lastY ;
                bool firstMouse;
                unsigned _width, _height;

                // glfw: whenever the window size changed (by OS or user resize) this callback function executes
                // ---------------------------------------------------------------------------------------------
                void framebuffer_size_callback(GLFWwindow* window, int width, int height)
                {
                    // make sure the viewport matches the new window dimensions; note that width and
                    // height will be significantly larger than specified on retina displays.
                    _width = width;
                    _height = height;
                    glViewport(0, 0, width, height);
                }
                static void delegate_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
                    if (windowSceneControllers.count(window) > 0) {
                        windowSceneControllers[window]->framebuffer_size_callback(window, width, height);
                    }
                }
                // glfw: whenever the mouse moves, this callback is called
                // -------------------------------------------------------
                void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
                {
                    float xpos = static_cast<float>(xposIn);
                    float ypos = static_cast<float>(yposIn);

                    if (firstMouse)
                    {
                        lastX = xpos;
                        lastY = ypos;
                        firstMouse = false;
                    }

                    float xoffset = xpos - lastX;
                    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

                    lastX = xpos;
                    lastY = ypos;

                    camera.ProcessMouseMovement(xoffset, yoffset);
                }
                static void delegate_mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
                    if (windowSceneControllers.count(window) > 0) {
                        windowSceneControllers[window]->mouse_callback(window, xposIn, yposIn);
                    }
                }
                // glfw: whenever the mouse scroll wheel scrolls, this callback is called
                // ----------------------------------------------------------------------
                void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
                {
                    camera.ProcessMouseScroll(static_cast<float>(yoffset));
                }
                static void delegate_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
                    if (windowSceneControllers.count(window) > 0) {
                        windowSceneControllers[window]->scroll_callback(window, xoffset, yoffset);
                    }
                }
            };

            class ViewController {
            public:
                explicit ViewController(GLFWwindow *window, Shader *shader, machinemodel *machine, unsigned awidth, unsigned aheight) : _window(window), _shader(shader), _machine(machine), _sceneController(window, shader, awidth, aheight), _keyArrayViewController(window, shader, 18) {
                }

                void Update() {
                    _sceneController.Update();
                    _keyArrayViewController.Draw(machine.Keys);
                }

            private:
                GLFWwindow *_window;
                Shader *_shader;
                machinemodel *_machine;

                SceneController _sceneController;
                KeyArrayViewController _keyArrayViewController;
                LEDArrayViewController _ledArrayViewController;
                DisplayViewController _displayViewController;
            };

            class ViewControllerFactory {
            public:
                ViewController *create(GLFWwindow *window, machinemodel *machine, unsigned awidth, unsigned aheight) {
                    const std::string vertexShaderCodeStr = std::string(vertexShaderCode);
                    const std::string fragmentShaderCodeStr = std::string(fragmentShaderCode);

                    Shader *shader = new Shader(vertexShaderCodeStr,  fragmentShaderCodeStr);
                    return new  ViewController(window, shader, machine, awidth, aheight);
                }
            private:
                static const char* vertexShaderCode;
                static const char* fragmentShaderCode;
            };
        }
    }

} // newdigate

#endif //VIEWCONTROLLER_H
