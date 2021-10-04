#include <set>
#include <iostream>
#include <fstream>

void find_one_and_three_diffs(std::set<size_t>& data, size_t& n_1_diff, size_t& n_3_diff){
    for(auto prev_el = data.begin(), el = prev_el; el != data.end(); ++el, ++prev_el){
        if(el == data.begin()) el++;        
        const auto diff = *el - *prev_el;
        //std::cout << "El: " << *el << " | Prev el: " << *prev_el << " | Diff: " << diff << std::endl;
        if(diff == 1) ++n_1_diff;
        else if(diff == 3) ++n_3_diff;
    }
}

std::set<size_t> read_from_file(const char* filename){
    std::set<size_t> input_set;
    input_set.insert(0);

    std::ifstream input_file(filename);
    if(!input_file){
        std::cerr << "Failed to open file." << std::endl;
        return input_set;
    }

    std::string line;
    while (std::getline(input_file, line)) {
        input_set.insert(std::stoul(line));
    }
    input_set.insert(*(input_set.rbegin())+3);

    return input_set;
}

int main(){
    auto data = read_from_file("input");
    size_t n_1_diff = 0, n_3_diff = 0;
    find_one_and_three_diffs(data, n_1_diff, n_3_diff);
    std::cout << "There are " << n_1_diff << " 1-diff and " << n_3_diff << " 3-diff." << std::endl;
    std::cout << "1-diff * 3-diff = " << n_1_diff*n_3_diff << std::endl;
}