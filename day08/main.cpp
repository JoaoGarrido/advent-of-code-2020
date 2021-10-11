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
            if(program_counter == programSize()){
                // std::cout << "Terminated" << std::endl;
                end_flag = true;
                return false;
            }
            auto& curr_instruction = program_address_space[program_counter];
            if(curr_instruction.visited){
                //std::cout << "Program counter = " << program_counter << std::endl;
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
        
        bool getEndFlag() const {
            return end_flag;
        }

        std::string getInstruction(size_t pc){
            return program_address_space[pc].name;
        }
        
        void resetCPUState() {
            program_counter = 0;
            global_accum = 0;
            for(auto& inst : program_address_space){
                inst.visited = false;
            }
            end_flag = false;
        }
        
        bool swapNOP_JMP(size_t a){
            auto& curr_instruction = program_address_space[a];
            if(curr_instruction.name == "nop"){
                curr_instruction.name = "jmp";
                //std::cout << "Changed from nop to jmp Instruction -> " << a << std::endl;
                return true;
            }
            else if(curr_instruction.name == "jmp"){
                curr_instruction.name = "nop";
                //std::cout << "Changed from jmp to nop Instruction -> " << a << std::endl;
                return true;
            }
            else return false;
        }
        
        size_t programSize(){
            return program_address_space.size();
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
        bool end_flag = false;
};

size_t fix_program(CPU cpu){
    for(size_t pc = 0; pc < cpu.programSize(); pc++){
        cpu.resetCPUState();
        if(cpu.swapNOP_JMP(pc)){
            while(cpu.execInstruction()){
            }
            if(cpu.getEndFlag()){
                return cpu.getGlobalAccumulator();
            }
            cpu.swapNOP_JMP(pc);
            //std::cout << cpu.getGlobalAccumulator() << std::endl;
        }
    }
    return 0;
}

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
    std::cout << "Value in accum when program terminates is " << fix_program(input_data) << std::endl;
}