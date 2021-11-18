#include "../include/Action.h"

using namespace std;

Order::Order(int id): trainerId(id) {}

void Order::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId);
    if (trainer == nullptr || !trainer->isOpen())
        BaseAction::error("Trainer does not exist or is not open");
    else {
        vector<Workout> workoutOptions = studio.getWorkoutOptions();
        vector<int> workout_ids;

        // place order per each customer in trainer
        for (Customer *customer: trainer->getCustomers())
            customer->order(workoutOptions);
        BaseAction::complete();
    }
}

std::string Order::toString() const {
    // TODO:
}