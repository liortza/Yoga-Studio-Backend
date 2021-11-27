#include "../include/Trainer.h"
#include <iostream>
#include <vector>

using namespace std;

int Trainer::counter = 0; // initialize static counter

Trainer::Trainer(int t_capacity) : capacity(t_capacity) {
    open = false;
    wasOpen = false;
    size = 0;
    id = counter;
    counter++;
    salary = 0;
}

// region RULE OF 5
// destructor
Trainer::~Trainer() { clear(); }

// copy constructor
Trainer::Trainer(const Trainer &other) : id(other.id), capacity(other.capacity), size(other.size), salary(other.salary),
                                         open(other.open), wasOpen(other.wasOpen) {
    for (OrderPair pair: other.orderList) orderList.push_back(pair);
    for (Customer *C: other.customersList) { // deep copy customersList
        customersList.push_back(C->clone());
    }
}

// move copy constructor
Trainer::Trainer(Trainer &&other) : id(other.id), capacity(other.capacity), size(other.size), salary(other.salary),
                                    open(other.open), wasOpen(other.wasOpen), customersList(other.customersList) {
    for (OrderPair pair: other.orderList) orderList.push_back(pair);
    other.customersList.clear();
}

// assignment operator
const Trainer &Trainer::operator=(const Trainer &other) {
    clear();
    id = other.id;
    capacity = other.capacity;
    size = other.size;
    salary = other.salary;
    open = other.open;
    wasOpen = other.wasOpen;
    for (OrderPair pair: other.orderList) orderList.push_back(pair);
    if (!other.customersList.empty()) {
        for (Customer *C: other.customersList) { // deep copy customersList
            customersList.push_back(C->clone());
        }
    }
    return *this;
}

// move assignment operator
Trainer &Trainer::operator=(Trainer &&other) {
    clear();
    id = other.id;
    capacity = other.capacity;
    size = other.size;
    salary = other.salary;
    open = other.open;
    wasOpen = other.wasOpen;
    for (const OrderPair &pair: other.orderList) orderList.push_back(pair);
    customersList = other.customersList;
    other.customersList.clear();
    return *this;
}
// endregion

int Trainer::getCapacity() const { return capacity; }

int Trainer::getId() const { return id; }

void Trainer::addCustomer(Customer *customer) {
    if (size < capacity) {
        customersList.push_back(customer);
        size++;
    }
}

void Trainer::removeCustomer(int id) {
    Customer *toRemove = getCustomer(id);
    if (toRemove != nullptr) {
        toRemove->setOrdered(false);
        salary -= toRemove->getPay();
        toRemove->resetPay();
        size--;

        // create new customersList without removed customer
        vector<Customer *> newCustomersList;
        for (Customer *customer: customersList) {
            if (customer->getId() != id)
                newCustomersList.push_back(customer);
        }
        customersList.clear();
        for (Customer *customer:newCustomersList) customersList.push_back(customer); // customersList <- new vector

        // create new orderList without removed customer's orders
        vector<OrderPair> newOrderList;
        for (const OrderPair& order: orderList) {
            if (order.first != id)
                newOrderList.push_back(order);
        }
        orderList.clear();
        for (const OrderPair& pair:newOrderList) orderList.push_back(pair); // ordersList <- new vector
    }
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
    if (customer != nullptr) {
        salary += customer->getPay();
        for (int workout_id: workout_ids) {
            OrderPair pair(customer_id, workout_options[workout_id]);
            orderList.push_back(pair);
            customer->setOrdered(true);
        }
    }
}

void Trainer::openTrainer() {
    open = true;
    wasOpen = true;
}

void Trainer::closeTrainer() {
    open = false;
    for (Customer *customer: customersList) delete customer;
    customersList.clear();
    size = 0;
    orderList.clear();
    cout << "Trainer " << id << " closed. Salary " << salary << "NIS" << endl;
}

int Trainer::getSalary() const { return salary; }

bool Trainer::isOpen() { return open; }

bool Trainer::wasOpened() { return wasOpen; }

int Trainer::getAvailable() { return capacity - size; }

void Trainer::clear() {
    if (!customersList.empty()) {
        for (Customer *customer: customersList) delete customer;
    }
    customersList.clear();
    orderList.clear();
}
