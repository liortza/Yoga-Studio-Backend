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
    ifstream myFile(configFilePath);
    vector<string> inputVector;
    string line;
    if (myFile.is_open()) {
        while (myFile) {
            getline(myFile, line);
            if (line.empty() || line[0] == '#')
                continue;
            else inputVector.push_back(line);
        }
    }
    string line2 = inputVector[1];
    for (int i = 0; i < line2.size(); i = i + 2) {
        int capacity = line2[i] - '0';
        trainers.push_back(new Trainer(capacity));
    }
    for (int i = 2; i < inputVector.size(); i++) {
        int index = 0;
        vector<string> workout;
        for (int j = 0; j < 3 & !inputVector[i].empty(); j++) {
            index = inputVector[i].find(',');
            if (index < string::npos) {
                workout[j] = inputVector[i].substr(0, index);
                inputVector[i].erase(0, index + 2);
            }
        }
        string name = workout[0];
        WorkoutType workoutType;
        if (workout[1] == ("ANAEROBIC"))
            workoutType = ANAEROBIC;
        if (workout[1] == ("MIXED"))
            workoutType = MIXED;
        if (workout[1] == ("CARDIO"))
            workoutType = CARDIO;
        int price = stoi(workout[2]);
        int id = i;
        workout_options.push_back(*new Workout(id, name, price, workoutType));
    }
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

const std::vector<Trainer *> &Studio::getTrainers() { return trainers; }

void Studio::start() {
    open = true;
    cout << "Studio is now open!" << endl;
}

void Studio::close() {
    open = false;
    clear();
    cout << "Studio is now closed!" << endl;
}

// Rule of 5
// destructor
Studio::~Studio() {
    if (!trainers.empty()) {
        for (Trainer *trainer: trainers) delete trainer;
    }
    if (!actionsLog.empty()) {
        for (BaseAction *action: actionsLog) delete action;
    }
}

// copy constructor
Studio::Studio(const Studio &other) {
    open = other.open;
    for (Trainer *trainerPtr: other.trainers) { // deep copy
        Trainer *myTrainer = new Trainer(&trainerPtr); // call Trainer copy constructor
        trainers.push_back(myTrainer);
    }
    workout_options = other.workout_options; // assignment operator
    for (BaseAction *action: other.actionsLog) {
        BaseAction *myAction(action);
        actionsLog.push_back(myAction);
    }
}

// assignment operator
const Studio &Studio::operator=(const Studio &other) {
    if (this != &other) {
        clear();
        open = other.open;

        // deep copy trainers
        for (int i = 0; i < other.getNumOfTrainers(); i++) {
            trainers.push_back(
                    new Trainer(other.trainers[i])); // use class Trainer's copy constructor (returns pointer??)
        }
        for (int i = 0; i < other.workout_options.size(); i++) {
            workout_options.push_back(other.workout_options[i]);
        }
        for (int i = 0; i < other.actionsLog.size(); i++) {
            actionsLog.push_back(new BaseAction(other.actionsLog[i]));
        }
    }
    return *this;
}

// move assignment operator
const Studio &Studio::operator=(Studio &&other) {
    clear();
    trainers = other.trainers;
    workout_options = other.workout_options;
    actionsLog = other.actionsLog;
    other.trainers = nullptr;
    other.workout_options = nullptr;
    other.actionsLog = nullptr;
}

vector<Workout> &Studio::getWorkoutOptions() { return workout_options; }

const vector<BaseAction *> &Studio::getActionsLog() const { return actionsLog; }

void Studio::addAction(BaseAction *action) { actionsLog.push_back(action); }

const std::vector<Trainer *> &Studio::getTrainers() { return trainers; }

void Studio::clear() {
    if (!trainers.empty()) {
        for (Trainer *trainer: trainers) delete trainer;
    }
    if (!actionsLog.empty()) {
        for (BaseAction *action: actionsLog) delete action;
    }
    workout_options.clear();
}