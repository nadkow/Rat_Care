#ifndef RATCARE_RATMANAGER_H
#define RATCARE_RATMANAGER_H

Shader ratShader;
Node rootNode;
const glm::mat4 defaultSceneRotation = glm::rotate(glm::mat4(1.f), .2f, glm::vec3(0, 1, 0));
const glm::mat4 defaultSceneRotationNeg = glm::rotate(glm::mat4(1.f), -.2f, glm::vec3(0, 1, 0));
extern glm::mat4 view, projection;

#include "Rat.h"


class RatManager {
public:
    RatManager() = default;

    void init() {
        ratShader = Shader("res/shaders/model.vert", "res/shaders/model.frag");
        ratShader.use();
        ratShader.setMat4("view", view);
    }

    void createRat(const std::string& name) {
        Node baseNode(glm::rotate(glm::mat4(1.f), (float) glfwGetTime(), {0, 1, 0}));
        children.emplace_back(name, baseNode);
    }

    void draw() {
        // draw all rats
        ratShader.use();
        ratShader.setMat4("projection", projection);
        for (auto child : children) {
            child.draw();
        }
    }

    void rotateRootLeft() {
        rootNode.rotate(defaultSceneRotationNeg);
        ratShader.use();
        ratShader.setMat4("transform", rootNode.getTransform());
    }

    void rotateRootRight() {
        rootNode.rotate(defaultSceneRotation);
        ratShader.use();
        ratShader.setMat4("transform", rootNode.getTransform());
    }
private:
    std::vector<Rat> children;
};

#endif //RATCARE_RATMANAGER_H
