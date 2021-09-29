#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using contained_bag_list_t = std::unordered_map<std::string, size_t>;
using bag_list_t = std::unordered_map<std::string, contained_bag_list_t>;

bool bag_contains_X(bag_list_t umm_bags, bag_list_t::iterator curr_bag, const std::string searching_bag){
    static std::unordered_set<std::string> bags_that_contain_shiny;
    bags_that_contain_shiny.insert(searching_bag);
    
    auto bag_name = curr_bag->first;
    auto bag_list = curr_bag->second;

    if(bags_that_contain_shiny.find(bag_name) != bags_that_contain_shiny.end()) return true; //if already inside list contains shiny

    if(bag_list.empty()) return false;      //if it doesn't contain any bags, can't contain shiny
    
    for(const auto& bag: bag_list){
        if(bags_that_contain_shiny.find(bag.first) != bags_that_contain_shiny.end()){
            bags_that_contain_shiny.insert(bag_name);
            return true;
        }
        if(bag_contains_X(umm_bags, umm_bags.find(bag.first), searching_bag)){
                bags_that_contain_shiny.insert(bag_name);
                return true;
        }
    }
    return false;
}

size_t number_of_bag_colors_that_contain_X(const std::string searching_bag, bag_list_t umm_bags){
    size_t bag_counter = 0;
    for (auto bag=umm_bags.begin(); bag!=umm_bags.end(); ++bag){
         if(bag_contains_X(umm_bags, bag, searching_bag)) bag_counter++;
    }
    return bag_counter-1; //shiny bag doesn't contain itself
}

//return true if contains number and writes the number in addres of pnumber
//return false if doesn't contain number
bool convert_string_to_size_t(std::string str, size_t *pnumber){
    if(str.find_first_not_of("0123456789") != std::string::npos){
        *pnumber = std::stoul(str);
        return true;
    }
    return false;
}

bag_list_t read_from_file(const char* filename){
    bag_list_t bag_list;
    std::ifstream input_file(filename);
    if(!input_file){
        std::cerr << "Failed to open file." << std::endl;
        return bag_list;
    }

    std::string line;
    //line:
    //main_bag_name_1 main_bag_name_2 thrash thrash
    while (std::getline(input_file, line)){
        std::string aux_1, aux_2, aux_3, _thrash;
        std::istringstream line_stream;
        line_stream.str(line);
        line_stream >> aux_1 >> aux_2;
        auto main_bag_name = aux_1 + " " + aux_2;
        std::cout << main_bag_name << std::endl;
        
        contained_bag_list_t contained_bag_list;
        line_stream >> _thrash >> _thrash; //skip bag|bags and contain
        while(line_stream >> aux_1 >> aux_2 >> aux_3 >> _thrash){ //skip bag,|bags,
            std::cout << "Number: " << aux_1 << " | Bag name: " << aux_2 << " " << aux_3 << std::endl;
            size_t bag_number = 0;
            if(convert_string_to_size_t(aux_1, &bag_number)) break;
            auto bag_name = aux_2 + " " + aux_3;
            std::pair<std::string, size_t> el(bag_name, bag_number);
            contained_bag_list.insert(el);

        }
        std::pair<std::string, contained_bag_list_t> el(main_bag_name, contained_bag_list);
        bag_list.insert(el);
        std::cout << "NEXT BAG" << std::endl << std::endl;
    }
    return bag_list;
}


int main(){
    auto bag_list = read_from_file("input"); //Read from file
    std::cout << number_of_bag_colors_that_contain_X(std::string("shiny gold"), bag_list) << std::endl;
}