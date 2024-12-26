#ifndef RATCARE_RAT_H
#define RATCARE_RAT_H

#include <string>
#include <utility>

class Rat {
public:
    int hunger = default_hp;
    int happiness = 0;

    explicit Rat(std::string name, Node node, std::string filename) : name(std::move(name)), ratNode(std::move(node)), texFilename(std::move(filename)) {
        rootNode.addChild(&ratNode);
        ratModel = Model(std::filesystem::absolute(texFilename));
    }
    Rat() = default;

    void draw() {
        // try buffer object orphaning
        ratShader.setMat4("transform", ratNode.getTransform());
        ratModel.Draw(ratShader);
    }

    void update() {
        
    }

    void save() {
        // save rat to individual file
        std::ofstream ratfile;
        std::string ratFilename = name;
        ratfile.open(ratFilename, std::ios::trunc);
        ratfile << texFilename << std::endl;
        ratfile << hunger << std::endl;
        ratfile.close();
    }

    std::string get_name() {
        return name;
    }

    void beginDay() {
        hunger -= default_hp;
    }

    void eat(int hp) {
        hunger += hp;
    }

private:
    std::string name;
    std::string texFilename;
    Model ratModel;
    Node ratNode;
};

#endif //RATCARE_RAT_H
