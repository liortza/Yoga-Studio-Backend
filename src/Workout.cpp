#include <string>
#include "../include/Workout.h"
using namespace std;

Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type):
id(w_id), name(w_name), price(w_price), type(w_type) {};

int Workout::getId() const {return id;}

std::string Workout::getName() const {return name;}
//TODO:we need to check if the string initialized on the heap

int Workout::getPrice() const {return price;}

WorkoutType Workout::getType() const {return type;}

//bool Workout::expensiveThan(Workout *const wk1Ptr, Workout *const &wk2Ptr) {
//    return wk1Ptr->getPrice() > wk2Ptr->getPrice();
//}

bool Workout::expensiveThan(const Workout &wk1, const Workout &wk2) {
    return wk1.getPrice() > wk2.getPrice();
}