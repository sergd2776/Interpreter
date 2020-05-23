//#ifndef INTERPRET_FUNCTIONS_H
//#define INTERPRET_FUNCTIONS_H

#include <memory>
#include <map>
#include <stack>
#include <vector>
#include <tuple>
#include <exception>
#include <queue>

class Function{
private:
    std::map <std::string, int> var_table;
    std::map <std::string, int> type_matching_table;
    std::map <std::string, std::tuple <int, std::queue <int>, bool>> var_parameters;
    int arguments_number, enter_point, return_type, address_space_point;
    int exit_point = -1;
    std::string name;
public:
    std::vector <std::string> fun_parameters;
    std::map <std::string, int>& get_var_table(){
        return var_table;
    }
    std::map <std::string, std::tuple <int, std::queue <int>, bool>>& get_var_parameters_table(){
        return var_parameters;
    }
    std::map <std::string, int>& get_type_matching_table(){
        return type_matching_table;
    }
    std::string& get_fun_name(){
        return name;
    }
    int& get_return_type(){
        return return_type;
    }
    int& get_address_space_point(){
        return address_space_point;
    }
    int& get_arguments_number(){
        return arguments_number;
    }
    int& get_enter_point(){
        return enter_point;
    }
    int& get_exit_point(){
        return exit_point;
    }
};



//#endif //INTERPRET_FUNCTIONS_H
