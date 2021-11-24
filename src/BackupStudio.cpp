#include "../include/Action.h"

extern Studio *backup;

BackupStudio::BackupStudio() {}

void BackupStudio::act(Studio &studio) {
    //backup: studio's status, trainers, orders, workout options, actions history
    backup = studio; //assignment operator of studio
    BaseAction::complete();

}

std::string BackupStudio::toString() const {
    return "backup Completed"; //never results in an error
}