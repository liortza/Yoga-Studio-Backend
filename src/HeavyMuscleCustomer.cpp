#include "../include/Customer.h"
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id): Customer(name, id) {}

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    vector<Workout> order_workouts; // duplicate workout objects, maybe by reference/pointers?
    vector<int> workout_ids;
    for (Workout W:workout_options) {
        if (W.getType() == ANAEROBIC) {
            order_workouts.push_back(W);
        }
    }

    sort(order_workouts.begin(), order_workouts.end(), &Workout::expensiveThan);
    for (Workout W: order_workouts)
        workout_ids.push_back(W.getId());
    return workout_ids;
}

