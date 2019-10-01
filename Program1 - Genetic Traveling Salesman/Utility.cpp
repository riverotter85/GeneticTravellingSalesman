#include "Utility.h"

#include <algorithm>

int random_int(int min, int max) {
    return rand() % (max - min) + min;
}

std::vector<int> random_permutation(int start, int end) {
    std::vector<int> numbers_list;
    for (int i = start; i < end; i++)
        numbers_list.push_back(i);
    random_shuffle(numbers_list.begin(), numbers_list.end());

    return numbers_list;
}