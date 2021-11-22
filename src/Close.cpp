#include "../include/Action.h"

using namespace std;

Close::Close(int id) : trainerId(id) {}

void Close::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId);
    if (trainer == nullptr || !trainer->isOpen())
        BaseAction::error("Trainer does not exist or is not open");
    else {
        trainer->closeTrainer();
        BaseAction::complete();
    }
}

std::string Close::toString() const {
    if (getStatus() == COMPLETED)
        return "close " + to_string(trainerId) + " Completed";
    else return "close " + to_string(trainerId) + " Error: " + getErrorMsg();
}