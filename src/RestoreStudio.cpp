#include "../include/Action.h"

extern Studio *backup;

RestoreStudio::RestoreStudio() {}

void RestoreStudio::act(Studio &studio) {
    if (backup != nullptr) {
        studio = *backup;//assignment operator of studio
        BaseAction::complete();
    } else BaseAction::error("No backup available");
}

std::string RestoreStudio::toString() const {
    if (getStatus() == COMPLETED)
        return "restore Completed";
    return "restore Error: " + getErrorMsg();
}