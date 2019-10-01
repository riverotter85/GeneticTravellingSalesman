#pragma once

#include "Salesman.h"

class SalesmanComparator {
public:
    bool operator() (Salesman* s1, Salesman* s2) {
        return s1->get_cost() > s2->get_cost();
    }
};