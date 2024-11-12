#ifndef RATCARE_RAT_H
#define RATCARE_RAT_H

#include <string>
#include <utility>

class Rat {
public:
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
private:
    std::string name;
    std::string texFilename;
    Model ratModel;
    Node ratNode;
};

#endif //RATCARE_RAT_H
