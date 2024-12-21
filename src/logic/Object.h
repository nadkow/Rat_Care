#include <utility>

#ifndef RATCARE_OBJECT_H
#define RATCARE_OBJECT_H

class Object {
public:

    Model model;
    Node* position;

    Object(const std::string& filename, Node* pos) {
        model = Model(std::filesystem::absolute(filename));
        position = pos;
    }

    void draw() {
        // try buffer object orphaning
        ratShader.setMat4("transform", position->getTransform());
        model.Draw(ratShader);
    }
};

class Food : public Object {
    int hp;

    Food(const std::string& filename, Node* pos, int points) : Object(filename, pos) {
        hp = points;
    }
};

#endif //RATCARE_OBJECT_H
