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
    if (getCustomer(id) != nullptr) {
        customersList = removeCustomerFromVector(customersList, id);
        orderList = removeOrderFromVector(orderList, id);
        getCustomer(id)->setOrdered(false);
        size--;
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
    if (customer != nullptr && !customer->getOrdered()) { // place orders of new customers (no duplicates)
        for (int workout_id: workout_ids) {
            OrderPair pair(customer_id, workout_options[workout_id]);
            orderList.push_back(pair);
            salary += pair.second.getPrice();
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
    cout << "Trainer " << id << "closed. Salary " << salary << "NIS" << endl;
    //todo: erase customers
}

int Trainer::getSalary() const { return salary; }

bool Trainer::isOpen() { return open; }

int Trainer::getAvailable() { return capacity - size; }

std::vector<Customer *> Trainer::removeCustomerFromVector(std::vector<Customer *> customersListInput, int removeId) {
    std::vector<Customer *> CustomersListResult;
    for (Customer *customer: customersListInput) {
        if (customer->getId() != removeId)
            CustomersListResult.push_back(customer);
    }
    return CustomersListResult;
}

std::vector<OrderPair> Trainer::removeOrderFromVector(std::vector<OrderPair> orderListInput, int removeId) {
    std::vector<OrderPair> OrdersListResult;
    for (OrderPair order: orderListInput) {
        if (order.first != removeId)
            OrdersListResult.push_back(order);
    }
    return OrdersListResult;
}
