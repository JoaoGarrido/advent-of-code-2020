#include <string>
#include <array>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>

void print_passport(std::map<std::string, std::string> passport){
    std::cout << "Passport Info | Number of fields: " << passport.size() << std::endl;
    for(const auto& el: passport){
        std::cout << "Key = " << el.first << " | Value = " << el.second << std::endl;
    }
    std::cout << std::endl << std::flush;
}

std::vector<std::map<std::string, std::string>> read_passport_vec_from_file(const char *filename){
    std::vector<std::map<std::string, std::string>> passport_vec;
    std::string key, value;
    
    std::ifstream input_file{filename};
    if(!input_file){
        std::cerr << "Failed to open file." << std::endl;
        return passport_vec;
    }

    std::string line, token;
    std::map<std::string, std::string> passport;
    while(std::getline(input_file, line)){
        char key[100], value[100];
        if(line != ""){
            std::istringstream line_stream;
            line_stream.str(line);
            while(line_stream >> token){
                sscanf(token.c_str(), "%[^:]:%s", key, value);
                passport.insert(std::make_pair(std::string(key), std::string(value)));
            }
        }
        else{
            passport_vec.push_back(passport);
            passport.clear();
        }
    }
    passport_vec.push_back(passport);

    return passport_vec;
}

bool is_byr_valid(std::string value){
    if(value.size() != 4) return false;
    int int_value = std::stoi(value);
    if(int_value < 1920 || int_value > 2002) return false;
    return true; 
}

bool is_iyr_valid(std::string value){
    if(value.size() != 4) return false;
    int int_value = std::stoi(value);
    if(int_value < 2010 || int_value > 2020) return false;
    return true; 
}

bool is_eyr_valid(std::string value){
    if(value.size() != 4) return false;
    int int_value = std::stoi(value);
    if(int_value < 2020 || int_value > 2030) return false;
    return true; 
}

bool is_hgt_valid(std::string value){
    auto cm_pos = value.find("cm");
    auto in_pos = value.find("in");
    if(cm_pos != std::string::npos){
        auto height = std::stoi(value.substr(0, value.size() - cm_pos + 1));
        if(height < 150 || height > 193) return false;
        else return true;
    }
    else if(in_pos != std::string::npos){
        auto height = std::stoi(value.substr(0, value.size() - in_pos + 1));
        if(height < 59 || height > 76) return false;
        else return true;
    }
    else{
        return false;
    }
}

bool is_hcl_valid(std::string value){
    if(value[0] != '#') return false;
    else if(value.size() != 7) return false;
    std::string substr = value.substr(1, value.size());
    for(auto c: substr){
        if(!std::isxdigit(c)) return false;
    }
    return true;
}

bool is_ecl_valid(std::string value){
    if(value == "amb" 
        || value == "blu" 
        || value == "brn" 
        || value == "gry" 
        || value == "grn" 
        || value == "hzl" 
        || value == "oth") return true;
    else return false;
}

bool is_pid_valid(std::string value){
    if(value.size() != 9) return false;
    for(auto c: value){
        if(!std::isdigit(c)) return false;
    }
    return true;
}

bool is_valid(std::map<std::string, std::string> passport, bool strict_rules){
    std::array<std::string, 8> fields = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid", "cid"};
    
    if(passport.size() < (fields.size() - 1)) return false;
    if(passport.size() == (fields.size() - 1) && passport.find(fields.back()) != passport.end()) return false;
    
    if(!strict_rules) return true;

    for(uint32_t idx = 0; idx < (fields.size() - 1); idx++){
        const auto& el = passport.find(fields[idx]);
        switch (idx){
        case 0:
            if(is_byr_valid(el->second) == false) return false;
            break;
        case 1:
            if(is_iyr_valid(el->second) == false) return false;
            break;
        case 2:
            if(is_eyr_valid(el->second) == false) return false;
            break;
        case 3:
            if(is_hgt_valid(el->second) == false) return false;
            break;
        case 4:
            if(is_hcl_valid(el->second) == false) return false;
            break;
        case 5:
            if(is_ecl_valid(el->second) == false) return false;
            break;
        case 6:
            if(is_pid_valid(el->second) == false) return false;
            break;
        default:
            return false;
            break;
        }
    }
    return true;
    
}

int main(){
    auto passport_vec = read_passport_vec_from_file("input");
    uint32_t valid_passports = 0, valid_passports_strict = 0;
    for(const auto& passport: passport_vec){
        valid_passports += is_valid(passport, false);
        valid_passports_strict += is_valid(passport, true);
    }
    std::cout << "Number of valid passports is " << valid_passports << std::endl;
    std::cout << "Number of valid passports with strict rules is " << valid_passports_strict << std::endl;
}