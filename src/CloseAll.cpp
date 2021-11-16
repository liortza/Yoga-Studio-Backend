#include "../include/Action.h"

using namespace std;

CloseAll::CloseAll() {}

void CloseAll::act(Studio &studio) {
    for (Trainer *trainer:studio.getTrainers()) { // is this foreach by increasing id order??
        if (trainer->isOpen()) { // TODO: update in forum, maybe change to all trainers
            trainer->closeTrainer();
        }
    }

}