#include "../include/Customer.h"
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

// region CUSTOMER
Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id), isOrdered(false) {
    pay = 0;
}

std::string Customer::getName() const { return name; }

int Customer::getId() const { return id; }

bool Customer::getOrdered() { return isOrdered; }

void Customer::setOrdered(bool set) { isOrdered = set; }

int Customer::getPay() { return pay; }
// endregion

// region SWEATY CUSTOMER
SweatyCustomer::SweatyCustomer(std::string name, int id) : Customer(name, id) {}

vector<int> SweatyCustomer::order(const vector<Workout> &workout_options) {
    vector<int> workout_ids;
    for (Workout W: workout_options) {
        if (W.getType() == CARDIO) {
            workout_ids.push_back(W.getId());
            pay += W.getPrice();
            cout << getName() << " Is Doing " << W.getName() << endl;
        }
    }
    return workout_ids;
}

std::string SweatyCustomer::toString() const {}
// endregion

// region CHEAP CUSTOMER
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
// endregion

// region HEAVY MUSCLE CUSTOMER
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(name, id) {}

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    vector<Workout> order_workouts;
    vector<int> workout_ids;
    for (Workout W: workout_options) {
        if (W.getType() == ANAEROBIC) {
            order_workouts.push_back(W);
        }
    }

    sort(order_workouts.begin(), order_workouts.end(), [workout_options](const int &a, const int &b) -> bool {
        return workout_options[a].getPrice() >= workout_options[b].getPrice();
    });
    for (Workout W: order_workouts) {
        workout_ids.push_back(W.getId());
        pay += W.getPrice();
    }
    return workout_ids;
}

std::string HeavyMuscleCustomer::toString() const {}

// endregion

// region FULL BODY CUSTOMER
FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name, id) {}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    vector<int> workout_ids;

    int cheapestCardioP = INT32_MAX;
    Workout *cheapestCardioW;
    int expensiveMixP = INT32_MIN;
    Workout *expensiveMixW;
    int cheapestAnaerobicP = INT32_MAX;
    Workout *cheapestAnaerobicW;

    // find workouts to order
    for (Workout W: workout_options) {
        switch (W.getType()) {
            case CARDIO: {
                if (W.getPrice() < cheapestCardioP) {
                    cheapestCardioP = W.getPrice();
                    cheapestCardioW = &W;
                }
                break;
            }
            case MIXED: {
                if (W.getPrice() > expensiveMixP) {
                    expensiveMixP = W.getPrice();
                    expensiveMixW = &W;
                }
                break;
            }
            case ANAEROBIC: {
                if (W.getPrice() < cheapestAnaerobicP) {
                    cheapestAnaerobicP = W.getPrice();
                    cheapestAnaerobicW = &W;
                }
                break;
            }
        }
    }

    if (cheapestCardioW != nullptr) {
        workout_ids.push_back(cheapestCardioW->getId());
        pay += cheapestCardioP;
    }
    if (expensiveMixW != nullptr) {
        workout_ids.push_back(expensiveMixW->getId());
        pay += expensiveMixP;
    }
    if (cheapestAnaerobicW != nullptr) {
        workout_ids.push_back(cheapestAnaerobicW->getId());
        pay += cheapestCardioP;
    }
}

std::string FullBodyCustomer::toString() const {}
// endregion
