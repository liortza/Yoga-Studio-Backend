#include "../include/Action.h"
#include "../include/Studio.h"

using namespace std;

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList): trainerId(id), customers(customersList) {

}

void OpenTrainer::act(Studio &studio) {

}

std::string OpenTrainer::toString() const {

}