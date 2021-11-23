#include "../include/Action.h"
#include "../src/Order.cpp"

using namespace std;

MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTrainer(src), dstTrainer(dst), id(customerId) {}

void MoveCustomer::act(Studio &studio) {

    Trainer *src = studio.getTrainer(srcTrainer);
    Trainer *dst = studio.getTrainer(dstTrainer);
    //legal move, we assume legal move is between ordered trainers
    if (src != nullptr & dst != nullptr && src->isOpen() & dst->isOpen() & dst->getAvailable() >= 1) {
        for (Customer *srcCustomer: src->getCustomers()) {
            if (srcCustomer->getId() == id) {
                src->removeCustomer(id);
                dst->addCustomer(srcCustomer);
                //place order for the new customer
                Order *order=new Order(dstTrainer);
                order->act(studio);
                if (src->getCustomers().size() == 0)
                    studio.closeTrainer(srcTrainer);
            }
        }
    } else BaseAction::error("Cannot move customer");
}

std::string MoveCustomer::toString() const {
    if (getStatus() == COMPLETED)
        return "move " + to_string(srcTrainer) + " " + to_string(dstTrainer) + " " + to_string(id) + " Completed";
    else
        return "move " + to_string(srcTrainer) + " " + to_string(dstTrainer) + " " + to_string(id) + " Error: " +
               getErrorMsg();
}