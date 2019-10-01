#include "Salesman.h"

#include "Utility.h"

Salesman* Salesman::crossover(Salesman* p1, Salesman* p2, int num_cities, int sc, std::vector< std::tuple<int, int, int> > pl) {
    int crossover_point = random_int(1, num_cities); // allow at least some data from both salesmen to crossover
    std::vector<int> p1_split_indices = p1->get_city_indices(0, crossover_point);
    std::vector<int> p2_split_indices = p2->get_city_indices(crossover_point, num_cities - 1);

    std::vector<int> child_indices = p1_split_indices;
    child_indices.insert(child_indices.end(), p2_split_indices.begin(), p2_split_indices.end());

    replace_duplicate_cities(&child_indices, sc, num_cities - 1);

    return new Salesman(sc, child_indices, pl);
}

void Salesman::replace_duplicate_cities(std::vector<int>* ci, int sc, int path_size) {
    std::vector<int> all_cities;
    std::vector<int> dup_cities;

    for (int i = 0; i < path_size; i++)
        if (i != sc)
            all_cities.push_back(i);

    for (int i = 0; i < path_size; i++) {
        int city_index = (*ci)[i];

        int found_index = find_index<int>(all_cities, city_index);
        if (found_index != -1) {
            all_cities.erase(all_cities.begin() + found_index);
        }
        else {
            dup_cities.push_back(city_index);
        }
    }

    int num_missing_cities = all_cities.size(); // should be same size as all_cities
    std::vector<int> all_cities_per = random_permutation(0, num_missing_cities);
    std::vector<int> dup_cities_per = random_permutation(0, num_missing_cities);
    for (int i = 0; i < num_missing_cities; i++) {
        int replace_index = find_index<int>(*ci, dup_cities[dup_cities_per[i]]);
        (*ci)[replace_index] = all_cities[all_cities_per[i]];
    }
}

std::vector<int> Salesman::get_city_indices(int start, int end) {
    std::vector<int> indices;
    for (int i = start; i < end; i++)
        indices.push_back(cities_path_indices[i]);

    return indices;
}

std::string Salesman::to_string() {
    std::string str = "Path taken: " + std::to_string(starting_city + 1);
    for (int i = 0; i < cities_path_indices.size(); i++)
        str += " -> " + std::to_string(cities_path_indices[i] + 1);
    str += " -> " + std::to_string(starting_city + 1) + "\n";
    str += "Total cost: " + std::to_string(cost * -1);

    return str;
}

int Salesman::compute_cost(std::vector< std::tuple<int, int, int> > paths_list) {
    int sum = 0;
    sum += path_cost(starting_city, cities_path_indices[0], paths_list);
    for (int i = 0; i < cities_path_indices.size() - 1; i++)
        sum += path_cost(cities_path_indices[i], cities_path_indices[i + 1], paths_list);
    sum += path_cost(cities_path_indices[cities_path_indices.size() - 1], starting_city, paths_list);

    sum *= -1; // genetic salesman is a maximization problem
    return sum;
}

int Salesman::path_cost(int src_index, int dst_index, std::vector< std::tuple<int, int, int> > paths_list) {
    for (int i = 0; i < paths_list.size(); i++) {
        std::tuple<int, int, int> tuple = paths_list[i];
        if (std::get<0>(tuple) == src_index && std::get<1>(tuple) == dst_index)
            return std::get<2>(tuple);
    }

    return 0; // no cost if index not found
}