#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ios>

class Generic_Password {
    public:
        virtual void read_pw_from_line(std::string line) = 0;
        virtual bool is_valid() = 0;
    protected:
        std::string password;
        char policy_char;
};

class Password_min_max_policy: public Generic_Password {
    public:
        void read_pw_from_line(std::string line){
            std::istringstream iss(line);
            char _;
            iss >> min >> _ >> max >> policy_char >> _ >> password;
            //std::cout << min << "-" << max << " " << policy_char << ": " << password << std::endl; 
        }
        bool is_valid(){
            uint32_t char_count = 0;
            for(char a: password){
                if(a == policy_char)
                    char_count++;
            }
            if(char_count < min || char_count > max){
                return false;
            }
            else
                return true;
        }
    private:
        uint32_t min;
        uint32_t max;
};

class Password_index_policy: public Generic_Password {
    public:
        void read_pw_from_line(std::string line){
            std::istringstream iss(line);
            char _;
            iss >> index_1 >> _ >> index_2 >> policy_char >> _ >> password;
        }
        bool is_valid(){
            uint8_t valid_at_index = (password[index_1-1] == policy_char) + (password[index_2-1] == policy_char);
            if(valid_at_index == 1) return true;
            else return false;
        }        
    private:
        uint32_t index_1;
        uint32_t index_2;
};

template<typename T> std::vector<T> read_password_from_file(std::ifstream& input_file){
    std::string line;
    std::vector<T> password_vec;
    while(std::getline(input_file, line)){
        T curr_pw;
        curr_pw.read_pw_from_line(line);
        password_vec.push_back(curr_pw);
    }
    return password_vec;
}

template <typename T> uint32_t count_valid_pw(std::vector<T> password_vec){
    uint32_t valid_pw_count = 0;
    for(T pw: password_vec){
        if(pw.is_valid())
            valid_pw_count++;
    }
    return valid_pw_count;
}

int main(){
    std::ifstream input_file{"input"};
    if(!input_file){
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }

    std::string line;

    auto password_vec_1 = read_password_from_file<Password_min_max_policy>(input_file);
    auto valid_pw_count_1 = count_valid_pw<Password_min_max_policy>(password_vec_1);


    input_file.clear(); //clear eof status
    input_file.seekg(0, std::ios::beg); //reset to begin of file stream

    auto password_vec_2 = read_password_from_file<Password_index_policy>(input_file);
    auto valid_pw_count_2 = count_valid_pw<Password_index_policy>(password_vec_2);


    std::cout << "Valid Passwords for policy min max: " << valid_pw_count_1 << std::endl;
    std::cout << "Valid Passwords for position policy: " << valid_pw_count_2 << std::endl;
}