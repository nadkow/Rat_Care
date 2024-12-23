#ifndef RATCARE_POINTMANAGER_H
#define RATCARE_POINTMANAGER_H

namespace pm {

    int points;

    bool buy(int cost) {
        if (cost > points) {
            return false;
        } else {
            points -= cost;
            return true;
        }
    }

    void earn(int pts) {
        points += pts;
    }

    void load() {
        std::ifstream allfile;
        allfile.open("wallet", std::ios::in);
        std::string str;
        getline(allfile, str);
        allfile.close();
        try {
            points = std::stoi(str);
        } catch (std::invalid_argument &exc) {
            points = 0;
        }
    }

    void save() {
        std::ofstream allfile;
        allfile.open("wallet", std::ios::trunc);
        allfile << points << std::endl;
        allfile.close();
    }
}
#endif //RATCARE_POINTMANAGER_H
