#include "../include/Action.h"
#include "../src/Order.cpp"

using namespace std;

MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTrainer(src), dstTrainer(dst), id(customerId) {}

void MoveCustomer::act(Studio &studio) {
    Trainer *src = studio.getTrainer(srcTrainer);
    Trainer *dst = studio.getTrainer(dstTrainer);
    //legal move, we assume legal move is between ordered trainers
    if (src != nullptr & dst != nullptr && src->isOpen() & dst->isOpen() & dst->getAvailable() >= 1) {
        if (src->getCustomer(id) != nullptr) {
            src->removeCustomer(id);
            dst->addCustomer(src->getCustomer(id));
            //place order for the new customer
            Order *order = new Order(dstTrainer);
            order->act(studio);
            //adds close src trainer action to studio.actionLog
            if (src->getCustomers().size() == 0)
                studio.closeTrainer(srcTrainer);
        }
    } else BaseAction::error("Cannot move customer");
}

std::string MoveCustomer::toString() const {
    if (getStatus() == COMPLETED)
        return "move " + to_string(srcTrainer) + " " + to_string(dstTrainer) + " " + to_string(id) + " Completed";
    return "move " + to_string(srcTrainer) + " " + to_string(dstTrainer) + " " + to_string(id) + " Error: " +
           getErrorMsg();
}