#include "../include/Action.h"

using namespace std;

Order::Order(int id) : trainerId(id) {}

void Order::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId);
    if (trainer == nullptr || !trainer->isOpen())
        BaseAction::error("Trainer does not exist or is not open");
    else {
        vector<Workout> workoutOptions = studio.getWorkoutOptions();
        vector<int> workout_ids;

        // place order per each customer in trainer
        for (Customer *customer: trainer->getCustomers()) {
            workout_ids = customer->order(workoutOptions);
            trainer->order(customer->getId(), workout_ids, workoutOptions);
        }
        BaseAction::complete();
    }
}

std::string Order::toString() const {
    if (getStatus() == COMPLETED)
        return "order " + to_string(trainerId) + " Completed";
    else return "order " + to_string(trainerId) + " Error: " + getErrorMsg();
}