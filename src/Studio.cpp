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
    //customersCounter = 0;

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
    // TODO: make sure we need to pass *this to actions
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
// endregion ACTIONS