#include "../include/Action.h"

using namespace std;

MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTrainer(src), dstTrainer(dst), id(customerId) {}

void MoveCustomer::act(Studio &studio) {
    Trainer *src = studio.getTrainer(srcTrainer);
    Trainer *dst = studio.getTrainer(dstTrainer);
    //legal move
    if (src != nullptr & dst != nullptr && src->isOpen() & dst->isOpen() & dst->getAvailable() >= 1) {
        for (Customer *srcCustomer: src->getCustomers()) {
            if (srcCustomer->getId() == id) {
                src->removeCustomer(id);
                dst->addCustomer(srcCustomer);
                if (src->getCustomers().size() == 0)
                    src->closeTrainer();
            }
        }
    } else BaseAction::error("Cannot move customer");
    studio.addAction(this);
}

std::string MoveCustomer::toString() const {
    if (getStatus() == COMPLETED)
        return "move " + to_string(srcTrainer) + " " + to_string(dstTrainer) + " " + to_string(id) + " Completed";
    else
        return "move " + to_string(srcTrainer) + " " + to_string(dstTrainer) + " " + to_string(id) + " Error: " +
               getErrorMsg();
}