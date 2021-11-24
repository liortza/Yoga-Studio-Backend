#include "../include/Customer.h"

Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id), isOrdered(false) {
    pay = 0;
}

std::string Customer::getName() const { return name; }

int Customer::getId() const { return id; }

bool Customer::getOrdered() { return isOrdered; }

void Customer::setOrdered(bool set) { isOrdered = set; }

int Customer::getPay() { return pay; }
