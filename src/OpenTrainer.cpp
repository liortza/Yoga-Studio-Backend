#include "../include/Action.h"
#include "../include/Studio.h"

using namespace std;

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList): trainerId(id), customers(customersList) {

}
//distractor

void OpenTrainer::act(Studio &studio) {
    Trainer *trainer=studio.getTrainer(trainerId);
    if(trainer==nullptr || trainer->isOpen() ) //todo: || trainer.getAvailable>= customers.size
        BaseAction::error("Workout session does not exist or is already open");
    else{
        trainer->openTrainer();
        vector<Workout>& workout_options=studio.getWorkoutOptions();
        for(Customer *customer: customers){
            trainer->addCustomer(customer);
            vector<int> workout_ids=customer->order(workout_options);
            trainer->order(customer->getId(),workout_ids,workout_options);
            //TODO: order may b in BaseAction-order
            }
        }

    }



std::string OpenTrainer::toString() const {
//
}