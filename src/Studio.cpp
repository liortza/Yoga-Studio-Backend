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

Studio::Studio() = default;

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

    // parse input vector
    string nextLine = inputVector[1];
    int index;
    while (!nextLine.empty()) {
        // trainer's capacity line
        index = nextLine.find(',');
        if (index < string::npos) {
            int capacity = stoi(nextLine.substr(0, index));
            trainers.push_back(new Trainer(capacity));
            nextLine.erase(0, index + 1);
        }
        trainers.push_back(new Trainer(stoi(nextLine))); // last capacity

        // workout options
        vector<string> workout;
        for (int i = 2; i < inputVector.size(); i++) {
            for (int j = 0; j < 2; j++) { // name, type
                index = inputVector[i].find(',');
                workout[j] = inputVector[i].substr(0, index);
                inputVector[i].erase(0, index + 2);
            }
            workout[2] = inputVector[i]; // price

            // create workout
            string name = workout[0];
            WorkoutType workoutType;
            if (workout[1] == ("Anaerobic"))
                workoutType = ANAEROBIC;
            if (workout[1] == ("Mixed"))
                workoutType = MIXED;
            if (workout[1] == ("Cardio"))
                workoutType = CARDIO;
            int price = stoi(workout[2]);
            int id = i - 2; // workouts start from index 2 of array, id's start from 0
            workout_options.push_back(*new Workout(id, name, price, workoutType));
        }
    }
    open = true;
    customersCounter = 0;
}

// region RULE OF 5
// destructor
Studio::~Studio() { clear(); }

// copy constructor
Studio::Studio(const Studio &other) : open(other.open), customersCounter(other.customersCounter),
                                      workout_options(other.workout_options) {

    for (Trainer *trainerPtr: other.trainers) { // deep copy trainers
        Trainer *myTrainer = trainerPtr; // call Trainer copy constructor
        trainers.push_back(myTrainer);
    }
    for (BaseAction *action: other.actionsLog) { // deep copy actionsLog
        BaseAction *myAction = action; // call BaseAction copy constructor
        actionsLog.push_back(myAction);
    }
}

// move copy constructor
Studio::Studio(Studio &&other) : open(other.open), customersCounter(other.customersCounter),
                                 workout_options(other.workout_options), trainers(other.trainers),
                                 actionsLog(other.actionsLog) {
    other.trainers.clear();
    other.actionsLog.clear();
}

// assignment operator
const Studio &Studio::operator=(const Studio &other) {
    if (this != &other) {
        clear();
        open = other.open;
        customersCounter = other.customersCounter;
        workout_options = other.workout_options; // vector assignment operator

        // TODO: use generic deepCopyDuplicate method
        for (Trainer *trainerPtr: other.trainers) { // deep copy trainers
            Trainer *myTrainer = trainerPtr; // call Trainer copy constructor
            trainers.push_back(myTrainer);
        }
        for (BaseAction *action: other.actionsLog) { // deep copy actionsLog
            BaseAction *myAction = action; // call BaseAction copy constructor
            actionsLog.push_back(myAction);
        }
    }
    return *this;
}

// move assignment operator
const Studio &Studio::operator=(Studio &&other) {
    clear();
    open = other.open;
    customersCounter = other.customersCounter;
    workout_options = other.workout_options;

    // steal other's resources (copy vector of pointers)
    trainers = other.trainers;
    actionsLog = other.actionsLog;

    // delete other's pointers
    other.trainers.clear();
    other.actionsLog.clear();
}
// endregion

int Studio::getNumOfTrainers() const {
    return trainers.size();
}

Trainer *Studio::getTrainer(int tid) {
    if (tid >= getNumOfTrainers()) return nullptr; // trainer doesn't exist
    return trainers[tid];
}

const std::vector<Trainer *> &Studio::getTrainers() { return trainers; }

void Studio::start() {
    open = true;
    cout << "Studio is now open!" << endl;
    string nextAction;
    vector<string> inputArgs;
    cin >> nextAction;
    while (nextAction != "closeall") {
        // parse input
        int index;
        while (!nextAction.empty()) {
            index = nextAction.find(' ');
            if (index < string::npos) { // ' ' found
                inputArgs.push_back(nextAction.substr(0, index));
                nextAction.erase(0, index + 1);
            } else {
                inputArgs.push_back(nextAction); // last argument
                inputArgs.clear();
            }
        }

        string actionName = inputArgs[0];

        if (actionName == "open") openTrainer(inputArgs);
        else if (actionName == "order") orderTrainer(stoi(inputArgs[1]));
        else if (actionName == "move") moveCustomer(inputArgs);
        else if (actionName == "close") closeTrainer(stoi(inputArgs[1]));
        else if (actionName == "workout_options") printWorkoutOptions();
        else if (actionName == "status") printTrainerStatus(stoi(inputArgs[1]));
        else if (actionName == "log") printActionsLog();
        else if (actionName == "backup") backupStudio();
        else if (actionName == "restore") restore();

        cin >> nextAction;
    }
    closeAll();
}

void Studio::close() {
    open = false;
    clear();
}

vector<Workout> &Studio::getWorkoutOptions() { return workout_options; }

const vector<BaseAction *> &Studio::getActionsLog() const { return actionsLog; }

void Studio::clear() {
    if (!trainers.empty()) {
        for (Trainer *trainer: trainers) delete trainer;
    }
    if (!actionsLog.empty()) {
        for (BaseAction *action: actionsLog) delete action;
    }
}

// region ACTIONS
void Studio::openTrainer(std::vector<std::string> inputArgs) {
    int trainerId = stoi(inputArgs[1]);
    vector<Customer *> customersList;
    string nextCustomer, name;

    // create all customers from open action
    for (int i = 2; i < inputArgs.size(); i++) {
        nextCustomer = inputArgs[i];
        name = nextCustomer.substr(0, nextCustomer.size() - 4);
        nextCustomer.erase(0, nextCustomer.size() - 3);

        // create new customer by type
        if (nextCustomer == "swt") customersList.push_back(new SweatyCustomer(name, customersCounter));
        else if (nextCustomer == "chp") customersList.push_back(new CheapCustomer(name, customersCounter));
        else if (nextCustomer == "mcl") customersList.push_back(new HeavyMuscleCustomer(name, customersCounter));
        else customersList.push_back(new FullBodyCustomer(name, customersCounter));
        customersCounter++;
    }

    BaseAction *openTrainer = new OpenTrainer(trainerId, customersList);
    openTrainer->act(*this);
    actionsLog.push_back(openTrainer);
}

void Studio::orderTrainer(int id) {
    BaseAction *orderTrainer = new Order(id);
    orderTrainer->act(*this);
    actionsLog.push_back(orderTrainer);
}

void Studio::moveCustomer(std::vector<std::string> inputArgs) {
    BaseAction *move = new MoveCustomer(stoi(inputArgs[0]), stoi(inputArgs[1]), stoi(inputArgs[2]));
    move->act(*this);
    actionsLog.push_back(move);
}

void Studio::closeTrainer(int id) {
    BaseAction *close = new Close(id);
    close->act(*this);
    actionsLog.push_back(close);
}

void Studio::closeAll() {
    BaseAction *closeAll = new CloseAll();
    closeAll->act(*this);
    actionsLog.push_back(closeAll);
}

void Studio::printWorkoutOptions() {
    BaseAction *printWorkoutOptions = new PrintWorkoutOptions();
    printWorkoutOptions->act(*this);
    actionsLog.push_back(printWorkoutOptions);
}

void Studio::printTrainerStatus(int id) {
    BaseAction *printTrainerStatus = new PrintTrainerStatus(id);
    printTrainerStatus->act(*this);
    actionsLog.push_back(printTrainerStatus);
}

void Studio::printActionsLog() {
    BaseAction *printActionsLog = new PrintActionsLog();
    printActionsLog->act(*this);
    actionsLog.push_back(printActionsLog);
}

void Studio::backupStudio() {
    BaseAction *backup = new BackupStudio();
    backup->act(*this);
    actionsLog.push_back(backup);
}

void Studio::restore() {
    BaseAction *restore = new RestoreStudio();
    restore->act(*this);
    actionsLog.push_back(restore);
}
// endregion