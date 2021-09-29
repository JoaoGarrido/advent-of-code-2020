#include <fstream>
#include <iostream>
#include <vector>

struct Instruction {
    std::string name;
    int64_t value;
    bool visited;
};

class CPU {
    public:
        void addInstruction(std::string name, int64_t value){
            program_address_space.push_back(Instruction{name, value, false});
        }
        bool execInstruction(){
            auto& curr_instruction = program_address_space[program_counter];
            if(curr_instruction.visited){
                return false;
            }
            curr_instruction.visited = true;
            if(curr_instruction.name == "nop"){
                execNOP(curr_instruction);
            }
            else if(curr_instruction.name == "acc") {
                execACC(curr_instruction);
            }
            else if(curr_instruction.name == "jmp") {
                execJMP(curr_instruction);
            }
            else {
                std::cerr << "Error: current instruction unrecognized" << std::endl;
            }
            return true;
        }
        size_t getGlobalAccumulator() const {
            return global_accum;
        }
    private:
        void execNOP(Instruction curr_i){
            program_counter++;
        }
        void execACC(Instruction curr_i){
            global_accum += curr_i.value;
            program_counter++;
        }
        void execJMP(Instruction curr_i){
            program_counter += curr_i.value;
        }
        size_t program_counter = 0;
        size_t global_accum = 0;
        std::vector<Instruction> program_address_space;
};

size_t value_in_accum_before_error(CPU cpu){
    while(cpu.execInstruction()){
        //std::cout << "Running another instruction" << std::endl;
    }
    return cpu.getGlobalAccumulator();
}

CPU read_from_file(const char *filename){
    CPU cpu;
    std::ifstream input_file(filename);
    if(!input_file){
        std::cerr << "Failed to open file." << std::endl;
        return cpu;
    }

    std::string line;
    while (std::getline(input_file, line)) {
        char opcode[3];
        char signal;
        int64_t value;
        sscanf(line.c_str(), "%s %c%ld", opcode, &signal, &value);
        if(signal == '-') value *= -1;
        cpu.addInstruction(opcode, value);
    }
    return cpu;
}

int main(){
    auto input_data = read_from_file("input");
    std::cout << "Value in accum before error is " << value_in_accum_before_error(input_data) << std::endl;
}