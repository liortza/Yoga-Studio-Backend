#include "../include/Action.h"
#include "./Close.cpp"

using namespace std;

CloseAll::CloseAll() {}

void CloseAll::act(Studio &studio) {
    for (Trainer *trainer: studio.getTrainers()) { // is this foreach by increasing id order??
        if (trainer->isOpen()) { // TODO: update in forum, maybe change to all trainers
            trainer->closeTrainer();
        }
    }
    BaseAction::complete();
    studio.close();
}

std::string CloseAll::toString() const { return "closeall Completed"; }