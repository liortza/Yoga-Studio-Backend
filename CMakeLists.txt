cmake_minimum_required(VERSION 3.16)
project(SPL1)

set(CMAKE_CXX_STANDARD 14)
set(HEADERS include/Action.h include/Trainer.h include/Customer.h include/Studio.h include/Workout.h)

add_executable(SPL1 src/main.cpp src/BaseAction.cpp src/Customer.cpp src/Studio.cpp src/Trainer.cpp src/Workout.cpp src/OpenTrainer.cpp src/Order.cpp src/MoveCustomer.cpp src/Close.cpp src/CloseAll.cpp src/PrintWorkoutOptions.cpp src/PrintTrainerStatus.cpp src/PrintActionsLog.cpp src/BackupStudio.cpp src/RestoreStudio.cpp src/SweatyCustomer.cpp)
