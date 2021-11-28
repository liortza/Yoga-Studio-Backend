#include "../include/Action.h"

using namespace std;
extern Studio *backup;

// region BASE_ACTION
BaseAction::BaseAction() = default;

BaseAction::~BaseAction() = default;

ActionStatus BaseAction::getStatus() const { return status; }

void BaseAction::complete() { status = COMPLETED; }

void BaseAction::error(std::string errorMsg) {
    this->errorMsg = errorMsg;
    status = ERROR;
}

std::string BaseAction::getErrorMsg() const { return errorMsg; }

// endregion

// region OPEN_TRAINER
OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList) : trainerId(id), customers(customersList) {}

OpenTrainer::~OpenTrainer() { for (Customer *C: customers) delete C; }

void OpenTrainer::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId);
    if (trainer == nullptr || trainer->isOpen() || customers.empty()) {
        BaseAction::error("Error: Trainer does not exist or is already open");
        cout << "Error: Trainer does not exist or is already open" << endl;
    } else {
        trainer->openTrainer();
        for (Customer *C: customers) { trainer->addCustomer(C->clone()); }
        BaseAction::complete();
    }
}

std::string OpenTrainer::toString() const {
    if (getStatus() == COMPLETED)
        return "open " + to_string(trainerId) + " Completed";
    return "open " + to_string(trainerId) + " " + getErrorMsg();
}

BaseAction *OpenTrainer::clone() {
    vector<Customer *> newCustomers; // clone customerList
    for (Customer *C: customers) newCustomers.push_back(C->clone());

    OpenTrainer *newOpen = new OpenTrainer(trainerId, newCustomers);

    if (getStatus() == COMPLETED) newOpen->complete(); // clone status
    else newOpen->error(getErrorMsg());
    return newOpen;
}
// endregion

// region ORDER
Order::Order(int id) : trainerId(id) {}

void Order::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId);
    if (trainer == nullptr || !trainer->isOpen()) {
        BaseAction::error("Error: Trainer does not exist or is not open");
        cout << "Error: Trainer does not exist or is not open" << endl;
    } else {
        vector<Workout> workoutOptions;
        for (Workout workout: studio.getWorkoutOptions()) workoutOptions.push_back(workout);
        vector<int> workout_ids;

        // place orders of new customers (no duplicates)
        for (Customer *customer: trainer->getCustomers()) {
            if (!customer->getOrdered()) {
                workout_ids = customer->order(workoutOptions);
                trainer->order(customer->getId(), workout_ids, workoutOptions);
            }
        }

        // iterate through trainer's orderList and print
        vector<OrderPair> ordersVector;
        for (OrderPair orderPair: trainer->getOrders()) ordersVector.push_back(orderPair);

        Customer *currentCustomer;
        Workout *currentWorkout;
        for (int i = 0; i < int(ordersVector.size()); i++) {
            currentCustomer = trainer->getCustomer(ordersVector[i].first);
            currentWorkout = &ordersVector[i].second;
            cout << currentCustomer->getName() << " Is Doing " << currentWorkout->getName() << endl;
        }
        BaseAction::complete();
    }
}

std::string Order::toString() const {
    if (getStatus() == COMPLETED)
        return "order " + to_string(trainerId) + " Completed";
    return "order " + to_string(trainerId) + " " + getErrorMsg();
}

BaseAction *Order::clone() {
    Order *newOrder = new Order(trainerId);
    if (getStatus() == COMPLETED) newOrder->complete();
    else newOrder->error(getErrorMsg());
    return newOrder;
}
// endregion

// region MOVE_CUSTOMER
MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTrainer(src), dstTrainer(dst), id(customerId) {}

void MoveCustomer::act(Studio &studio) {
    Trainer *src = studio.getTrainer(srcTrainer);
    Trainer *dst = studio.getTrainer(dstTrainer);

    Customer *customer;
    if (src != nullptr) customer = src->getCustomer(id);

    //legal move, we assume move is between two ordered trainers
    if ((src != nullptr) & (dst != nullptr) & (customer != nullptr) &&
        (src->isOpen()) & (dst->isOpen()) & (dst->getAvailable() >= 1)) {
        src->removeCustomer(id);
        dst->addCustomer(customer);

        // place order for new customer in dst
        vector<int> workout_ids;
        workout_ids = customer->order(studio.getWorkoutOptions());
        dst->order(customer->getId(), workout_ids, studio.getWorkoutOptions());

        // adds close src trainer action to studio.actionLog
        if (src->getCustomers().size() == 0) studio.closeTrainer(srcTrainer);

        BaseAction::complete();
    } else {
        BaseAction::error("Error: Cannot move customer");
        cout << "Error: Cannot move customer" << endl;
    }
}

std::string MoveCustomer::toString() const {
    if (getStatus() == COMPLETED)
        return "move " + to_string(srcTrainer) + " " + to_string(dstTrainer) + " " + to_string(id) + " Completed";
    return "move " + to_string(srcTrainer) + " " + to_string(dstTrainer) + " " + to_string(id) + " " + getErrorMsg();
}

BaseAction *MoveCustomer::clone() {
    MoveCustomer *newMove = new MoveCustomer(srcTrainer, dstTrainer, id);
    if (getStatus() == COMPLETED) newMove->complete();
    else newMove->error(getErrorMsg());
    return newMove;
}
// endregion

// region CLOSE
Close::Close(int id) : trainerId(id) {}

void Close::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId);
    if (trainer == nullptr || !trainer->isOpen()) {
        BaseAction::error("Error: Trainer does not exist or is not open");
        cout << "Error: Trainer does not exist or is not open" << endl;
    } else {
        trainer->closeTrainer();
        BaseAction::complete();
    }
}

std::string Close::toString() const {
    if (getStatus() == COMPLETED)
        return "close " + to_string(trainerId) + " Completed";
    return "close " + to_string(trainerId) + " " + getErrorMsg();
}

BaseAction *Close::clone() {
    Close *newClose = new Close(trainerId);
    if (getStatus() == COMPLETED) newClose->complete();
    else newClose->error(getErrorMsg());
    return newClose;
}
// endregion

// region CLOSE_ALL
CloseAll::CloseAll() {}

void CloseAll::act(Studio &studio) {
    for (Trainer *trainer: studio.getTrainers()) {
        if (trainer->isOpen()) { trainer->closeTrainer(); }
    }
    BaseAction::complete();
    studio.close();
}

std::string CloseAll::toString() const { return "closeall Completed"; }

BaseAction *CloseAll::clone() {
    CloseAll *newCloseAll = new CloseAll();
    newCloseAll->complete();
    return newCloseAll;
}
// endregion

// region PRINT_WORKOUT_OPTIONS
PrintWorkoutOptions::PrintWorkoutOptions() {}

void PrintWorkoutOptions::act(Studio &studio) {
    string type;
    for (Workout W: studio.getWorkoutOptions()) {
        if (W.getType() == ANAEROBIC) type = "Anaerobic";
        else if (W.getType() == MIXED) type = "Mixed";
        else type = "Cardio";
        cout << W.getName() << ", " << type << ", " << W.getPrice() << std::endl;
        BaseAction::complete();
    }
}

std::string PrintWorkoutOptions::toString() const { return "workout_options Completed"; }

BaseAction *PrintWorkoutOptions::clone() {
    PrintWorkoutOptions *newPrintWorkoutOptions = new PrintWorkoutOptions();
    newPrintWorkoutOptions->complete();
    return newPrintWorkoutOptions;
}
// endregion

// region PRINT_TRAINER_STATUS
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

BaseAction *PrintTrainerStatus::clone() {
    PrintTrainerStatus *newPrintTrainerStatus = new PrintTrainerStatus(trainerId);
    newPrintTrainerStatus->complete();
    return newPrintTrainerStatus;
}
// endregion

// region PRINT_ACTIONS_LOG
PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Studio &studio) {
    for (BaseAction *action: studio.getActionsLog())
        cout << action->toString() << endl;
    BaseAction::complete();
}

std::string PrintActionsLog::toString() const { return "log Completed"; }

BaseAction *PrintActionsLog::clone() {
    PrintActionsLog *newPrintActionsLog = new PrintActionsLog();
    newPrintActionsLog->complete();
    return newPrintActionsLog;
}
// endregion

// region BACKUP_STUDIO
BackupStudio::BackupStudio() {}

void BackupStudio::act(Studio &studio) {
    //backup: studio's status, trainers, orders, workout options, actions history
    if (!backup) backup = new Studio(studio); // Studio copy constructor
    else *backup = studio; // Studio assignment operator
    BaseAction::complete();
}

std::string BackupStudio::toString() const {
    return "backup Completed"; //never results in an error
}

BaseAction *BackupStudio::clone() {
    BackupStudio *newBackup = new BackupStudio();
    newBackup->complete();
    return newBackup;
}
// endregion

// region RESTORE_STUDIO
RestoreStudio::RestoreStudio() {}

void RestoreStudio::act(Studio &studio) {
    if (backup) {
        studio = *backup; // Studio assignment operator
        BaseAction::complete();
    } else {
        BaseAction::error("Error: No backup available");
        cout << "Error: No backup available" << endl;
    }
}

std::string RestoreStudio::toString() const {
    if (getStatus() == COMPLETED)
        return "restore Completed";
    return "restore " + getErrorMsg();
}

BaseAction *RestoreStudio::clone() {
    RestoreStudio *newRestore = new RestoreStudio();
    if (getStatus() == COMPLETED) newRestore->complete();
    else newRestore->error(getErrorMsg());
    return newRestore;
}
// endregion
