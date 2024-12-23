#ifndef RATCARE_SHOP_H
#define RATCARE_SHOP_H

extern Cage cage;

namespace shop {

    const char* file = "wares.yaml";
    int noOfFoods;
    YAML::Node waresfile;

    void load() {
        waresfile = YAML::LoadFile(file);
        noOfFoods = waresfile["food"].size();
    }

    std::string getName(int item) {
        return waresfile["food"][item]["name"].as<std::string>();
    }

    int getPrice(int item) {
        return waresfile["food"][item]["price"].as<int>();
    }

    int getHunger(int item) {
        return waresfile["food"][item]["hunger"].as<int>();
    }

    std::string getFilename(int item) {
        return waresfile["food"][item]["file"].as<std::string>();
    }

    void buyFood(int item) {
        if (pm::buy(getPrice(item))) {
            cage.addFood(getFilename(item), getHunger(item));
        }
    }

}
#endif //RATCARE_SHOP_H
