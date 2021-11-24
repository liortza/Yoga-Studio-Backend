#include "../include/Action.h"

using namespace std;

PrintTrainerStatus::PrintTrainerStatus(int id) : trainerId(id) {}

void PrintTrainerStatus::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId);
    if (trainer != nullptr && trainer->wasOpened()) {
        if (trainer->isOpen()) {
            cout << "Trainer " + to_string(trainerId) + " status: open" << endl;
            cout << "Customers:" << endl;
            for (Customer *customer: trainer->getCustomers())
                cout << to_string(customer->getId()) << " " << customer->getName() << endl;
            cout << "Orders:" << endl;
            for (OrderPair order: trainer->getOrders())
                cout << order.second.getName() << " " << to_string(order.second.getPrice()) << "NIS "
                     << to_string(order.first) << endl;
            cout << "Current Trainer's Salary: " << to_string(trainer->getSalary()) << "NIS" << endl;
        } else cout << "Trainer " + to_string(trainerId) + " status: closed" << endl;
    }
    BaseAction::complete();
}

std::string PrintTrainerStatus::toString() const {
    return "status " + to_string(trainerId) + " Completed";
}