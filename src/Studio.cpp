#include "../include/Studio.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "../include/Workout.h"
#include "../include/Trainer.h"
#include "../include/Action.h"
#include <filesystem>

using namespace std;

Studio::Studio(const string &configFilePath) {
    // extract file content to "nextLine" string
    /*ifstream myFile(configFilePath);
    vector<string> inputVector;
    string line;
    if (myFile.is_open()) {
        while (myFile) {
            getline(myFile, line);
            if (line.empty() || line[0] == '#')
                continue;
            else inputVector.push_back(line);
        }
    }*/

    //trainers = nullptr;

}

//TODO:1. parse input (example) 2. open=false, actionslog>empty vector 3. check if the workouts need to be initialized from the input.

int Studio::getNumOfTrainers() const {
    return trainers.size();
}

Trainer *Studio::getTrainer(int tid) {
    for (Trainer *trainerPtr: trainers) {
        if (trainerPtr->getId() == tid) return trainerPtr;
    }
    // TODO: error if doesn't exist? return nullptr?
}

void Studio::start() {
    open = true;
    cout << "Studio is now open!" << endl;
}

// Rule of 5
// destructor
Studio::~Studio() {
    //if (trainers) delete[] trainers;
    //if (actionsLog) delete[] actionsLog;
    // TODO: how to check if vector is not null?
    //if(workout_options) delete[] workout_options; - vector of values (not pointers) --> deleted by default destructor of vector?
}

// copy constructor
Studio::Studio(const Studio &other) {
    //TODO: implement copy constructor
    open = other.open;
    //trainers = other.
    //workout_options = other.
}

// assignment operator
const Studio &Studio::operator=(const Studio &other) {
    if (this != &other) {
        open = other.open;
        if (trainers) delete[] trainers;
        if (workout_options) delete[] workout_options;
        if (actionsLog) delete[] actionsLog;

        // deep copy trainers
        for (int i = 0; i < other.getNumOfTrainers(); i++) {
            trainers.push_back(new Trainer(other.trainers[i])); // use class Trainer's copy constructor (returns pointer??)
        }
        for (int i = 0; i < other.workout_options.size(); i++) {
            workout_options.push_back(other.workout_options[i]);
        }
        for (int i=0; i<other.actionsLog.size();i++){
            actionsLog.push_back(new BaseAction(other.actionsLog[i]));
        }
    }
    return *this;
}

// move assignment operator
const Studio &Studio::operator=(Studio &&other) {
    if (trainers) delete[] trainers;
    if (workout_options) delete[] workout_options;
    if (actionsLog) delete[] actionsLog;
    trainers = other.trainers;
    workout_options = other.workout_options;
    actionsLog = other.actionsLog;
    other.trainers = nullptr;
    other.workout_options = nullptr;
    other.actionsLog = nullptr;
}

vector<Workout> &Studio::getWorkoutOptions() { return workout_options; }

const vector<BaseAction *> &Studio::getActionsLog() const { return actionsLog; }

const std::vector<Trainer *> &Studio::getTrainers() {return trainers;}
