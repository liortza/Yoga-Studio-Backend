#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"

class BaseAction;

class Studio {
public:
    Studio(); // todo: needed?? what do we do in empty constructor?
    Studio(const std::string &configFilePath);

    // region RULE OF 5
    virtual ~Studio(); // destructor
    Studio(const Studio &other); // copy constructor
    Studio(Studio &&other); // move copy constructor
    const Studio &operator=(const Studio &other); // assignment operator
    const Studio &operator=(Studio &&other); // move assignment operator
    // endregion

    void start();

    void close();

    int getNumOfTrainers() const;

    Trainer *getTrainer(int tid);

    const std::vector<Trainer *> &getTrainers();

    const std::vector<BaseAction *> &getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout> &getWorkoutOptions();

    void closeTrainer(int id);

private:
    bool open;
    std::vector<Trainer *> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction *> actionsLog;
    int customersCounter;

    void clear();

    //region ACTIONS
    void openTrainer(std::vector<std::string> inputArgs);

    void orderTrainer(int id);

    void moveCustomer(std::vector<std::string> inputArgs);

    void closeAll();

    void printWorkoutOptions();

    void printTrainerStatus(int id);

    void printActionsLog();

    void backupStudio();

    void restore();
    //endregion
};

#endif