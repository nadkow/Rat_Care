#ifndef RATCARE_RATMANAGER_H
#define RATCARE_RATMANAGER_H

Shader ratShader;
Node rootNode;
const glm::mat4 defaultSceneRotation = glm::rotate(glm::mat4(1.f), .2f, glm::vec3(0, 1, 0));
const glm::mat4 defaultSceneRotationNeg = glm::rotate(glm::mat4(1.f), -.2f, glm::vec3(0, 1, 0));
extern glm::mat4 view, projection;
const int noOfTexs = 5;
const std::string texFiles[] = {"rat", "rat2", "rat3", "rat4", "rat5"};

#include "Rat.h"
#include "Cage.h"
#include <random>

class RatManager {
public:
    RatManager(Cage &cage) : cage(cage) {
        srand(time(nullptr)); // move to main if needed
        rnd = std::default_random_engine{std::random_device{}()};
        dist = (std::uniform_real_distribution<double>(-1., 1.));
    };

    void init(int hour) {
        ratShader = Shader("res/shaders/model.vert", "res/shaders/model.frag");
        ratShader.use();
        ratShader.setMat4("view", view);
    }

    void createRat(const std::string& name) {
        Node baseNode(glm::rotate(glm::mat4(1.f), (float) glfwGetTime(), {0, 1, 0}));
        baseNode.translate(dist(rnd) * cage.half_xsize, .0, dist(rnd) * cage.half_zsize);
        std::string filename = "res/models/rat/";
        filename.append(texFiles[rand()%noOfTexs]);
        filename.append(".obj");
        children.emplace_back(name, baseNode, filename);
        std::string msg = "Rat ";
        msg.append(name);
        msg.append(" created!");
        spdlog::info(msg);
    }

    void createRat(const std::string& name, const std::string& tex) {
        Node baseNode(glm::rotate(glm::mat4(1.f), (float) glfwGetTime(), {0, 1, 0}));
        baseNode.translate(dist(rnd) * cage.half_xsize, .0, dist(rnd) * cage.half_zsize);
        children.emplace_back(name, baseNode, tex);
        std::string msg = "Rat ";
        msg.append(name);
        msg.append(" created!");
        spdlog::info(msg);
    }

    void draw() {
        // draw all rats
        ratShader.use();
        // TODO probably not necessary to update each frame
        ratShader.setMat4("projection", projection);
        for (auto child : children) {
            child.draw();
        }
        cage.draw();
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

    void saveRats() {
        std::ofstream allfile;
        // TODO change file extension, txt is for debug purposes
        allfile.open("all.txt", std::ios::trunc);
        for (auto child : children) {
            child.save();
            allfile << child.get_name() << std::endl;
        }
        allfile.close();
    }

    void loadRats() {
        std::ifstream allfile;
        allfile.open("all.txt", std::ios::in);
        std::string ratname, rattex;
        std::ifstream ratfile;
        // this does not read the empty line at the end of file
        while (getline(allfile, ratname)) {
            // output name form allfile
            spdlog::info("Reading rat from file:");
            std::cout << ratname << std::endl;
            // read from individual file
            ratfile.open(ratname, std::ios::in);
            getline(ratfile, rattex);
            createRat(ratname, rattex);
            ratfile.close();
        }
        allfile.close();
    }

private:
    Cage &cage;
    std::vector<Rat> children;
    std::default_random_engine rnd;
    std::uniform_real_distribution<double> dist;
};

#endif //RATCARE_RATMANAGER_H
