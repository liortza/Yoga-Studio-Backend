#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef std::pair<int, Workout> OrderPair;

class Trainer {
public:
    Trainer(int t_capacity);

    // region RULE OF 5
    virtual ~Trainer(); // destructor
    Trainer(const Trainer &other); // copy constructor
    Trainer(Trainer &&other); // move copy constructor
    const Trainer &operator=(const Trainer &other); // assignment operator
    Trainer &operator=(Trainer &&other); // move assignment operator
    // endregion

    int getCapacity() const;

    int getId() const;

    void addCustomer(Customer *customer);

    void removeCustomer(int id);

    Customer *getCustomer(int id);

    std::vector<Customer *> &getCustomers();

    std::vector<OrderPair> &getOrders();

    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout> &workout_options);

    void openTrainer();

    void closeTrainer();

    int getSalary() const;

    bool isOpen();

    bool wasOpened();

    int getAvailable();

private:
    static int counter;
    int id;
    int capacity;
    int size;
    int salary;
    bool open;
    bool wasOpen;
    std::vector<Customer *> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
    static std::vector<Customer *> &removeCustomerFromVector(std::vector<Customer *> &customersListInput, int removeId);

    static std::vector<OrderPair> &removeOrderFromVector(std::vector<OrderPair> &orderListInput, int removeId);
};


#endif