#ifndef RATCARE_CAGE_H
#define RATCARE_CAGE_H

#include "Object.h"

class Cage {
public:
    double xsize = 6.;
    double zsize = 6.;
    double half_xsize = xsize / 2.0;
    double half_zsize = zsize / 2.0;

    void init() {

    }

    void add_object() {

    }

    void draw() {

    }

private:
    std::vector<cage_object> objects;
};

#endif //RATCARE_CAGE_H
