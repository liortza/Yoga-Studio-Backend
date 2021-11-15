#include "../include/Trainer.h"
#include <iostream>

using namespace std;

Trainer::Trainer(int t_capacity) : capacity(t_capacity) {
    open = false;
    size = 0;
    id = counter;
    counter++;
    salary = 0;
}

// Rule of 5


int Trainer::getCapacity() const { return capacity; }

int Trainer::getId() const { return id; }

void Trainer::addCustomer(Customer *customer) {
    if (size < capacity) {
        customersList.push_back(customer);
        size++;
    }
    //TODO: throw error if size == capacity?
}

void Trainer::removeCustomer(int id) {
    // remove from customersList
    for (Customer *customerPtr: customersList) {
        if (customerPtr->getId() == id) {

            size--;
        }
    }

    // remove from ordersList
    //TODO: delete element from middle of vector
}

Customer *Trainer::getCustomer(int id) {
    for (Customer *customerPtr: customersList) {
        if (customerPtr->getId() == id) return customerPtr;
    }
    return nullptr;
    //TODO: throw error if customer doesn't exist?
}

std::vector<Customer *> &Trainer::getCustomers() { return customersList; }

std::vector<OrderPair> &Trainer::getOrders() { return orderList; }

void
Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout> &workout_options) {
    //TODO: ??
}

void Trainer::openTrainer() {
    open = true;
    //TODO: update salary?
}

void Trainer::closeTrainer() {
    open = false;
//    customersList = nullptr;
//    orderList = nullptr;
    cout << "Trainer " << id << "closed. Salary " << salary << "NIS" << endl;
}

int Trainer::getSalary() const { return salary; }

bool Trainer::isOpen() { return open; }
