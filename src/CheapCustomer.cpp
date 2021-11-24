#include "../include/Customer.h"
#include <iostream>

using namespace std;

CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id) {}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options) {
    vector<int> workout_ids;
    if (!workout_options.empty()) {
        int cheapest = INT32_MAX;
        int id;
        Workout *cheapest_workout;
        for (Workout W: workout_options) {
            if (W.getPrice() < cheapest) {
                cheapest = W.getPrice();
                id = W.getId();
                cheapest_workout = &W;
            }
        }
        workout_ids = {id};
        pay += cheapest;//assuming at least one workout
        cout << getName() << " Is Doing " << cheapest_workout->getName() << endl;
    }
    return workout_ids;
}

std::string CheapCustomer::toString() const {}