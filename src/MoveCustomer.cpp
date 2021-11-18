#include "../include/Action.h"

using namespace std;

MoveCustomer::MoveCustomer(int src, int dst, int customerId): srcTrainer(src), dstTrainer(dst), id(customerId) {}

void MoveCustomer::act(Studio &studio) {
    Trainer *src=studio.getTrainer(srcTrainer);
    Trainer *dst=studio.getTrainer(dstTrainer);
    //legal move
    if(src!= nullptr & dst!= nullptr & src->isOpen() & dst->isOpen()){
        for(Customer *srcCustomer: src->getCustomers()){
            if(srcCustomer->getId()==id)
                if(dst->getAvailable()>=1){
                    src->removeCustomer(id);
                    dst->addCustomer(srcCustomer);
                    if(src->getCustomers().size()==0)
                        src->closeTrainer();
                }
        }
    }
    else
        BaseAction::error("Cannot move customer");
}

std::string MoveCustomer::toString() const {
    return "move "+to_string(srcTrainer)+" "+to_string(dstTrainer)+" "+to_string(id)+to_string(getStatus());
}