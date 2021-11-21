#include "../include/Action.h"

using namespace std;

PrintWorkoutOptions::PrintWorkoutOptions() {}

void PrintWorkoutOptions::act(Studio &studio) {
    for (Workout workout: studio.getWorkoutOptions()) {
        cout << workout.getName() << ", " << workout.getType() << ", " << workout.getPrice() << std::endl;
        BaseAction::complete();
    }
}

std::string PrintWorkoutOptions::toString() const {
    return "workout_options Completed";
}