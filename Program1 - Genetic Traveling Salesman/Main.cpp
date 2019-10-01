#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <vector>
#include <tuple>
#include <algorithm>

#include "Salesman.h"
#include "SalesmanComparator.h"
#include "Utility.h"

std::vector<Salesman*> determine_crossover_parents(std::vector<Salesman*>* salesmen_list) {
    std::vector<Salesman*> parents;
    SalesmanComparator desc;
    
    // randomly select 10 salesmen from total list
    std::vector<Salesman*> random_salesmen;
    std::vector<int> random_salesmen_indices = random_permutation(0, 10);
    for (int i = 0; i < 10; i++)
        random_salesmen.push_back((*salesmen_list)[random_salesmen_indices[i]]);

    // sort by order of decreasing optimality (path_cost * -1)
    sort(random_salesmen.begin(), random_salesmen.end(), desc);

    // select top 4 from sorted list
    std::vector<Salesman*> top_four_salesmen;
    for (int i = 0; i < 4; i++)
        top_four_salesmen.push_back(random_salesmen[i]);

    // randomly select 2 of these 4 salesmen to crossover
    random_salesmen_indices = random_permutation(0, 4);
    parents.push_back(top_four_salesmen[random_salesmen_indices[0]]);
    parents.push_back(top_four_salesmen[random_salesmen_indices[1]]);

    return parents;
}

std::vector<Salesman*>* next_generation(std::vector<Salesman*>* salesmen_list, int num_cities, std::vector< std::tuple<int, int, int> > paths_list) {
    std::vector<Salesman*>* new_salesmen_list = new std::vector<Salesman*>();
    for (int i = 0; i < 10; i++)
        new_salesmen_list->push_back((*salesmen_list)[i]);

    while (new_salesmen_list->size() != salesmen_list->size()) {
        std::vector<Salesman*> parents = determine_crossover_parents(salesmen_list);
        Salesman* child = Salesman::crossover(parents[0], parents[1], num_cities, 0, paths_list);
        new_salesmen_list->push_back(child);
    }

    return new_salesmen_list;
}

void print_salesmen_generation(std::vector<Salesman*>* salesmen_list, int index, int population_size) {
    std::cout << "Generation #" << index + 1 << " (Best to Worst):\n";
    for (int i = 0; i < population_size; i++) {
        std::cout << "Salesman #" << i + 1 << "\n";
        std::cout << (*salesmen_list)[i]->to_string() << "\n";
    }
    std::cout << "\n";
}

void delete_leftover_salesmen(std::vector<Salesman*>* salesmen_list, std::vector<Salesman*>* new_salesmen_list) {
    for (int i = 0; i < salesmen_list->size(); i++) {
        Salesman* temp = (*salesmen_list)[i];
        bool found = false;
        for (int j = 0; j < new_salesmen_list->size(); j++) {
            if (temp == (*new_salesmen_list)[j])
                found = true;
        }

        if (!found)
            delete temp;
    }
}

// Accept 4 arguments:
//      1) vertex info file
//      2) edge info file
//      3) population size
//      4) num generations
int main(int argc, char* argv[]) {
    std::ifstream in_file;
    std::vector<std::string> cities_list;
    std::vector< std::tuple<int, int, int> > paths_list;

    srand(time(NULL));

    if (argc < 5) {
        std::cout << "ERROR: Insufficient arguments!\n";
        exit(EXIT_FAILURE);
    }

    const int population_size = atoi(argv[3]);
    const int num_generations = atoi(argv[4]);

    // is vertex file path valid?
    in_file.open(argv[1]);
    if (in_file.fail()) {
        std::cout << "ERROR: Invalid path for vertex file!\n";
        exit(EXIT_FAILURE);
    }

    // parse into cities_list
    while (in_file.good()) {
        std::string temp;
        in_file >> temp;
        cities_list.push_back(temp);
    }
    in_file.close();

    // is edge file path valid?
    in_file.open(argv[2]);
    if (in_file.fail()) {
        std::cout << "ERROR: Invalid path for edge file!\n";
        exit(EXIT_FAILURE);
    }

    // parse into paths_list
    while (in_file.good()) {
        std::string src_city;
        std::string dst_city;
        std::string cost_str;
        
        in_file >> src_city;
        in_file >> dst_city;
        in_file >> cost_str;

        int src_index = find_index<std::string>(cities_list, src_city);
        int dst_index = find_index<std::string>(cities_list, dst_city);
        int cost = atoi(cost_str.c_str());

        paths_list.push_back(std::tuple<int, int, int>(src_index, dst_index, cost));
    }
    in_file.close();

    // check that there are at least 10 in population size
    // NOTE: May want to change required value (must be nonnegative though)!!
    if (population_size < 10) {
        std::cout << "ERROR: Insufficient population size!\n";
        exit(EXIT_FAILURE);
    }

    if (num_generations < 1) {
        std::cout << "ERROR: Must provide nonnegative value for number of generations!";
        exit(EXIT_FAILURE);
    }

    // start with salesmen having random path permutations
    std::vector<Salesman*>* salesmen_list = new std::vector<Salesman*>();
    Salesman* temp = NULL;
    for (int i = 0; i < population_size; i++) {
        std::vector<int> city_indices = random_permutation(1, cities_list.size());
        salesmen_list->push_back(new Salesman(0, city_indices, paths_list));
    }

    SalesmanComparator desc; // sort by descending order
    sort(salesmen_list->begin(), salesmen_list->end(), desc);
    print_salesmen_generation(salesmen_list, 0, population_size);

    // crossover and sort for each new generation
    for (int curr_gen = 1; curr_gen < num_generations; curr_gen++) {
        std::vector<Salesman*>* new_salesmen_list = next_generation(salesmen_list, cities_list.size(), paths_list);

        delete_leftover_salesmen(salesmen_list, new_salesmen_list); // prevent any memory leaks
        delete salesmen_list;
        salesmen_list = new_salesmen_list;

        sort(salesmen_list->begin(), salesmen_list->end(), desc);
        print_salesmen_generation(salesmen_list, curr_gen, population_size);
    }

    for (int i = 0; i < population_size; i++)
        delete (*salesmen_list)[i];
    delete salesmen_list;
    exit(EXIT_SUCCESS);
}