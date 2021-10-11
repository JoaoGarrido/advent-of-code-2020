#include <fstream>
#include <iostream>
#include <vector>
#include <string>

std::vector<std::string> read_from_input_file(const char *filename){
    std::vector<std::string> input_vec;

    std::ifstream input_file{filename};
    if(!input_file){
        std::cerr << "Failed to open file." << std::endl;
        return input_vec;
    }

    std::string line;
    while(std::getline(input_file, line)){
        input_vec.push_back(line);
    }

    return input_vec;
}

struct Slopes{
    uint32_t right;
    uint32_t down;
};

int main(){
    auto input_vec = read_from_input_file("input");

    uint32_t rows = input_vec.size();
    uint32_t cols = input_vec[0].size();
    std::cout << rows << " " << cols << std::endl;

    const struct Slopes slopes[5] = {
        {1,1},{3,1},{5,1},{7,1},{1,2}
    };

    std::vector<uint32_t> tree_counters_per_slope;

    for(uint32_t idx = 0; idx < 5; idx++){
        uint32_t x = 0, y = 0;
        uint64_t tree_counter = 0;
        while(y < rows){
            if(input_vec[y][x%cols] == '#') tree_counter++; 
            x+=slopes[idx].right;
            y+=slopes[idx].down;
        };

        if(idx == 1) std::cout << "Tree counter for right 3 and down 1: " << tree_counter << std::endl;

        tree_counters_per_slope.push_back(tree_counter);
    }

    uint64_t multiply_tree_counters = 1;
    for(const auto& tree_counter: tree_counters_per_slope){
        std::cout << tree_counter << std::endl;
        multiply_tree_counters *= tree_counter;
    }

    std::cout << "Tree counter multiply: " << multiply_tree_counters << std::endl;
}