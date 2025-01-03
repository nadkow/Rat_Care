#include "imgui.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"
#include <cstdio>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include <glad/glad.h>  // Initialize with gladLoadGL()

#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

const char *glsl_version = "#version 460";
constexpr int32_t GL_VERSION_MAJOR = 4;
constexpr int32_t GL_VERSION_MINOR = 6;

glm::vec3 cameraPos = glm::vec3(0.0f, 3.f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, -3.0f, -10.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 0.996f, 0.08f);
glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

#include <ctime>
#include <random>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <yaml-cpp/yaml.h>
#include "rendering/Shader.h"
#include "rendering/Skybox.h"
#include "logic/Node.h"
#include "rendering/Model.h"
#include "logic/RatManager.h"
#include "logic/TimeManager.h"
#include "logic/PointManager.h"
#include "logic/TaskManager.h"
#include "logic/Shop.h"
#include "rendering/GuiManager.h"

/** FUNCTIONS **/

void update();

void render();

/** GLOBAL VARIABLES **/

ImVec4 clear_color = ImVec4(0.45f, 0.25f, 0.20f, 1.f);

float deltaTime = 0.0f;  // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float currentFrame;

Cage cage;
RatManager ratManager(&cage);

int main(int, char **) {

    if (gui::init()) return EXIT_FAILURE;
    dtm::init();
    skybox::init(dtm::hour);
    pm::load();
    shop::load();
    tasks::init(dtm::newDay, dtm::hour);
    cage.load();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

    // Shader reflectShader("res/shaders/basic.vert", "res/shaders/reflect.frag");
    // Shader refractShader("res/shaders/basic.vert", "res/shaders/refract.frag");
    ratManager.init(dtm::newDay);
    //ratManager.createRat("moomoo");

    // Main loop
    while (!glfwWindowShouldClose(gui::window)) {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process I/O operations here
        gui::input();

        // Update game objects' state here
        //update();

        // OpenGL rendering code here
        render();

        gui::draw();
    }

    // Save game
    ratManager.saveRats();
    cage.save();
    dtm::save();
    pm::save();
    tasks::saveState();
    gui::terminate();

    return 0;
}

void update() {

    ratManager.update();
}

void render() {
    // clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    skybox::draw();
    // draw all rats and cage
    ratManager.draw();
}

