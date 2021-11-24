#include "../include/Action.h"
#include "./Close.cpp"

using namespace std;

CloseAll::CloseAll() {}

void CloseAll::act(Studio &studio) {
    for (Trainer *trainer: studio.getTrainers()) {
        if (trainer->isOpen()) {
            trainer->closeTrainer();
        }
    }
    BaseAction::complete();
    studio.close();
}

std::string CloseAll::toString() const {
    return "closeall Completed";
}