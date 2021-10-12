#include <array>
#include <vector>
#include <iostream>
#include <fstream>

enum SeatState {Floor, Empty, Occupied};

using seating_system_t = std::vector<std::vector<SeatState> >;

//std::array<SeatState, 8> adjacentSeats
// 0 1 2
// 3 * 4
// 5 6 7

SeatState seatNextIter(SeatState currentSeat, std::array<SeatState, 8> adjacentSeats){
    size_t seat_counter = 0;
    for(const auto& adjSeat: adjacentSeats){
        if(adjSeat == Occupied) seat_counter++;
    }
    if(currentSeat == Empty && seat_counter == 0)
        return Occupied;
    else if(currentSeat == Occupied && seat_counter >= 4)
        return Empty;
    else
        return currentSeat;    
}

std::array<SeatState, 8> constructAdjSeats(const seating_system_t& seatingSystem, size_t row, size_t col){
    std::array<SeatState, 8> adjSeats;
    for(auto& seat: adjSeats) seat = Empty;
    size_t max_col = seatingSystem.front().size();
    size_t max_row = seatingSystem.size();
    if(col > 0 && row > 0){
        adjSeats[0] = seatingSystem[row-1][col-1];
    }
    if(row > 0){
        adjSeats[1] = seatingSystem[row-1][col];
    }
    if(row > 0 && (col+1) < max_col){
        adjSeats[2] = seatingSystem[row-1][col+1];
    }
    if(col > 0){
        adjSeats[3] = seatingSystem[row][col-1];
    }
    if((col+1) < max_col){
        adjSeats[4] = seatingSystem[row][col+1];
    }
    if(col > 0 && (row+1) < max_row){
        adjSeats[5] = seatingSystem[row+1][col-1];
    }
    if((row+1) < max_row){
        adjSeats[6] = seatingSystem[row+1][col];
    }
    if((col+1) < max_col && (row+1) < max_row){
        adjSeats[7] = seatingSystem[row+1][col+1];
    }
    return adjSeats;
}

seating_system_t seatingSystemNextIter(const seating_system_t& currentSeatingSystem){
    seating_system_t nextSeatingSystem;
    for(size_t i = 0; i < currentSeatingSystem.size(); i++){
        std::vector<SeatState> nextSeatingSystemRow;
        for(size_t j = 0; j < currentSeatingSystem[i].size(); j++){
            const auto adjSeats = constructAdjSeats(currentSeatingSystem, i, j);
            SeatState nextSeat = seatNextIter(currentSeatingSystem[i][j], adjSeats);
            nextSeatingSystemRow.push_back(nextSeat);
        }
        nextSeatingSystem.push_back(nextSeatingSystemRow);
    }
    return nextSeatingSystem;
}

size_t countOccupied(const seating_system_t seatingSystem){
    size_t counter = 0;
    for(const auto& row: seatingSystem)
        for(const auto& seat: row)
            if(seat == Occupied) counter++;
    return counter;
}

size_t iterateUntilStable(seating_system_t seatingSystem){
    seating_system_t nextSeatingSystem = seatingSystem;
    do{
        seatingSystem = nextSeatingSystem;
        nextSeatingSystem = seatingSystemNextIter(seatingSystem);
    }while(seatingSystem != nextSeatingSystem);
    return countOccupied(nextSeatingSystem);
}

seating_system_t readFromFile(const char* filename){
    seating_system_t input;

    std::ifstream input_file(filename);
    if(!input_file){
        std::cerr << "Failed to open file." << std::endl;
        return input;
    }

    std::string line;
    while (std::getline(input_file, line)) {
        std::vector<SeatState> row;
        for(const auto& c: line){
            switch (c){
            case '.':
                row.push_back(Floor);
                break;
            case 'L':
                row.push_back(Empty);
                break;
            case '#':
                row.push_back(Occupied);
                break;
            default:
                std::cout << "Unknown seat state! Exit..." << std::endl;
                break;
            }
        }
        input.push_back(row);
    }

    return input;  
}



int main(){
    auto data = readFromFile("input");
    std::cout << "When stable number of occupied seats is " << iterateUntilStable(data) << std::endl;;
}