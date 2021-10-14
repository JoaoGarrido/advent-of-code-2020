#include <array>
#include <vector>
#include <iostream>
#include <fstream>

enum SeatState {Floor, Empty, Occupied};

//std::array<SeatPlace, 8> adjacentSeats
// 0 1 2
// 3 * 4
// 5 6 7
enum SeatMapPosition {TopLeft = 0, Top, TopRight, Left, Right, BotLeft, Bot, BotRight};

struct SeatPlace {
    size_t row;
    size_t col;
    SeatState state;
};

using seating_system_t = std::vector<std::vector<SeatState> >;

SeatState seatNextIter(SeatState currentSeat, std::array<SeatState, 8> adjacentSeats, size_t leaveRule){
    size_t seat_counter = 0;
    for(const auto& adjSeat: adjacentSeats){
        if(adjSeat == Occupied) seat_counter++;
    }
    if(currentSeat == Empty && seat_counter == 0)
        return Occupied;
    else if(currentSeat == Occupied && seat_counter >= leaveRule)
        return Empty;
    else
        return currentSeat;    
}

constexpr void updateIDs(SeatPlace& currentSeat, size_t& row_id, size_t& col_id, size_t& aux, SeatMapPosition& pos){
    switch (pos){
    case TopLeft:
        col_id = currentSeat.col - aux - 1;
        row_id = currentSeat.row - aux - 1;
        break;
    case Top:
        col_id = currentSeat.col;    
        row_id = currentSeat.row - aux - 1;
        break;
    case TopRight:
        col_id = currentSeat.col + aux + 1;    
        row_id = currentSeat.row - aux - 1;
        break;
    case Left:
        col_id = currentSeat.col - aux - 1;
        row_id = currentSeat.row;
        break;
    case Right:
        col_id = currentSeat.col + aux + 1;
        row_id = currentSeat.row;
        break;
    case BotLeft:
        col_id = currentSeat.col - aux - 1;
        row_id = currentSeat.row + aux + 1;
        break;
    case Bot:
        col_id = currentSeat.col;
        row_id = currentSeat.row + aux + 1;
        break;
    case BotRight:
        col_id = currentSeat.col + aux + 1;
        row_id = currentSeat.row + aux + 1;
        break;
    }
}

void setAdjSeatPlace(const seating_system_t& seatingSystem, const size_t& max_row, const size_t max_col, SeatState& adjSeat, SeatMapPosition adjSeatPos, SeatPlace current_seat_info, bool visible){
    size_t aux = 0;
    size_t row_id, col_id;
    updateIDs(current_seat_info, row_id, col_id, aux, adjSeatPos);
    while(row_id >= 0 && row_id < max_row && col_id >= 0 && col_id < max_col){
        adjSeat = seatingSystem[row_id][col_id];
        if(adjSeat != Floor)    return;
        else if(!visible)       return;
        aux++;
        updateIDs(current_seat_info, row_id, col_id, aux, adjSeatPos);
    }
}


std::array<SeatState, 8> constructAdjSeats(const seating_system_t& seatingSystem, size_t row, size_t col, bool visible){
    std::array<SeatState, 8> adjSeats;
    for(auto& seat: adjSeats) seat = Empty;
    const SeatPlace current_seat_info = {.row = row, .col = col};
    const size_t max_row = seatingSystem.size();
    const size_t max_col = seatingSystem.front().size();

    setAdjSeatPlace(seatingSystem, max_row, max_col, adjSeats[TopLeft], TopLeft, current_seat_info, visible);
    setAdjSeatPlace(seatingSystem, max_row, max_col, adjSeats[TopRight], TopRight, current_seat_info, visible);
    setAdjSeatPlace(seatingSystem, max_row, max_col, adjSeats[Top], Top, current_seat_info, visible);
    
    setAdjSeatPlace(seatingSystem, max_row, max_col, adjSeats[Left], Left, current_seat_info, visible);
    setAdjSeatPlace(seatingSystem, max_row, max_col, adjSeats[Right], Right, current_seat_info, visible);
    
    setAdjSeatPlace(seatingSystem, max_row, max_col, adjSeats[BotLeft], BotLeft, current_seat_info, visible);
    setAdjSeatPlace(seatingSystem, max_row, max_col, adjSeats[BotRight], BotRight, current_seat_info, visible);
    setAdjSeatPlace(seatingSystem, max_row, max_col, adjSeats[Bot], Bot, current_seat_info, visible);

    return adjSeats;
}

seating_system_t seatingSystemNextIter(const seating_system_t& currentSeatingSystem, bool visible){
    size_t seatRule = visible ? 5 : 4;
    seating_system_t nextSeatingSystem;
    for(size_t i = 0; i < currentSeatingSystem.size(); i++){
        std::vector<SeatState> nextSeatingSystemRow;
        for(size_t j = 0; j < currentSeatingSystem[i].size(); j++){
            const auto adjSeats = constructAdjSeats(currentSeatingSystem, i, j, visible);
            SeatState nextSeat = seatNextIter(currentSeatingSystem[i][j], adjSeats, seatRule);
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

size_t iterateUntilStable(seating_system_t seatingSystem, bool visible){
    seating_system_t nextSeatingSystem = seatingSystem;
    do{
        seatingSystem = nextSeatingSystem;
        nextSeatingSystem = seatingSystemNextIter(seatingSystem, visible);
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
    std::cout << "Adj Seats Rule: When stable number of occupied seats is " << iterateUntilStable(data, false) << std::endl;
    std::cout << "Visible Seats Rule: When stable number of occupied seats is " << iterateUntilStable(data, true) << std::endl;
}