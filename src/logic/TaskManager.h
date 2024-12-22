#ifndef RATCARE_TASKMANAGER_H
#define RATCARE_TASKMANAGER_H

namespace tasks {

    const char* file = "tasklist.yaml";
    int noOfTasks;
    YAML::Node taskfile;
    bool* switches;

    void init(int currDay, int currHour) {
        taskfile = YAML::LoadFile(file);
        if (taskfile["lastCompletedDay"].as<int>() != currDay) {
            noOfTasks = taskfile["daily"].size();
            switches = new bool[noOfTasks];
            for (int i = 0; i < noOfTasks; i++) {
                switches[i] = false;
            }
        }
    }

    void endDay() {
        taskfile["lastCompletedDay"] = dtm::day;
        std::ofstream fout(file);
        fout << taskfile;
        fout.close();
    }

    std::string getKey(int item) {
        return taskfile["daily"][item]["title"].as<std::string>();
    }

}
#endif //RATCARE_TASKMANAGER_H
