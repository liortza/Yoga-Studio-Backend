#include "../include/Action.h"

using namespace std;

PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Studio &studio) {
    for (BaseAction *action: studio.getActionsLog())
        cout << action->toString() << endl;
    BaseAction::complete();
}

std::string PrintActionsLog::toString() const { return "log Completed"; }

