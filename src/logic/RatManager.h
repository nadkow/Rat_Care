#ifndef RATCARE_RATMANAGER_H
#define RATCARE_RATMANAGER_H

Shader ratShader;
Node rootNode;
const glm::mat4 defaultSceneRotation = glm::rotate(glm::mat4(1.f), .2f, glm::vec3(0, 1, 0));
const glm::mat4 defaultSceneRotationNeg = glm::rotate(glm::mat4(1.f), -.2f, glm::vec3(0, 1, 0));
extern glm::mat4 view, projection;
const int noOfTexs = 5;
const std::string texFiles[] = {"rat", "rat2", "rat3", "rat4", "rat5"};
const char allRatFilename[] = "all";
const int default_hp = 100;

#include "Rat.h"
#include "Cage.h"
#include <random>

class RatManager {
public:
    RatManager(Cage* cage) : cage(cage) {
        srand(time(nullptr)); // move to main if needed
        rnd = std::default_random_engine{std::random_device{}()};
        dist = (std::uniform_real_distribution<double>(-1., 1.));
    };

    void init(bool isNewDay) {
        ratShader = Shader("res/shaders/model.vert", "res/shaders/model.frag");
        ratShader.use();
        ratShader.setMat4("view", view);
        cage->init();

        if (isNewDay) {
            for (auto child : children) {
                child->beginDay();
                feed(child);
            }
        }
    }

    void createRat(const std::string& name) {
        Node baseNode(glm::rotate(glm::mat4(1.f), (float) glfwGetTime(), {0, 1, 0}));
        baseNode.translate(dist(rnd) * xsize, .0, dist(rnd) * zsize);
        std::string filename = "res/models/rat/";
        filename.append(texFiles[rand()%noOfTexs]);
        filename.append(".obj");
        Rat* rat = new Rat(name, baseNode, filename);
        children.push_back(rat);
        std::string msg = "Rat ";
        msg.append(name);
        msg.append(" created!");
        spdlog::info(msg);
    }

    void createRat(const std::string& name, const std::string& tex, int hunger) {
        Node baseNode(glm::rotate(glm::mat4(1.f), (float) glfwGetTime(), {0, 1, 0}));
        baseNode.translate(dist(rnd) * xsize, .0, dist(rnd) * zsize);
        Rat* rat = new Rat(name, baseNode, tex);
        rat->hunger = hunger;
        children.push_back(rat);
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
            child->draw();
        }
        cage->draw();
    }

    void update() {
        for (auto child : children) {
            child->update();
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

    void saveRats() {
        std::ofstream allfile;
        allfile.open(allRatFilename, std::ios::trunc);
        for (auto child : children) {
            child->save();
            allfile << child->get_name() << std::endl;
        }
        allfile.close();
    }

    void loadRats() {
        std::ifstream allfile;
        allfile.open(allRatFilename, std::ios::in);
        std::string ratname, rattex;
        std::ifstream ratfile;
        // this does not read the empty line at the end of file
        while (getline(allfile, ratname)) {
            // output name form allfile
            spdlog::info("Reading rat from file: {}", ratname);
            // read from individual file
            ratfile.open(ratname, std::ios::in);
            getline(ratfile, rattex);
            std::string hpstr;
            getline(ratfile, hpstr);
            int hp;
            try {
                hp = std::stoi(hpstr);
            } catch (std::invalid_argument &exc) {
                hp = default_hp;
            }
            createRat(ratname, rattex, hp);
            ratfile.close();
        }
        allfile.close();
    }

private:
    Cage* cage;
    std::vector<Rat*> children;
    std::default_random_engine rnd;
    std::uniform_real_distribution<double> dist;

    void feed(Rat* rat) {
        //find food in cage to give to rat
        rat->eat(cage->getFood(100-rat->hunger));
    }
};

#endif //RATCARE_RATMANAGER_H
