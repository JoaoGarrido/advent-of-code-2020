#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using contained_bag_list_t = std::unordered_map<std::string, size_t>;
using bag_list_t = std::unordered_map<std::string, contained_bag_list_t>;

size_t number_of_bag_colors_that_contain_X(const std::string searching_bag, bag_list_t umm_bags){
    std::unordered_set<std::string> bags_that_contain_shiny;
    bags_that_contain_shiny.insert(searching_bag);
    std::queue<std::string> queue_of_bags_that_contain_searching_bag;
    queue_of_bags_that_contain_searching_bag.push(searching_bag);
    while(!queue_of_bags_that_contain_searching_bag.empty()){
        auto curr_searching_bag = queue_of_bags_that_contain_searching_bag.front();
        for(const auto & bag: umm_bags){
            auto contained_bags = bag.second; 
            if(contained_bags.find(curr_searching_bag) != contained_bags.end()){
                bags_that_contain_shiny.insert(bag.first);
                queue_of_bags_that_contain_searching_bag.push(bag.first);
            }
        }
        queue_of_bags_that_contain_searching_bag.pop();
    }
    return bags_that_contain_shiny.size()-1; //shiny bag doesn't contain itself
}

//return true if contains number and writes the number in addres of pnumber
//return false if doesn't contain number
bool convert_string_to_size_t(std::string str, size_t &pnumber){
    if(std::all_of(str.begin(), str.end(), ::isdigit)){
        pnumber = std::stoul(str);
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
        //std::cout << main_bag_name << std::endl;
        
        contained_bag_list_t contained_bag_list;
        line_stream >> _thrash >> _thrash; //skip bag|bags and contain
        while(line_stream >> aux_1 >> aux_2 >> aux_3 >> _thrash){ //skip bag,|bags,
            //std::cout << "Number: " << aux_1 << " | Bag name: " << aux_2 << " " << aux_3 << std::endl;
            size_t bag_number = 0;
            if(!convert_string_to_size_t(aux_1, bag_number)) break;
            auto bag_name = aux_2 + " " + aux_3;
            std::pair<std::string, size_t> el(bag_name, bag_number);
            contained_bag_list.insert(el);
        }
        std::pair<std::string, contained_bag_list_t> el(main_bag_name, contained_bag_list);
        bag_list.insert(el);
        //std::cout << "NEXT BAG" << std::endl << std::endl;
    }
    return bag_list;
}


int main(){
    auto bag_list = read_from_file("input"); //Read from file
    std::cout << number_of_bag_colors_that_contain_X(std::string("shiny gold"), bag_list) << std::endl;
}