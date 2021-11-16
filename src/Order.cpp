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
        for (Customer *customer: trainer->getCustomers()) {
            workout_ids = customer->order(workoutOptions);
            string name = customer->getName();

            trainer->order(customer->getId(), workout_ids, workoutOptions);
            for (int id:workout_ids)
                cout << name << " Is Doing " << workoutOptions[id].getName() << endl;
            // TODO: double loop, print here??
        }
        BaseAction::complete();
    }
}

std::string Order::toString() const {
    // TODO:
}