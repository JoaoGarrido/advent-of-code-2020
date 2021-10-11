#include <vector>
#include <iostream>
#include <fstream>
#include <deque>

size_t min_in_deque(std::deque<size_t> contigious_set){
    size_t smallest = UINT64_MAX;
    for(const auto& el:contigious_set){
        if(el < smallest) smallest = el;
    }
    return smallest;
}

size_t max_in_deque(std::deque<size_t> contigious_set){
    size_t largest = 0;
    for(const auto& el:contigious_set){
        if(el > largest) largest = el;
    }
    return largest;
}

size_t deque_sum(std::deque<size_t> contigious_set){
    size_t sum = 0;
    for(const auto& deque_el:contigious_set){
        sum += deque_el;
    }
    return sum;
}

size_t smallest_and_largest_of_contiguous_set(std::vector<size_t> data, size_t invalid_number){
    std::deque<size_t> contigious_set;
    for(const auto& el: data){
        contigious_set.push_front(el);
        auto sum = deque_sum(contigious_set);
        while(1){
            if(sum > invalid_number){
                contigious_set.pop_back();
                sum = deque_sum(contigious_set);
            }
            else if(sum == invalid_number){
                goto found_set;
            }
            else{
                break;
            }
        }
    }
found_set:
    size_t smallest = min_in_deque(contigious_set);
    size_t largest = max_in_deque(contigious_set);
    return smallest + largest;
}

bool find_pair(const size_t index, const size_t preamble_size, const size_t sum, const std::vector<size_t> data) {
    for(size_t pair_0_idx = (index - preamble_size); pair_0_idx < index; pair_0_idx++){
        for(size_t pair_1_idx = (index - preamble_size); pair_1_idx < index; pair_1_idx++){
            if(pair_0_idx != pair_1_idx && (sum == (data[pair_0_idx] + data[pair_1_idx])) ){
                return true;
            }
        }
    }
    return false;
}

size_t first_without_pair(std::vector<size_t> data, size_t preamble_size){
    for(size_t index = preamble_size; index < data.size(); index++){
        if(!find_pair(index, preamble_size, data[index], data)) return data[index];
    }
    return 0;
}

std::vector<size_t> read_from_file(const char* filename){
    std::vector<size_t> input_vec;
    
    std::ifstream input_file(filename);
    if(!input_file){
        std::cerr << "Failed to open file." << std::endl;
        return input_vec;
    }

    std::string line;
    while (std::getline(input_file, line)) {
        input_vec.push_back(std::stoul(line));
    }

    return input_vec;
}

int main(){
    auto input_data = read_from_file("input");
    auto first_invalid_number = first_without_pair(input_data, 25);
    std::cout << "First number without pair is " << first_invalid_number << std::endl;
    std::cout << "Sum of smallest and largest number in contiguous set that adds to " << first_invalid_number << " is " << smallest_and_largest_of_contiguous_set(input_data, first_invalid_number) << std::endl;
}