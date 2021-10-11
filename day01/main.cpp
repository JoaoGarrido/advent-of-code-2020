#include <iostream>
#include <fstream>
#include <vector>

auto sum_2_to_x(std::vector<int> input_vec, int x){
    for(const auto& a: input_vec){
        for(const auto& b: input_vec){
            auto sum = a + b;
            if(sum == x){
                std::cout << a << " + " << b << " = " << x << std::endl;
                auto mult = a*b;
                std::cout << a << " * " << b << " = " << mult << std::endl;
                return mult;
            }
        }
    }
    return -1;
}

auto sum_3_to_x(std::vector<int> input_vec, int x){
    for(const auto& a: input_vec){
        for(const auto& b: input_vec){
            for(const auto& c: input_vec){
                auto sum = a + b + c;
                if(sum == x){
                    std::cout << a << " + " << b << " + " << c << " = " << x << std::endl;
                    auto mult = a*b*c;
                    std::cout << a << " * " << b << " * " << c << " = " << mult << std::endl;
                    return 0;
                }
            }
        }
    }
    return -1;
}

int main(){
    //Open file
    std::ifstream input_file{"input"};
    if(!input_file){
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }

    //Read into vector of int
    std::vector<int> input_vec;
    int _n;
    while (input_file >> _n){
        input_vec.push_back(_n);
    }

    //Print vector
    for(const auto& a: input_vec)
        std::cout << a << std::endl;

    auto x = 2020;
    //Find the two integers that sum == 2020 and multiply them
    std::cout << std::endl << "Sum 2 to " << x << std::endl;
    sum_2_to_x(input_vec, 2020);

    //Find three numbers that sum == 2020 and multiply them
    std::cout << std::endl << "Sum 3 to " << x << std::endl;
    sum_3_to_x(input_vec, 2020);

}