//#ifndef INTERPRET_VARIABLES_H
//#define INTERPRET_VARIABLES_H

#include "Data.h"


class Object{
private:
    int special_code_1 = -1;
    std::pair <int, int> special_code_2 = {0,0};
    C_Object obj_type = variable;
    std::unique_ptr <Data> value;
    bool assignable_flag = false;
public:
    Object(const Object& other){
        obj_type = other.obj_type;
        value.reset(other.value->copy_data());
        special_code_1 = other.special_code_1;
        special_code_2 = other.special_code_2;
        assignable_flag = other.assignable_flag;
    }
    Object( Data* info, C_Object Class, int ID, std::pair <int, int> mem, bool flag = false ) :
        value(info) {
        obj_type = Class;
        special_code_1 = ID;
        special_code_2 = mem;
        assignable_flag = flag;
    }
    Object() = default;
    Object& operator= (const Object& other){
        obj_type = other.obj_type;
        value.reset(other.value->copy_data());
        return *this;
    }
    ~Object() = default;
    template <typename T>
    explicit Object( T info, C_Object Class = constant ){
        obj_type = Class;
        value.reset(new D_Storage<T>(info));
    }

    explicit Object( int info, C_Object Class = constant ){
        obj_type = Class;
        value.reset(new D_Storage<int>(info));
    }
    template <typename T>
    explicit operator T(){
        if (obj_type == constant) {
            return dynamic_cast<D_Storage<T>*>(value.get())->get_info();
        }
        else{
            return *(dynamic_cast<D_Pointer<T*>*>(value.get())->get_info());
        }
    }

    template <typename T>
    T* get_pointer() const {
        if (obj_type == variable) {
            return (dynamic_cast<D_Pointer<T *> *>(value.get()))->get_info();
        }
        else{
            throw std::logic_error("Logic error at Object::get_pointer");
        }
    }
    int get_special_code_1() const {
        return special_code_1;
    }
    bool get_assignable_flag() const {
        return assignable_flag;
    }
    std::pair <int, int> get_special_code_2(){
        return special_code_2;
    }
};

class Variable{
private:
    int type_number = -1;
    std::unique_ptr<Data> value;
    int address = -1;
    bool assignable_flag = true;
    std::queue <int> vars_in_subordination;
    std::pair <int, int> special_for_pointer = {0,0};
    std::string name;
public:

    Variable() = default;
    ~Variable() = default;
    Variable(const Variable& other) :
        value(other.value->copy_data()){
        type_number = other.type_number;
        address = other.address;
        assignable_flag = other.assignable_flag;
        vars_in_subordination = other.vars_in_subordination;
        special_for_pointer = other.special_for_pointer;
        name = other.name;
    }
    template<typename T>
    Variable(int t_number, T v, int addr, std::queue <int>& subordination, bool flag, std::string& v_name) :
    value(new D_Storage<T>(v)){
        type_number = t_number;
        address = addr;
        assignable_flag = flag;
        vars_in_subordination = subordination;
        name = v_name;
    }
    int get_var_type() const{
        return type_number;
    }
    Object cast_variable() const{
        return Object(value->copy_data_pointer(), variable, address, special_for_pointer, assignable_flag);
    }

    template <typename T>
    explicit operator T(){
        return dynamic_cast<D_Storage<T>*>(value.get())->get_info();
    }

};


//#endif //INTERPRET_VARIABLES_H
