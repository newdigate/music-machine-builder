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

            extern unsigned int alpine_keycap_led_intermediate_obj_len;
            extern unsigned char alpine_keycap_led_intermediate_obj[];

            class KeyArrayViewController {
            public:

                explicit KeyArrayViewController(GLFWwindow *window, Shader *shader, uint16_t numKeys, float *textureIndexes) : _window(window), _shader(shader), _numKeys(numKeys), _modelTransformGLBuffer(0), _modelTextureIndex(textureIndexes) {
                    _keyModel = new Model(tr909_key_intermediate_obj, tr909_key_intermediate_obj_len);
                    _modelMatrices = new glm::mat4[MAX_KEYS];
                    //modelTextureIndex = new float[MAX_KEYS] {1.0f, 0.25f, 0.0f};

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

                void Draw(const std::vector<machinekey> &keys) const {
                    unsigned i = 0;
                    for (auto && key : keys) {
                        glm::mat4 model = glm::mat4(1.0f);
                        //model = glm::rotate(model, -static_cast<float>(M_PI)/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
                        _modelMatrices[i] =  glm::translate(model, glm::vec3(key.x, key.y, key.z));
                        i++;
                    }

                    _shader->use();
                    _shader->setVec4("aColor1",0.7f, 0.7f, 0.7f, 0.8f);
                    _shader->setVec4("aColor2",1.0f, 0.7f, 0.7f, 1.0f);

                    for (unsigned int i = 0; i < _keyModel->meshes.size(); i++)
                    {
                        glBindVertexArray(_keyModel->meshes[i].VAO);

                        glBindBuffer(GL_ARRAY_BUFFER, _modelTextIndexGLBuffer);
                        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(keys.size()) * sizeof(float), &_modelTextureIndex[0], GL_STATIC_DRAW);

                        glBindBuffer(GL_ARRAY_BUFFER, _modelTransformGLBuffer);
                        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(keys.size()) * sizeof(glm::mat4), &_modelMatrices[0], GL_STATIC_DRAW);

                        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(_keyModel->meshes[i].indices.size()), GL_UNSIGNED_INT, 0, keys.size());

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
            public:
                    explicit LEDArrayViewController(GLFWwindow *window, Shader *shader, uint16_t numKeys, float *textureIndexes) :
                    _window(window), _shader(shader), _numLEDs(numKeys), _modelTransformGLBuffer(0), _modelTextureIndex(textureIndexes),_modelTextIndexGLBuffer(0)  {
                    _ledModel = new Model(alpine_keycap_led_intermediate_obj, alpine_keycap_led_intermediate_obj_len);
                    _modelMatrices = new glm::mat4[MAX_LEDS];
                    //modelTextureIndex = new float[MAX_KEYS] {1.0f, 0.25f, 0.0f};

                    /* Bind the modelTextureIndex instance array parameter */
                    glGenBuffers(1, &_modelTextIndexGLBuffer);
                    glBindBuffer(GL_ARRAY_BUFFER, _modelTextIndexGLBuffer);
                    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(MAX_LEDS * sizeof(float)), &_modelTextureIndex[0], GL_STATIC_DRAW);
                    for (unsigned int i = 0; i < _ledModel->meshes.size(); i++)
                    {
                        unsigned int VAO = _ledModel->meshes[i].VAO;
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
                    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(MAX_LEDS * sizeof(glm::mat4)), &_modelMatrices[0], GL_STATIC_DRAW);


                    // set transformation matrices as an instance vertex attribute (with divisor 1)
                    // note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
                    // normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
                    // -----------------------------------------------------------------------------------------------------------------------------------
                    for (unsigned int i = 0; i < _ledModel->meshes.size(); i++)
                    {
                        unsigned int VAO = _ledModel->meshes[i].VAO;
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

                void Draw(const std::vector<machineled> &leds) const {
                    unsigned i = 0;
                    for (auto && key : leds) {
                        glm::mat4 model = glm::mat4(1.0f);
                        //model = glm::rotate(model, -static_cast<float>(M_PI)/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
                        _modelMatrices[i] =  glm::translate(model, glm::vec3(key.x, key.y, key.z));
                        i++;
                    }

                    _shader->use();
                    _shader->setVec4("aColor1",0.4f, 0.4f, 0.4f, 1.0f);
                    _shader->setVec4("aColor2",1.0f, 0.0f, 0.0f, 1.0f);

                    for (unsigned int i = 0; i < _ledModel->meshes.size(); i++)
                    {
                        glBindVertexArray(_ledModel->meshes[i].VAO);

                        glBindBuffer(GL_ARRAY_BUFFER, _modelTextIndexGLBuffer);
                        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(leds.size() * sizeof(float)), &_modelTextureIndex[0], GL_STATIC_DRAW);

                        glBindBuffer(GL_ARRAY_BUFFER, _modelTransformGLBuffer);
                        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(leds.size() * sizeof(glm::mat4)), &_modelMatrices[0], GL_STATIC_DRAW);

                        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(_ledModel->meshes[i].indices.size()), GL_UNSIGNED_INT, 0, leds.size());

                        glBindVertexArray(0);
                    }
                    _ledModel->Draw(*_shader);
                }


                virtual ~LEDArrayViewController() {
                    delete [] _modelMatrices;
                    delete [] _modelTextureIndex;
                }
            private:
                GLFWwindow* _window;
                Shader *_shader;
                Model *_ledModel;
                uint16_t _numLEDs;

                unsigned _modelTextIndexGLBuffer;
                unsigned _modelTransformGLBuffer;

                glm::mat4* _modelMatrices;
                float *_modelTextureIndex;
                const unsigned MAX_LEDS = 256;
            };

            class DisplayViewController {
            public:
                static const float vertices[32];
                static const unsigned int indices[6] ;
                GLuint texture;

                explicit DisplayViewController(GLFWwindow *window, Shader *shader, uint16_t *framebuffer) :
                        _window(window),
                        _shader(shader),
                        _framebuffer(framebuffer),
                        VAO(0),
                        VBO(0),
                        EBO(0),
                        texture(0){
                    glGenVertexArrays(1, &VAO);
                    glGenBuffers(1, &VBO);
                    glGenBuffers(1, &EBO);

                    glBindVertexArray(VAO);

                    glBindBuffer(GL_ARRAY_BUFFER, VBO);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

                    // position attribute
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
                    glEnableVertexAttribArray(0);
                    // normal attribute
                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
                    glEnableVertexAttribArray(1);
                    // texture coord attribute
                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
                    glEnableVertexAttribArray(2);


                    // load and create a texture
                    // -------------------------

                    glGenTextures(1, &texture);
                    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
                    // set the texture wrapping parameters
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                    // set texture filtering parameters
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                }

                void Update() {
                    _shader->use();

                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
                    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));

                    _shader->setMat4("aInstanceMatrix", model);
                    glBindTexture(GL_TEXTURE_2D, texture);
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, _framebuffer);
                    // render
                    // ------
                    // render container
                    glBindVertexArray(VAO);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }
            private:
                GLFWwindow* _window;
                Shader *_shader;
                unsigned int VBO;
                unsigned int VAO;
                unsigned int EBO;
                uint16_t *_framebuffer;
            };

            class SceneController {
            public:
                SceneController(GLFWwindow *window, Shader *shader, Shader *texture_shader, unsigned awidth, unsigned aheight)
                    : _window(window),
                      _bicolor_instance_shader(shader),
                      _texture_shader(texture_shader),
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
                    glfwSetMouseButtonCallback(window, delegate_mouse_button_callback);
                    glEnable(GL_DEPTH_TEST);
                }

                void Update() {
                    float currentFrame = static_cast<float>(glfwGetTime());
                    deltaTime = currentFrame - lastFrame;
                    lastFrame = currentFrame;
                    processInput();
                    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    //float aspectRatio = static_cast<float>(_width) / static_cast<float>(_height);
                    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 100.0f);
                    glm::mat4 view = camera.GetViewMatrix();
                    _texture_shader->use();
                    _texture_shader->setMat4("projection", projection);
                    _texture_shader->setMat4("view", view);
                    _texture_shader->setVec3("lightPos", lightPos);
                    _texture_shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
                    _texture_shader->setVec3("viewPos", camera.Position);

                    _bicolor_instance_shader->use();
                    _bicolor_instance_shader->setMat4("projection", projection);
                    _bicolor_instance_shader->setMat4("view", view);
                    _bicolor_instance_shader->setVec4("aColor1",0.7f, 0.7f, 0.7f, 0.8f);
                    _bicolor_instance_shader->setVec4("aColor2",1.0f, 0.7f, 0.7f, 1.0f);
                    _bicolor_instance_shader->setVec3("lightPos", lightPos);
                    _bicolor_instance_shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
                    _bicolor_instance_shader->setVec3("viewPos", camera.Position);
                    // draw planet
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
                    model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
                    _bicolor_instance_shader->setMat4("model", model);


                }
                void processInput()
                {
                    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                        glfwSetWindowShouldClose(_window, true);

                    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
                        camera.ProcessKeyboard(FORWARD, deltaTime);
                    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
                        camera.ProcessKeyboard(BACKWARD, deltaTime);
                    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
                        camera.ProcessKeyboard(LEFT, deltaTime);
                    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
                        camera.ProcessKeyboard(RIGHT, deltaTime);
                    if (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS)
                        camera.ProcessMouseMovement(1, 0);
                    if (glfwGetKey(_window, GLFW_KEY_Q) == GLFW_PRESS)
                        camera.ProcessMouseMovement(-1, 0);
                    if (glfwGetKey(_window, GLFW_KEY_Z) == GLFW_PRESS)
                        camera.ProcessMouseMovement(0, -1);
                    if (glfwGetKey(_window, GLFW_KEY_C) == GLFW_PRESS)
                        camera.ProcessMouseMovement(0, 1);
                }
            private:
                GLFWwindow* _window;
                Shader *_bicolor_instance_shader;
                Shader *_texture_shader;
                static std::map<GLFWwindow*, SceneController*> windowSceneControllers;

                Camera camera;
                glm::vec3 lightPos;
                float lastX;
                float lastY ;
                bool firstMouse;
                unsigned _width, _height;

                // timing
                float deltaTime = 0.0f;
                float lastFrame = 0.0f;
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

                    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
                    if (state != GLFW_PRESS) {
                        return;
                    }


                    camera.ProcessMouseMovement(xoffset, yoffset);
                }
                static void delegate_mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
                    if (windowSceneControllers.count(window) > 0) {
                        windowSceneControllers[window]->mouse_callback(window, xposIn, yposIn);
                    }
                }

                void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
                {
                }
                static void delegate_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
                    if (windowSceneControllers.count(window) > 0) {
                        windowSceneControllers[window]->mouse_button_callback(window, button, action, mods);
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
                explicit ViewController(GLFWwindow *window, Shader *bicolor_instance_shader, Shader *texture_shader, machinemodel *machine, unsigned awidth, unsigned aheight) :
                    _window(window), _bicolor_instance_shader(bicolor_instance_shader), _texture_shader(texture_shader), _machine(machine),
                    _sceneController(window, bicolor_instance_shader, texture_shader, awidth, aheight),
                    _keyArrayViewController(window, bicolor_instance_shader, 18, machine->machine_led_pwm_values),
                    _displayViewController(window, texture_shader, machine->framebuffer),
                    _ledArrayViewController(window, bicolor_instance_shader, 18, machine->machine_led_pwm_values)
                    {
                }

                void Update() {
                    _sceneController.Update();
                    _keyArrayViewController.Draw(machine.Keys);
                    _ledArrayViewController.Draw(machine.Leds);
                    _displayViewController.Update();
                }

            private:
                GLFWwindow *_window;
                Shader *_bicolor_instance_shader;
                Shader *_texture_shader;
                machinemodel *_machine;

                SceneController _sceneController;
                KeyArrayViewController _keyArrayViewController;
                LEDArrayViewController _ledArrayViewController;
                DisplayViewController _displayViewController;
            };

            class ViewControllerFactory {
            public:
                ViewController *create(GLFWwindow *window, machinemodel *machine, unsigned awidth, unsigned aheight) {
                    const std::string vertex_shader_bicolor_code_str = std::string(vertex_shader_bicolor_code);
                    const std::string fragment_shader_bicolor_code_str = std::string(fragment_shader_bicolor_code);
                    Shader *bicolor_instance_shader = new Shader(vertex_shader_bicolor_code_str,  fragment_shader_bicolor_code_str);

                    const std::string vertex_shader_texture_code_str = std::string(vertex_shader_texture_code);
                    const std::string fragment_shader_texture_code_str = std::string(fragment_shader_texture_code);
                    Shader *texture_shader = new Shader(vertex_shader_texture_code_str,  fragment_shader_texture_code_str);

                    return new  ViewController(window, bicolor_instance_shader, texture_shader, machine, awidth, aheight);
                }
            private:
                static const char* vertex_shader_bicolor_code;
                static const char* fragment_shader_bicolor_code;
                static const char* vertex_shader_texture_code;
                static const char* fragment_shader_texture_code;
            };
        }
    }

} // newdigate

#endif //VIEWCONTROLLER_H
