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
                                         open(other.open), wasOpen(other.wasOpen), orderList(other.orderList) {
    for (Customer *customer: other.customersList) { // deep copy customersList
        Customer *myCustomer = customer; // call Customer default copy constructor
        customersList.push_back(myCustomer);
    }
}

// move copy constructor
Trainer::Trainer(Trainer &&other) : id(other.id), capacity(other.capacity), size(other.size), salary(other.salary),
                                    open(other.open), wasOpen(other.wasOpen), orderList(other.orderList),
                                    customersList(other.customersList) {
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
    orderList = other.orderList;
    if (!other.customersList.empty()) {
        for (Customer *customer: other.customersList) { // deep copy customersList
            Customer *myCustomer = customer; // call Customer default copy constructor
            customersList.push_back(myCustomer);
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
    orderList = other.orderList;
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
    if (getCustomer(id) != nullptr) {
        customersList = removeCustomerFromVector(customersList, id);
        orderList = removeOrderFromVector(orderList, id);
        getCustomer(id)->setOrdered(false);
        size--;
        salary -= getCustomer(id)->getPay();
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
    cout << "Trainer " << id << "closed. Salary " << salary << "NIS" << endl;
}

int Trainer::getSalary() const { return salary; }

bool Trainer::isOpen() { return open; }

bool Trainer::wasOpened() { return wasOpen; }

int Trainer::getAvailable() { return capacity - size; }

std::vector<Customer *> &Trainer::removeCustomerFromVector(std::vector<Customer *> &customersListInput, int removeId) {
    std::vector<Customer *> CustomersListResult;
    for (Customer *customer: customersListInput) {
        if (customer->getId() != removeId)
            CustomersListResult.push_back(customer);
    }
    return CustomersListResult;
}

// TODO: make sure correct use of vector reference
std::vector<OrderPair> &Trainer::removeOrderFromVector(std::vector<OrderPair> &orderListInput, int removeId) {
    std::vector<OrderPair> OrdersListResult;
    for (OrderPair order: orderListInput) {
        if (order.first != removeId)
            OrdersListResult.push_back(order);
    }
    return OrdersListResult;
}

void Trainer::clear() {
    if (!customersList.empty())
        for (Customer *customer: customersList) delete customer;
}
