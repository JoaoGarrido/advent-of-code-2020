#include <set>
#include <unordered_set>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <memory>

class Group_answer{
    public:
        void addAnswer(const std::string person_answer){
            for(const auto& c: person_answer){
                person_answers.insert(c);
                same_answer.insert(c);
            }
            number_of_answers++;
        }
        size_t getYesAnswers(){
            return same_answer.size();
        }
        size_t getCommonYesAnswer(){
            size_t common_answers = 0;
            for(const auto &c: same_answer){
                if(person_answers.count(c) == number_of_answers) common_answers++;
            }
            return common_answers;
        }
    private:
        size_t number_of_answers = 0;
        std::set<char> same_answer;
        std::unordered_multiset<char> person_answers;
};

std::vector<std::shared_ptr<Group_answer>> read_from_file_to_vec(const char *filename){
    std::vector<std::shared_ptr<Group_answer>> groups_vec;

    std::ifstream input_file(filename);
    if(!input_file){
        std::cerr << "Failed to open file." << std::endl;
        return groups_vec;
    }

    std::string line;
    auto group = std::make_shared<Group_answer>();
    while (std::getline(input_file, line)){
        if(line == ""){
            groups_vec.push_back(group);
            group = std::make_shared<Group_answer>();
        }
        else{
            group->addAnswer(line);
        }
    }

    return groups_vec;
}

int main(){
    auto groups_vec = read_from_file_to_vec("input");

    size_t sum_all = 0;
    for(const auto& group: groups_vec){
        sum_all += group->getYesAnswers();
    }
    std::cout << "Sum of each group yes answers is " << sum_all << std::endl;

    size_t sum_all_common = 0;
    for(const auto& group: groups_vec){
        sum_all_common += group->getCommonYesAnswer();
    }
    std::cout << "Sum of each group common yes answers is " << sum_all_common << std::endl;
}
