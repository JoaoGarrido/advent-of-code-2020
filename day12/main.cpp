#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

struct Instruction {
    char type;
    size_t value;
};

enum CardinalPoints {North = 90, South = 270, East = 0, West = 180};

class Point{
    public:
        Point(){
            std::cout << "Pos: " << north_south_pos << "," << east_west_pos << " | Orientation: " << orientation << std::endl;
        };
        Point(CardinalPoints _orientation, long long int _east_west_pos, long long int _north_south_pos) 
            : orientation(_orientation)
            , east_west_pos(_east_west_pos)
            , north_south_pos(_north_south_pos) {};
        void moveCardinal(CardinalPoints point, size_t value){
            if(point == North)      north_south_pos += value;
            else if(point == South) north_south_pos -= value;
            else if(point == East)  east_west_pos += value;
            else if(point == West)  east_west_pos -= value;
            std::cout << "Move Cardinal " << point << " | Pos (" << north_south_pos << "," << east_west_pos << ")" << std::endl;  
        }
        long long int getNorthSoutPos() const {
            return north_south_pos;
        }
        long long int getEastWestPos() const {
            return east_west_pos;
        }
        virtual void moveForward(size_t value){};
        virtual void turnRight(size_t angle){};
        virtual void turnLeft(size_t angle){};

    protected:
        CardinalPoints orientation = East;
        long long int east_west_pos = 0;
        long long int north_south_pos = 0;
};

class WayPoint: public Point {
    public:
        WayPoint(CardinalPoints _orientation, long long int _east_west_pos, long long int _north_south_pos){
            orientation = _orientation;
            east_west_pos = _east_west_pos;
            north_south_pos = _north_south_pos;
            std::cout << "Pos: " << north_south_pos << "," << east_west_pos << " | Orientation: " << orientation << std::endl;
        }
        void turnRight(size_t angle) override {
            if(angle%90 != 0){
                std::cerr << "Error: Bad input. Angle should be divisible per 90" << std::endl;
                return;
            }
            while(angle != 0){
                std::swap(north_south_pos, east_west_pos);
                angle -= 90;
                north_south_pos *= -1;
            }
        }
        void turnLeft(size_t angle) override {
            if(angle%90 != 0){
                std::cerr << "Error: Bad input. Angle should be divisible per 90" << std::endl;
                return;
            }
            while(angle != 0) {
                std::swap(north_south_pos, east_west_pos);
                angle -= 90;
                east_west_pos *= -1;
            }
        }
};

class Ship: public Point{
    public:
        void moveForward(size_t value) override {
            std::cout << "Move Forward -> ";  
            moveCardinal(orientation, value);
        }
        void moveForwardWayPoint(const WayPoint waypoint, size_t value){
            north_south_pos += waypoint.getNorthSoutPos()*value;
            east_west_pos += waypoint.getEastWestPos()*value;
            std::cout << "Move Forward Waypoint" << " | Pos (" << north_south_pos << "," << east_west_pos << ")" << std::endl;  
        }
        void turnRight(size_t angle) override {
            if(angle%90 != 0){
                std::cerr << "Error: Bad input. Angle should be divisible per 90" << std::endl;
                return;
            }
            const int offset = orientation - angle;
            orientation = (CardinalPoints) ((offset % 360 + 360) % 360);
            std::cout << "Turn Right " << angle << " | Orientation " << orientation << std::endl;  
        }
        void turnLeft(size_t angle) override {
            if(angle%90 != 0){
                std::cerr << "Error: Bad input. Angle should be divisible per 90" << std::endl;
                return;
            }
            const int offset = orientation + angle;
            orientation = (CardinalPoints) ((offset % 360 + 360) % 360);
            std::cout << "Turn Left " << angle << " | Orientation " << orientation << std::endl;  
        }
};

std::vector<Instruction> readFromFile(const char* filename){
    std::vector<Instruction> input;

    std::ifstream input_file(filename);
    if(!input_file){
        std::cerr << "Failed to open file." << std::endl;
        return input;
    }

    std::string line;
    while (std::getline(input_file, line)) {
        Instruction i;
        sscanf(line.c_str(), "%c%lu", &i.type, &i.value);
        input.push_back(i);
    }
    return input;
}

void firstSolution(const std::vector<Instruction>& data){
    Ship ship;
    for(const auto instruction:data){
        std::cout << instruction.type << instruction.value << "\t";
        switch (instruction.type){
        case 'F':
            ship.moveForward(instruction.value);            
            break;
        case 'N':
            ship.moveCardinal(North, instruction.value);            
            break;
        case 'S':
            ship.moveCardinal(South, instruction.value);            
            break;
        case 'E':
            ship.moveCardinal(East, instruction.value);            
            break;
        case 'W':
            ship.moveCardinal(West, instruction.value);            
            break;
        case 'R':
            ship.turnRight(instruction.value);            
            break;
        case 'L':
            ship.turnLeft(instruction.value);            
            break;
        default:
            break;
        }
    }
    std::cout << "Manhattan distance is " << abs(ship.getNorthSoutPos()) + abs(ship.getEastWestPos()) << std::endl;
}

void secondSolution(const std::vector<Instruction>& data){
    Ship ship;
    WayPoint waypoint(East, 10, 1);
    for(const auto instruction:data){
        std::cout << instruction.type << instruction.value << "\t";
        switch (instruction.type){
        case 'F':
            ship.moveForwardWayPoint(waypoint, instruction.value);            
            break;
        case 'N':
            waypoint.moveCardinal(North, instruction.value);            
            break;
        case 'S':
            waypoint.moveCardinal(South, instruction.value);            
            break;
        case 'E':
            waypoint.moveCardinal(East, instruction.value);            
            break;
        case 'W':
            waypoint.moveCardinal(West, instruction.value);            
            break;
        case 'R':
            waypoint.turnRight(instruction.value);            
            break;
        case 'L':
            waypoint.turnLeft(instruction.value);            
            break;
        default:
            break;
        }
    }
    std::cout << "Manhattan distance is " << abs(ship.getNorthSoutPos()) + abs(ship.getEastWestPos()) << std::endl;  
}

int main(){
    auto data = readFromFile("input");
    firstSolution(data);
    secondSolution(data);
}