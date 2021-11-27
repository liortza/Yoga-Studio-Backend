#include "../include/Customer.h"
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

// region CUSTOMER
Customer::Customer(std::string c_name, int c_id) : isOrdered(false), pay(0), name(c_name), id(c_id) {}

Customer::~Customer() = default;

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

std::string SweatyCustomer::toString() const {
    return "name: " + getName() + ", id: " + to_string(getId()), +"type: Sweaty";
}
// endregion

// region CHEAP CUSTOMER
CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id) {}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options) {
    vector<int> workout_ids;
    int cheapest = INT32_MAX;
    int id;
    string name;
    for (Workout W: workout_options) { // assuming workout_options is not empty
        if (W.getPrice() < cheapest) {
            cheapest = W.getPrice();
            id = W.getId();
            name = W.getName();
        }
    }
    workout_ids = {id};
    pay += cheapest; // assuming at least one workout
    cout << getName() << " Is Doing " << name << endl;
    return workout_ids;
}

std::string CheapCustomer::toString() const {
    return "name: " + getName() + ", id: " + to_string(getId()) + ", type: Cheap";
}
// endregion

// region HEAVY MUSCLE CUSTOMER
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(name, id) {}

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    vector<int> workout_ids;
    for (Workout W: workout_options) {
        if (W.getType() == ANAEROBIC) {
            workout_ids.push_back(W.getId());
        }
    }

    sort(workout_ids.begin(), workout_ids.end(), [&workout_options](const int &a, const int &b) {
        return workout_options[a].getPrice() >= workout_options[b].getPrice();
    });
    for (int id: workout_ids) {
        pay += workout_options[id].getPrice();
        cout << getName() << " Is Doing " << workout_options[id].getName() << endl;
    }
    return workout_ids;
}

std::string HeavyMuscleCustomer::toString() const {
    return "name: " + getName() + ", id: " + to_string(getId()), +"type: HeavyMuscle";
}
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
        cout << getName() << " Is Doing " << cheapestCardioW->getName() << endl;
    }
    if (expensiveMixW != nullptr) {
        workout_ids.push_back(expensiveMixW->getId());
        pay += expensiveMixP;
        cout << getName() << " Is Doing " << expensiveMixW->getName() << endl;
    }
    if (cheapestAnaerobicW != nullptr) {
        workout_ids.push_back(cheapestAnaerobicW->getId());
        pay += cheapestCardioP;
        cout << getName() << " Is Doing " << cheapestAnaerobicW->getName() << endl;
    }
    return workout_ids;
}

std::string FullBodyCustomer::toString() const {
    return "name: " + getName() + ", id: " + to_string(getId()), +"type: FullBody";
}
// endregion
