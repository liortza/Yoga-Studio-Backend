#include "../include/Studio.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

Studio::Studio() {
    open = false;
    customersCounter = 0;
}

Studio::Studio(const string &configFilePath) : open(false), customersCounter(0) {

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
    line = inputVector[1];
    int index;

    // initialize trainers
    index = line.find(',');
    int capacity;
    while (index != int(string::npos)) {
        capacity = stoi(line.substr(0, index));
        trainers.push_back(new Trainer(capacity));
        line.erase(0, index + 1);
        index = line.find(',');
    }
    trainers.push_back(new Trainer(stoi(line))); // last capacity

    //initialize workouts
    string nextLine, name, s_type;
    int id, price;
    WorkoutType type;

    for (int i = 2; i < int(inputVector.size()); i++) { // workouts start from 3'rd line
        nextLine = inputVector[i];

        index = nextLine.find(','); // name
        name = nextLine.substr(0, index);
        nextLine.erase(0, index + 2);

        index = nextLine.find(','); // type
        s_type = nextLine.substr(0, index);
        if (s_type == ("Anaerobic"))
            type = ANAEROBIC;
        if (s_type == ("Mixed"))
            type = MIXED;
        if (s_type == ("Cardio"))
            type = CARDIO;
        nextLine.erase(0, index + 2);

        price = stoi(nextLine);
        id = i - 2; // workouts start from index 2 of array, id's start from 0
        cout << "id: " << to_string(id) << " name:" << name << " price: " << to_string(price) << " type enum: " << type
             << endl;
        workout_options.push_back(*new Workout(id, name, price, type));
    }
}

// region RULE OF 5
// destructor
Studio::~Studio() { clear(); }

// copy constructor
Studio::Studio(const Studio &other) : open(other.open), customersCounter(other.customersCounter) {
    for (const Workout &W: other.workout_options) workout_options.push_back(W);
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
Studio::Studio(Studio &&other) : open(other.open), customersCounter(other.customersCounter), trainers(other.trainers),
                                 actionsLog(other.actionsLog) {
    for (const Workout &W: other.workout_options) workout_options.push_back(W);
    other.trainers.clear();
    other.actionsLog.clear();
}

// assignment operator
const Studio &Studio::operator=(const Studio &other) {
    if (this != &other) {
        clear();
        open = other.open;
        customersCounter = other.customersCounter;
        for (const Workout &W: other.workout_options) workout_options.push_back(W);

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
    for (const Workout &W: other.workout_options) workout_options.push_back(W);

    // steal other's resources (copy vector of pointers)
    trainers = other.trainers;
    actionsLog = other.actionsLog;

    // delete other's pointers
    other.trainers.clear();
    other.actionsLog.clear();

    return *this;
}
// endregion

void Studio::start() {
    open = true;
    cout << "Studio is now open!" << endl;
    string nextAction;
    vector<string> inputArgs;
    getline(cin, nextAction);
    while (nextAction != "closeall") {
        // parse input
        int index;
        while (!nextAction.empty()) {
            index = nextAction.find(' ');
            if (index != int(string::npos)) { // ' ' found
                inputArgs.push_back(nextAction.substr(0, index));
                nextAction.erase(0, index + 1);
            } else {
                inputArgs.push_back(nextAction); // last argument
                nextAction.clear();
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

        inputArgs.clear();
        getline(cin, nextAction);
    }
    closeAll();
}

void Studio::close() {
    open = false;
    clear();
}

int Studio::getNumOfTrainers() const { return trainers.size(); }

Trainer *Studio::getTrainer(int tid) {
    if (tid >= getNumOfTrainers()) return nullptr; // trainer doesn't exist
    return trainers[tid];
}

const std::vector<Trainer *> &Studio::getTrainers() { return trainers; }

const vector<BaseAction *> &Studio::getActionsLog() const { return actionsLog; }

vector<Workout> &Studio::getWorkoutOptions() { return workout_options; }

void Studio::closeTrainer(int id) {
    BaseAction *close = new Close(id);
    close->act(*this);
    actionsLog.push_back(close);
}

void Studio::clear() {
    for (Trainer *trainer: trainers) delete trainer;
    for (BaseAction *action: actionsLog) delete action;
    workout_options.clear();
    trainers.clear();
    actionsLog.clear();
}

// region ACTIONS
void Studio::openTrainer(std::vector<std::string> inputArgs) {
    int trainerId = stoi(inputArgs[1]);
    vector<Customer *> customersList;
    string nextCustomer, name;

    if (inputArgs.size() >= 2) { // at least one customer
        // TODO: check if trainer doesn't exist or is already open (error) - need to create new customers?
        // create all customers from open action
        for (int i = 2; i < int(inputArgs.size()); i++) { // assuming input arguments no more than max int
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