#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"
#include "BaseAction.h"


class Studio{		
public:
	Studio();
    Studio(const std::string &configFilePath);
    virtual ~Studio();
    Studio(const Studio &other)
    const Studio& operator=(const Sudio &other);
    const Studio& operator=(Studio&& other);
    void start();
    void close();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
    const std::vector<Trainer *>& getTrainers();
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    void addAction(BaseAction *action);

private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    void clear();
};

#endif