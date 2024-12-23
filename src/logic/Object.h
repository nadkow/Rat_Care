#include <utility>

#ifndef RATCARE_OBJECT_H
#define RATCARE_OBJECT_H

class Object {
public:

    Model model;
    Node* position;
    std::string filename;

    Object(const std::string& filename, Node* pos) {
        this->filename = filename;
        model = Model(std::filesystem::absolute(filename));
        position = pos;
        rootNode.addChild(pos);
    }

    void draw() {
        // try buffer object orphaning
        ratShader.setMat4("transform", position->getTransform());
        model.Draw(ratShader);
    }
};

class Food : public Object {
public:
    int hp;

    Food(const std::string& filename, Node* pos, int points) : Object(filename, pos) {
        hp = points;
    }

    int tryGet(int req_hp) {
        if (hp > req_hp) {
            hp -= req_hp;
            return req_hp;
        } else if (hp == req_hp) {
            hp = 0;
            return req_hp;
        } else {
            hp = 0;
            return hp;
        }
    }
};

#endif //RATCARE_OBJECT_H
