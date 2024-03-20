#ifndef RATCARE_RAT_H
#define RATCARE_RAT_H

#include <string>
#include <utility>

class Rat {
public:
    explicit Rat(std::string name, Node node) : name(std::move(name)), ratNode(std::move(node)) {
        rootNode.addChild(&ratNode);
        ratModel = Model(std::filesystem::absolute("res/models/rat/rat.obj"));
    }
    Rat() = default;

    void draw() {
        ratShader.setMat4("transform", ratNode.getTransform());
        ratModel.Draw(ratShader);
    }
private:
    std::string name;
    Model ratModel;
    Node ratNode;
};

#endif //RATCARE_RAT_H
