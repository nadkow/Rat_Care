#ifndef RATCARE_TIMEMANAGER_H
#define RATCARE_TIMEMANAGER_H

namespace dtm {

    int hour;
    int day; // 0-365 current day
    int daysPassed = 0;
    bool newDay = false; // whether it's the first run of the day (resets at 4 AM)

    void load() {
        std::ifstream allfile;
        allfile.open("save", std::ios::in);
        std::string daystr;
        getline(allfile, daystr);
        allfile.close();
        try {
            day = std::stoi(daystr);
        } catch (std::invalid_argument &exc) {
            day = 0;
        }
    }

    void init() {
        // at the start of program
        load();
        time_t rawtime;
        struct tm * timeinfo;
        time (&rawtime);
        timeinfo = localtime(&rawtime);
        hour = timeinfo->tm_hour;
        if (day == 0) {
            newDay = true;
            daysPassed = 0;
            day = timeinfo->tm_yday - 1;
            spdlog::info("Welcome!");
        }
        // only counts days passed if it's not the first run and the day has changed
        if (day != timeinfo->tm_yday && hour > 3) {
            newDay = true;
            daysPassed = timeinfo->tm_yday - day;
            day = timeinfo->tm_yday;
            spdlog::info("It's a new day!");
        }
    }

    void save() {
        if (newDay) {
            std::ofstream allfile;
            allfile.open("save", std::ios::trunc);
            allfile << day << std::endl;
            allfile.close();
        }
    }

}
#endif //RATCARE_TIMEMANAGER_H
