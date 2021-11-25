#include "../include/Action.h"

using namespace std;
extern Studio *backup;

// region BASEACTION
BaseAction::BaseAction() = default;

BaseAction::BaseAction(const BaseAction &other) = default;

BaseAction::~BaseAction() = default;

ActionStatus BaseAction::getStatus() const { return status; }

void BaseAction::complete() { status = COMPLETED; }

void BaseAction::error(std::string errorMsg) {
    this->errorMsg = errorMsg; // todo: correct syntax?
    status = ERROR;
}

std::string BaseAction::getErrorMsg() const { return errorMsg; }
// endregion

// region OPENTRAINER
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
    if (trainer == nullptr || trainer->isOpen())
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
// endregion

// region ORDER
Order::Order(int id) : trainerId(id) {}

void Order::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId);
    if (trainer == nullptr || !trainer->isOpen())
        BaseAction::error("Trainer does not exist or is not open");
    else {
        vector<Workout> workoutOptions = studio.getWorkoutOptions();
        vector<int> workout_ids;

        // place order per each customer in trainer
        for (Customer *customer: trainer->getCustomers()) {
            workout_ids = customer->order(workoutOptions);
            trainer->order(customer->getId(), workout_ids, workoutOptions);
        }
        BaseAction::complete();
    }
}

std::string Order::toString() const {
    if (getStatus() == COMPLETED)
        return "order " + to_string(trainerId) + " Completed";
    return "order " + to_string(trainerId) + " Error: " + getErrorMsg();
}
// endregion

// region MOVECUSTOMER
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
// endregion

// region CLOSE
Close::Close(int id) : trainerId(id) {}

void Close::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId);
    if (trainer == nullptr || !trainer->isOpen())
        BaseAction::error("Trainer does not exist or is not open");
    else {
        trainer->closeTrainer();
        BaseAction::complete();
    }
}

std::string Close::toString() const {
    if (getStatus() == COMPLETED)
        return "close " + to_string(trainerId) + " Completed";
    return "close " + to_string(trainerId) + " Error: " + getErrorMsg();
}
// endregion

// region CLOSEALL
CloseAll::CloseAll() {}

void CloseAll::act(Studio &studio) {
    for (Trainer *trainer: studio.getTrainers()) {
        if (trainer->isOpen()) { trainer->closeTrainer(); }
    }
    BaseAction::complete();
    studio.close();
}

std::string CloseAll::toString() const { return "closeall Completed"; }
// endregion

// region PRINTWORKOUTOPTIONS
PrintWorkoutOptions::PrintWorkoutOptions() {}

void PrintWorkoutOptions::act(Studio &studio) {
    for (Workout workout: studio.getWorkoutOptions()) {
        cout << workout.getName() << ", " << workout.getType() << ", " << workout.getPrice() << std::endl;
        BaseAction::complete();
    }
}

std::string PrintWorkoutOptions::toString() const { return "workout_options Completed"; }
// endregion

// region PRINTTRAINERSTATUS
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
// endregion

// region PRINTACTIONSLOG
PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Studio &studio) {
    for (BaseAction *action: studio.getActionsLog())
        cout << action->toString() << endl;
    BaseAction::complete();
}

std::string PrintActionsLog::toString() const { return "log Completed"; }
// endregion

// region BACKUPSTUDIO
BackupStudio::BackupStudio() {}

void BackupStudio::act(Studio &studio) {
    //backup: studio's status, trainers, orders, workout options, actions history
    *backup = studio; //assignment operator of studio
    BaseAction::complete();

}

std::string BackupStudio::toString() const {
    return "backup Completed"; //never results in an error
}
// endregion

// region RESTORESTUDIO
RestoreStudio::RestoreStudio() {}

void RestoreStudio::act(Studio &studio) {
    if (backup != nullptr) {
        studio = *backup;//assignment operator of studio
        BaseAction::complete();
    } else BaseAction::error("No backup available");
}

std::string RestoreStudio::toString() const {
    if (getStatus() == COMPLETED)
        return "restore Completed";
    return "restore Error: " + getErrorMsg();
}
// endregion
