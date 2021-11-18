#include "../include/Customer.h"

using namespace std;

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

    if (cheapestCardioW != nullptr) workout_ids.push_back(cheapestCardioW->getId());
    if (expensiveMixW != nullptr) workout_ids.push_back(expensiveMixW->getId());
    if (cheapestAnaerobicW != nullptr) workout_ids.push_back(cheapestAnaerobicW->getId());
}