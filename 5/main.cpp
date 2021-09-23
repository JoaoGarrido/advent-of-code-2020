#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <set>
#include <algorithm>

uint32_t select_upper_half(uint32_t min, uint32_t max){
    return (min + max + 1)/2;
}

uint32_t select_lower_half(uint32_t min, uint32_t max){
    return (max - min)/2 + min;
}

class BoardingPass {
    public:
        BoardingPass(const std::string& the_ticket_name) : ticket_name(the_ticket_name){}
        BoardingPass(const std::string& the_ticket_name, const uint32_t the_seat_id) : ticket_name(the_ticket_name), seat_id(the_seat_id){}
        bool calculateSeatData(){
            if(!calcSeatPosition()) return false;
            calcSeatID();
            return true;
        }
        
        uint32_t getSeatID() const {
            return seat_id;
        }

        std::string getTicketName() const {
            return ticket_name;
        }

        void printTicketName() const {
            std::cout << ticket_name << std::endl;
        }

        void printSeatData() const{
            printTicketName();
            std::cout << "Row: " << seat_row << " | Column: " << seat_column << " | Seat ID: " << seat_id << std::endl << std::endl;
        }

        //overload operator to compare class based on seatID
        bool operator<(const BoardingPass& rhs) const {
            return seat_id < rhs.getSeatID();
        }

    private:
        bool calcSeatPosition(){
            //       F       B       F       B       B       F       F
            //0     ->  0   ->  32  ->  32  ->  40  ->  44  ->  44  ->  44
            //127   ->  63  ->  63  ->  47  ->  47  ->  47  ->  45  ->  44
            uint32_t min_row = 0, max_row = 127; 
            uint32_t min_column = 0, max_column = 7;
            for(auto c: ticket_name){
                switch (c){
                case 'F':
                    max_row = select_lower_half(min_row, max_row);
                    break;
                case 'B':
                    min_row = select_upper_half(min_row, max_row); 
                    break;
                case 'L':
                    max_column = select_lower_half(min_column, max_column);
                    break;
                case 'R':
                    min_column = select_upper_half(min_column, max_column);
                    break;
                default:
                    std::cout << "Error: Unknown character in string name. Not possible to find the position." << std::endl;
                    return false;
                    break;
                }
            }
            if(min_row != max_row || min_column != max_column){
                std::cout << "Not possible to find the row and/or column of the ticket" << std::endl;
                return false;
            }
            else{
                seat_row = max_row;
                seat_column = max_column;
                return true;
            }
        }
        void calcSeatID(){
            seat_id = seat_row * 8 + seat_column;
        }
        std::string ticket_name;
        uint32_t seat_row;
        uint32_t seat_column;
        uint32_t seat_id;
};

std::set<BoardingPass> read_from_file_to_vec(const char *filename){
    std::set<BoardingPass> boarding_pass_vec;
    
    std::ifstream input_file(filename);
    if(!input_file){
        std::cerr << "Failed to open file." << std::endl;
        return boarding_pass_vec;
    }

    std::string line;
    while (std::getline(input_file, line)){
        //Insert ordered by using set and overloading operator
        BoardingPass boarding_pass(line);
        boarding_pass.calculateSeatData();
        boarding_pass_vec.insert(boarding_pass);
    }
    
    return boarding_pass_vec;
}


int main(){
    auto boarding_pass_set = read_from_file_to_vec("input");

    for(const auto& boarding_pass: boarding_pass_set){
        boarding_pass.printSeatData();
    }
    std::cout << "Max seat ID is " << boarding_pass_set.rbegin()->getSeatID() << std::endl; //First el of a set

    
    //boarding_pass_set.insert(BoardingPass("a", 743)); //Test if every seat was occupied
    //Search in set for the missing element from a to b
    for(auto it = boarding_pass_set.begin(); it != --boarding_pass_set.end(); it++){
        auto next_it = it;
        next_it++; 
        if((*it).getSeatID() != ((*next_it).getSeatID() - 1)){
            std::cout << "Your seat is " << (*it).getSeatID() + 1 << std::endl;
            break;
        }
        if(next_it == --boarding_pass_set.end()) std::cout << "Couldn't find your seat" << std::endl;
    }
}