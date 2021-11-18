#include "../include/Customer.h"
#include <iostream>

using namespace std;

SweatyCustomer::SweatyCustomer(std::string name, int id): Customer(name, id) {}

vector<int> SweatyCustomer::order(const vector<Workout> &workout_options) {
    vector<int> workout_ids;
    for (Workout W: workout_options) {
        if (W.getType() == CARDIO) {
            workout_ids.push_back(W.getId());
            cout << getName() << " Is Doing " << W.getName() << endl;
        }
    }
    return workout_ids;
}

std::string SweatyCustomer::toString() const {}