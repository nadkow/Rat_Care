#ifndef RATCARE_TASKMANAGER_H
#define RATCARE_TASKMANAGER_H

namespace tasks {

    const char* file = "tasklist.yaml";
    int noOfTasks;
    YAML::Node taskfile;
    bool* switches;

    void init(bool isNewDay, int currHour) {
        taskfile = YAML::LoadFile(file);
        noOfTasks = taskfile["daily"].size();
        switches = new bool[noOfTasks];
        if (isNewDay) {
            // reset all tasks for a new day
            for (int i = 0; i < noOfTasks; i++) {
                switches[i] = false;
            }
        } else {
            for (int i = 0; i < noOfTasks; i++) {
                switches[i] = taskfile["daily"][i]["completed"].as<bool>();
            }
        }
    }

    void saveState() {
        for (int i = 0; i < noOfTasks; i++) {
            taskfile["daily"][i]["completed"] = switches[i];
        }
        std::ofstream fout(file);
        fout << taskfile;
        fout.close();
    }

    std::string getKey(int item) {
        return taskfile["daily"][item]["title"].as<std::string>();
    }

    int getPoints(int item) {
        return taskfile["daily"][item]["points"].as<int>();
    }

}
#endif //RATCARE_TASKMANAGER_H
