#include "../include/Action.h"

using namespace std;

BaseAction::BaseAction() {
    errorMsg = nullptr;
    // status = ??
    // kjsldf
}

ActionStatus BaseAction::getStatus() const {return status;}

std::string BaseAction::getErrorMsg() const {return errorMsg;}

void BaseAction::complete() {status = COMPLETED;}

void BaseAction::error(std::string errorMsg) {this->errorMsg;}