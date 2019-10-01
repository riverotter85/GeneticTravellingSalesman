#pragma once

#include <string>
#include <vector>
#include <tuple>

class Salesman {
private:
    int starting_city;
    std::vector<int> cities_path_indices;
    int cost;
    
    static void replace_duplicate_cities(std::vector<int>* ci, int sc, int path_size);

    std::vector<int> get_city_indices(int start, int end);
    int path_cost(int src_index, int dst_index, std::vector< std::tuple<int, int, int> > paths_list);
    int compute_cost(std::vector< std::tuple<int, int, int> > paths_list);
public:
    int get_cost() {
        return cost;
    }

    std::string to_string();

    static Salesman* crossover(Salesman* p1, Salesman* p2, int num_cities, int sc, std::vector< std::tuple<int, int, int> > pl);

    Salesman(int sc, std::vector<int> cpi, std::vector< std::tuple<int, int, int> > pl) {
        starting_city = sc;
        cities_path_indices = cpi;
        cost = compute_cost(pl);
    }
    ~Salesman() {
    }
};