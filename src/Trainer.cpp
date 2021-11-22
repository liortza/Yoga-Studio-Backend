#include "../include/Trainer.h"
#include <iostream>
#include <vector>

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

    // remove from ordersList
    for (OrderPair order: orderList) {
        if (order.first == id) {

        }
    }
    //TODO: delete element from middle of vector
}

Customer *Trainer::getCustomer(int id) {
    for (Customer *customerPtr: customersList) {
        if (customerPtr->getId() == id) return customerPtr;
    }
    return nullptr;
}

std::vector<Customer *> &Trainer::getCustomers() { return customersList; }

std::vector<OrderPair> &Trainer::getOrders() { return orderList; }

void
Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout> &workout_options) {
    Customer *customer = getCustomer(customer_id);
    if (!customer->getOrdered()) { // place orders of new customers (no duplicates)
        for (int workout_id: workout_ids) {
            OrderPair pair(customer_id, workout_options[workout_id]);
            orderList.push_back(pair);
            customer->setOrdered(true);
        }
    }
}

void Trainer::openTrainer() {
    open = true;
}

void Trainer::closeTrainer() {
    open = false;
    customersList.clear();
    size = 0;
    orderList.clear();
    for (const OrderPair &orderPair: orderList)
        salary += orderPair.second.getPrice();
    cout << "Trainer " << id << "closed. Salary " << salary << "NIS" << endl;
}

int Trainer::getSalary() const { return salary; }

bool Trainer::isOpen() { return open; }

int Trainer::getAvailable() { return capacity - size; }
