#include "../include/Action.h"

using namespace std;

BaseAction::BaseAction() {
    errorMsg = nullptr; // TODO: needed??
}

BaseAction::BaseAction(const BaseAction &other) {
    errorMsg = other.errorMsg;
    status = other.status;
}

ActionStatus BaseAction::getStatus() const {return status;}

std::string BaseAction::getErrorMsg() const {return errorMsg;}

void BaseAction::complete() {status = COMPLETED;}

void BaseAction::error(std::string errorMsg) {
    this->errorMsg = errorMsg; // todo: correct syntax?
    status = ERROR;
}