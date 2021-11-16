#include "../include/Action.h"
#include "./Close.cpp"

using namespace std;

CloseAll::CloseAll() {}

void CloseAll::act(Studio &studio) {
    for (Trainer *trainer:studio.getTrainers()) { // is this foreach by increasing id order??
        if (trainer->isOpen()) { // TODO: update in forum, maybe change to all trainers
            Close(trainer->getId()).act(studio);
            trainer->closeTrainer(); // which one is better?
        }
    }
    // TODO: how to close all open workout sessions?
    studio.close();
}

std::string CloseAll::toString() const {

}