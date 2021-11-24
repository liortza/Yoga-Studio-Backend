#include "../include/Action.h"

using namespace std;

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList) : trainerId(id), customers(customersList) {}

// copy constructor
OpenTrainer::OpenTrainer(const OpenTrainer &other) : trainerId(other.trainerId) {
    for (Customer *customer: other.customers) { // deep copy customers
        Customer *myCustomer = customer; // call Customer default copy constructor
        customers.push_back(myCustomer);
    }
}

//destructor
OpenTrainer::~OpenTrainer() { for (Customer *customer: customers) delete customer; }

void OpenTrainer::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId);
    if (trainer == nullptr || trainer->isOpen()) //todo: || trainer.getAvailable>= customers.size
        BaseAction::error("Workout session does not exist or is already open");
    else {
        trainer->openTrainer();
        vector<Workout> &workout_options = studio.getWorkoutOptions();
        for (Customer *customer: customers)
            trainer->addCustomer(customer);
    }
}


std::string OpenTrainer::toString() const {
    if (getStatus() == COMPLETED)
        return "open " + to_string(trainerId) + " Completed";
    return "open " + to_string(trainerId) + " Error: " + getErrorMsg();
}