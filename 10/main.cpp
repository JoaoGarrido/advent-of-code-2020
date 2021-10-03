#include <set>
#include <iostream>
#include <fstream>

void find_one_and_three_diffs(const std::set<size_t>& data, size_t& n_1_diff, size_t& n_2_diff){
    for(const auto& el:data) {
        std::cout << el << std::endl;
        
        auto& previous_el = el;
    }
}

std::set<size_t> read_from_file(const char* filename){
    std::set<size_t> input_set;
    
    std::ifstream input_file(filename);
    if(!input_file){
        std::cerr << "Failed to open file." << std::endl;
        return input_set;
    }

    std::string line;
    while (std::getline(input_file, line)) {
        input_set.insert(std::stoul(line));
    }

    return input_set;
}

int main(){
    auto data = read_from_file("input");
    size_t n_1_diff = 0, n_2_diff = 0;
    find_one_and_three_diffs(data, n_1_diff, n_2_diff);
}