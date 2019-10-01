#pragma once

#include <vector>

template <class T>
int find_index(std::vector<T> list, T element) {
    for (int i = 0; i < list.size(); i++) {
        if (list[i] == element)
            return i;
    }

    return -1; // error not found!
}

int random_int(int min, int max);
std::vector<int> random_permutation(int start, int end);