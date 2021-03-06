#ifndef INTERPRET_COMMANDS_H
#define INTERPRET_COMMANDS_H

#include "Variables.h"


typedef std::vector <std::shared_ptr<Variable>> HDD;
typedef std::stack <Function> RAM;
typedef std::map <std::string, Function> Table;
typedef std::map <std::string, std::tuple <int, std::queue <int>, std::stack <int>, bool>> Par_Table;

void RefactorMemory(HDD& Address_Space, int refactoring_point){
    std::queue <int> for_massive;
    std::stack <int> types;
    int type_id;
    for (int i = refactoring_point; i < Address_Space.size(); i++){
        type_id = Address_Space[i].get()->get_type_number();
        types = Address_Space[i].get()->get_type_pointing();
        for_massive = Address_Space[i].get()->get_vars_in_subordination();
        if (!for_massive.empty()){
            int* value = Address_Space[i].get()->cast_variable().get_pointer<int>();
            *value = Address_Space.size();
            int a = for_massive.front();
            int b = types.top();
            Address_Space[i].get()->get_pointer_borders() = std::make_pair(Address_Space.size(), Address_Space.size()+a-1);
            for_massive.pop();
            types.pop();
            for (int j = 1; j <= a; j++){
                switch (b){
                    case 1:
                        Address_Space.emplace_back(std::make_shared<Variable>(b, char(), Address_Space.size(),
                                for_massive,types, for_massive.empty(), "@"));
                        break;
                    case 2:
                        Address_Space.emplace_back(std::make_shared<Variable>(b, u_char(), Address_Space.size(),
                                for_massive,types, for_massive.empty(), "@"));
                        break;
                    case 3:
                        Address_Space.emplace_back(std::make_shared<Variable>(b, short(), Address_Space.size(),
                                for_massive,types, for_massive.empty(), "@"));
                        break;
                    case 4:
                        Address_Space.emplace_back(std::make_shared<Variable>(b, u_short(), Address_Space.size(),
                                for_massive,types, for_massive.empty(), "@"));
                        break;
                    case 5:
                        Address_Space.emplace_back(std::make_shared<Variable>(b, int(), Address_Space.size(),
                                for_massive,types, for_massive.empty(), "@"));
                        break;
                    case 6:
                        Address_Space.emplace_back(std::make_shared<Variable>(b, u_int(), Address_Space.size(),
                                for_massive,types, for_massive.empty(), "@"));
                        break;
                    case 7:
                        Address_Space.emplace_back(std::make_shared<Variable>(b, long(), Address_Space.size(),
                                for_massive,types, for_massive.empty(), "@"));
                        break;
                    case 8:
                        Address_Space.emplace_back(std::make_shared<Variable>(b, u_long(), Address_Space.size(),
                                for_massive,types, for_massive.empty(), "@"));
                        break;
                    case 9:
                        Address_Space.emplace_back(std::make_shared<Variable>(b, longlong(), Address_Space.size(),
                                for_massive,types, for_massive.empty(), "@"));
                        break;
                    case 10:
                        Address_Space.emplace_back(std::make_shared<Variable>(b, u_longlong(), Address_Space.size(),
                                for_massive,types, for_massive.empty(), "@"));
                        break;
                    case 11:
                        Address_Space.emplace_back(std::make_shared<Variable>(b, float(), Address_Space.size(),
                                for_massive,types, for_massive.empty(), "@"));
                        break;
                    case 12:
                        Address_Space.emplace_back(std::make_shared<Variable>(b, double(), Address_Space.size(),
                                for_massive,types, for_massive.empty(), "@"));
                        break;
                    case 13:
                        Address_Space.emplace_back(std::make_shared<Variable>(b, longdouble (), Address_Space.size(),
                                for_massive,types, for_massive.empty(), "@"));
                        break;
                    case 14:
                        Address_Space.emplace_back(std::make_shared<Variable>(b, std::string(), Address_Space.size(),
                                for_massive,types, for_massive.empty(), "@"));
                        break;
                    default:
                        Address_Space.emplace_back(std::make_shared<Variable>(b, int(), Address_Space.size(),
                                for_massive,types, for_massive.empty(), "@"));
                        break;
                }
            }
            types = {};
            for_massive = {};
        }
    }
}

template <typename T>
T Read(){
    T val;
    std::cin >> val;
    return val;
}

template <typename T>
void Write(T val){
    std::cout << val << std::endl;
}

class Commander{
public:
    virtual ~Commander() = default;
    virtual int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const = 0;
};

class Push: public Commander{
private:
    Object value;
    Lexeme lex;
public:
    Push() = default;
    explicit Push(Lexeme& lexeme, int& a){
        lex = lexeme;
        switch (lexeme.type){
            case LexemDecIntConst:
                value = Object(lexeme.GetDecInt());
                a = 5;
                break;
            case LexemOctIntConst:
                value = Object(lexeme.GetOctInt());
                a = 5;
                break;
            case LexemHexIntConst:
                value = Object(lexeme.GetHexInt());
                a = 5;
                break;
            case LexemCharConst:
                value = Object(lexeme.GetChar());
                a = 1;
                break;
            case LexemStringConst:
                value = Object(lexeme.GetString());
                a = 14;
                break;
            case LexemDoubleConst:
                value = Object(lexeme.GetDouble());
                a = 12;
                break;
            default:
                throw std::logic_error("Logic error in Command_Push");
        }
    }
    explicit Push(int point){
        value = Object(point);
        lex.type = LexemDecIntConst;
    }
    explicit Push(Lexeme& lexeme){
        lex = lexeme;
    }
    int Execute ( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override{
        if (lex.type != LexemIdentifier){
            stack.push(value);
        }
        else {
            const Variable& v = *(Address_Space[function_stack.top().get_var_table().at(lex.value)].get());
            stack.push(v.cast_variable());
        }
        return EIP+1;
    }
};

class Pop: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        stack.pop();
        return EIP+1;
    }
};

template <typename T1, typename T2>
class Add: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(value_1 + value_2));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class Sub: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(value_1 - value_2));
        return EIP+1;
    }
public:
};

template <typename T1, typename T2>
class Mul: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(value_1 * value_2));
        return EIP+1;
    }
public:
};

template <typename T1, typename T2>
class Div: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(value_1 / value_2));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class Mod: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(value_1 % value_2));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class MoveLeft: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(value_1 << value_2));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class MoveRight: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(value_1 >> value_2));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class Less: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(value_1 < value_2 ? 1 : 0));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class LessEqual: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(value_1 <= value_2 ? 1 : 0));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class Greater: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(value_1 > value_2 ? 1 : 0));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class GreaterEqual: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(value_1 >= value_2 ? 1 : 0));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class Equal: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(value_1 == value_2 ? 1 : 0));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class NotEqual: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(value_1 != value_2 ? 1 : 0));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class ByteAnd: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(value_1 & value_2));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class ByteXor: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(value_1 ^ value_2));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class ByteOr: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(value_1 | value_2));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class LogicalAnd: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(value_1 && value_2 ? 1 : 0));
        return EIP + 1;
    }
};

template <typename T1, typename T2>
class LogicalOr: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(value_1 || value_2 ? 1 : 0));
        return EIP+1;
    }
};

template <typename T1, typename T2, typename T3>
class Conditional: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T3 value_3 = static_cast<T3>(stack.top());
        stack.pop();
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(value_1 ? value_2 : value_3));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class Equally: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        if ((stack.top().get_special_code_1() == -1) || (!stack.top().get_assignable_flag())){
            ///////////////////////////////////////////////////////////////////////
            throw "Expression is not assignable";
            ///////////////////////////////////////////////////////////////////////
        }
        T1* value_1 = stack.top().get_pointer<T1>();
        stack.pop();
        *value_1 = value_2;
        stack.push(Object((*value_1) = value_2));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class AddEqual: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        if ((stack.top().get_special_code_1() == -1) || (!stack.top().get_assignable_flag())){
            ///////////////////////////////////////////////////////////////////////
            throw "Expression is not assignable";
            ///////////////////////////////////////////////////////////////////////
        }
        T1* value_1 = stack.top().get_pointer<T1>();
        stack.pop();
        stack.push(Object(*value_1 += value_2));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class SubEqual: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        if ((stack.top().get_special_code_1() == -1) || (!stack.top().get_assignable_flag())){
            ///////////////////////////////////////////////////////////////////////
            throw "Expression is not assignable";
            ///////////////////////////////////////////////////////////////////////
        }
        T1* value_1 = stack.top().get_pointer<T1>();
        stack.pop();
        stack.push(Object(*value_1 -= value_2));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class MulEqual: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        if ((stack.top().get_special_code_1() == -1) || (!stack.top().get_assignable_flag())){
            ///////////////////////////////////////////////////////////////////////
            throw "Expression is not assignable";
            ///////////////////////////////////////////////////////////////////////
        }
        T1* value_1 = stack.top().get_pointer<T1>();
        stack.pop();
        stack.push(Object(*value_1 *= value_2));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class DivEqual: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        if ((stack.top().get_special_code_1() == -1) || (!stack.top().get_assignable_flag())){
            ///////////////////////////////////////////////////////////////////////
            throw "Expression is not assignable";
            ///////////////////////////////////////////////////////////////////////
        }
        T1* value_1 = stack.top().get_pointer<T1>();
        stack.pop();
        stack.push(Object(*value_1 /= value_2));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class ModEqual: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        if ((stack.top().get_special_code_1() == -1) || (!stack.top().get_assignable_flag())){
            ///////////////////////////////////////////////////////////////////////
            throw "Expression is not assignable";
            ///////////////////////////////////////////////////////////////////////
        }
        T1* value_1 = stack.top().get_pointer<T1>();
        stack.pop();
        stack.push(Object(*value_1 %= value_2));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class MoveLeftEqual: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        if ((stack.top().get_special_code_1() == -1) || (!stack.top().get_assignable_flag())){
            ///////////////////////////////////////////////////////////////////////
            throw "Expression is not assignable";
            ///////////////////////////////////////////////////////////////////////
        }
        T1* value_1 = stack.top().get_pointer<T1>();
        stack.pop();
        stack.push(Object(*value_1 <<= value_2));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class MoveRightEqual: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        if ((stack.top().get_special_code_1() == -1) || (!stack.top().get_assignable_flag())){
            ///////////////////////////////////////////////////////////////////////
            throw "Expression is not assignable";
            ///////////////////////////////////////////////////////////////////////
        }
        T1* value_1 = stack.top().get_pointer<T1>();
        stack.pop();
        stack.push(Object(*value_1 >>= value_2));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class LogicalAndEqual: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        if ((stack.top().get_special_code_1() == -1) || (!stack.top().get_assignable_flag())) {
            ///////////////////////////////////////////////////////////////////////
            throw "Expression is not assignable";
            ///////////////////////////////////////////////////////////////////////
        }
        T1 *value_1 = stack.top().get_pointer<T1>();
        stack.pop();
        stack.push(Object(*value_1 &= value_2));
        return EIP + 1;
    }
};

template <typename T1, typename T2>
class LogicalXorEqual: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        if ((stack.top().get_special_code_1() == -1) || (!stack.top().get_assignable_flag())){
            ///////////////////////////////////////////////////////////////////////
            throw "Expression is not assignable";
            ///////////////////////////////////////////////////////////////////////
        }
        T1* value_1 = stack.top().get_pointer<T1>();
        stack.pop();
        stack.push(Object(*value_1 ^= value_2));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class LogicalOrEqual: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        if ((stack.top().get_special_code_1() == -1) || (!stack.top().get_assignable_flag())){
            ///////////////////////////////////////////////////////////////////////
            throw "Expression is not assignable";
            ///////////////////////////////////////////////////////////////////////
        }
        T1* value_1 = stack.top().get_pointer<T1>();
        stack.pop();
        stack.push(Object(*value_1 |= value_2));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class Comma: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object((value_1 , value_2)));
        return EIP+1;
    }
};

template <typename T>
class UnaryPlus: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T value_1 = static_cast<T>(stack.top());
        stack.pop();
        stack.push(Object(+value_1));
        return EIP+1;
    }
};

template <typename T>
class UnaryMinus: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T value_1 = static_cast<T>(stack.top());
        stack.pop();
        stack.push(Object(-value_1));
        return EIP+1;
    }
};

template <typename T>
class PrefixIncrement: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        if (stack.top().get_special_code_1() == -1){
            ///////////////////////////////////////////////////////////////////////
            throw "Expression is not assignable";
            ///////////////////////////////////////////////////////////////////////
        }
        T* value_1 = stack.top().get_pointer<T>();
        stack.pop();
        stack.push(Object(++(*value_1)));
        return EIP+1;
    }
};

template <typename T>
class PrefixDecrement: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        if (stack.top().get_special_code_1() == -1){
            ///////////////////////////////////////////////////////////////////////
            throw "Expression is not assignable";
            ///////////////////////////////////////////////////////////////////////
        }
        T* value_1 = stack.top().get_pointer<T>();
        stack.pop();
        stack.push(Object(--(*value_1)));
        return EIP+1;
    }
};

template <typename T>
class PostfixIncrement: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        if (stack.top().get_special_code_1() == -1){
            ///////////////////////////////////////////////////////////////////////
            throw "Expression is not assignable";
            ///////////////////////////////////////////////////////////////////////
        }
        T* value_1 = stack.top().get_pointer<T>();
        stack.pop();
        stack.push(Object((*value_1)++));
        return EIP+1;
    }
};

template <typename T>
class PostfixDecrement: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        if (stack.top().get_special_code_1() == -1){
            ///////////////////////////////////////////////////////////////////////
            throw "Expression is not assignable";
            ///////////////////////////////////////////////////////////////////////
        }
        T* value_1 = stack.top().get_pointer<T>();
        stack.pop();
        stack.push(Object((*value_1)--));
        return EIP+1;
    }
};

template <typename T>
class ByteNot: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T value_1 = static_cast<T>(stack.top());
        stack.pop();
        stack.push(Object(~value_1));
        return EIP+1;
    }
};

template <typename T>
class Not: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T value_1 = static_cast<T>(stack.top());
        stack.pop();
        stack.push(Object(!static_cast<bool>(value_1) ? 1 : 0));
        return EIP+1;
    }
};

class PointerTo: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        int value_1 = stack.top().get_special_code_1();
        if (value_1 == -1){
            ///////////////////////////////////////////////////////////////////////
            throw "Trying to get a pointer of a temporary value";
            ///////////////////////////////////////////////////////////////////////
        }
        stack.pop();
        stack.push(Object(value_1));
        return EIP+1;
    }
};

class RemovePointer: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        int value_1 = static_cast<int>(stack.top());
        if (value_1 > Address_Space.size() - 1){
            ///////////////////////////////////////////////////////////////////////
            throw "Trying to access nonexistent address";
            ///////////////////////////////////////////////////////////////////////
        }
        stack.pop();
        stack.push(Object(Address_Space[value_1]->cast_variable()));
        return EIP+1;
    }
};

class SizeofType: public Commander{
private:
    int value_1;
public:
    explicit SizeofType(int& a){
        //std::cout << "SizeofType" << std::endl;
        value_1 = a;
    }
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        int size;
        if (value_1 == 0){
            throw "Can't get sizeof void";
        }
        else if ((value_1 == 1) || (value_1 == 2)){
            size = 1;
        }
        else if ((value_1 == 3) || (value_1 == 4)){
            size = 2;
        }
        else if ((value_1 == 5) || (value_1 == 6) || (value_1 == 7) || (value_1 == 8)){
            size = 4;
        }
        else if ((value_1 == 9) || (value_1 == 10)){
            size = 8;
        }
        else if (value_1 == 11){
            size = 4;
        }
        else if (value_1 == 12){
            size = 8;
        }
        else if (value_1 == 13){
            size = 10;
        }
        else{
            size = 4;
        }
        stack.push(Object(size));
        return EIP+1;
    }
};

template <typename T>
class SizeofExpression: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T value_1 = static_cast<T>(stack.top());
        stack.pop();
        stack.push(Object(sizeof(value_1)));
        return EIP+1;
    }
};

template <typename T1, typename T2>
class TypeCast: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_1 = static_cast<T2>(stack.top());
        stack.pop();
        stack.push(Object((T1) value_1));
        return EIP+1;
    }
};

template <typename T>
class Addressing: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T value_2 = static_cast<T>(stack.top());
        stack.pop();
        int value_1 = static_cast<int>(stack.top());
        if ((stack.top().get_special_code_2().first > value_2 + value_1) || (stack.top().get_special_code_2().second < value_2 + value_1)){
            ///////////////////////////////////////////////////////////////////
            throw "Runtime error while trying to access massive";
            ///////////////////////////////////////////////////////////////////
        }
        stack.pop();
        stack.push(Address_Space[value_1 + value_2]->cast_variable());
        return EIP+1;
    }
};

class If: public Commander {
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        int value_2 = static_cast<int>(stack.top());
        stack.pop();
        int value_1 = static_cast<int>(stack.top());
        stack.pop();
        if (!value_1) {
            return value_2;
        } else {
            return EIP+1;
        }
    }
};

class Goto: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        int value_1 = static_cast<int>(stack.top());
        stack.pop();
        return value_1;
    }
};

class Call: public Commander{
public:
    explicit Call(std::string s){
        //std::cout << "Call" << std::endl;
        fun_name = s;
    }
private:
    std::string fun_name;
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        Function p_s = table[fun_name];
        function_stack.push(p_s);
        function_stack.top().get_address_space_point() = Address_Space.size();
        if (function_stack.top().get_fun_name() != "main") {
            function_stack.top().get_exit_point() = EIP + 1;
        }
        std::string var_name;
        Par_Table p_table = function_stack.top().get_var_parameters_table();
        for (auto iter = function_stack.top().get_var_table().begin(); iter != function_stack.top().get_var_table().end();
        iter++){
            var_name = iter->first;
            int type_state = std::get<0>(p_table[var_name]);
            switch (type_state){
                case 1:
                    Address_Space.emplace_back(std::make_shared<Variable>(type_state, char(), Address_Space.size(),
                            std::get<1>(p_table[var_name]),std::get<2>(p_table[var_name]), std::get<3>(p_table[var_name]),
                                    var_name));
                    break;
                case 2:
                    Address_Space.emplace_back(std::make_shared<Variable>(type_state, u_char(), Address_Space.size(),
                            std::get<1>(p_table[var_name]),std::get<2>(p_table[var_name]), std::get<3>(p_table[var_name]),
                                    var_name));
                    break;
                case 3:
                    Address_Space.emplace_back(std::make_shared<Variable>(type_state, short(), Address_Space.size(),
                            std::get<1>(p_table[var_name]),std::get<2>(p_table[var_name]), std::get<3>(p_table[var_name]),
                                    var_name));
                    break;
                case 4:
                    Address_Space.emplace_back(std::make_shared<Variable>(type_state, u_short(), Address_Space.size(),
                            std::get<1>(p_table[var_name]),std::get<2>(p_table[var_name]), std::get<3>(p_table[var_name]),
                                    var_name));
                    break;
                case 5:
                    Address_Space.emplace_back(std::make_shared<Variable>(type_state, int(), Address_Space.size(),
                            std::get<1>(p_table[var_name]),std::get<2>(p_table[var_name]), std::get<3>(p_table[var_name]),
                                    var_name));
                    break;
                case 6:
                    Address_Space.emplace_back(std::make_shared<Variable>(type_state, u_int(), Address_Space.size(),
                            std::get<1>(p_table[var_name]),std::get<2>(p_table[var_name]), std::get<3>(p_table[var_name]),
                                    var_name));
                    break;
                case 7:
                    Address_Space.emplace_back(std::make_shared<Variable>(type_state, long(), Address_Space.size(),
                            std::get<1>(p_table[var_name]),std::get<2>(p_table[var_name]), std::get<3>(p_table[var_name]),
                                    var_name));
                    break;
                case 8:
                    Address_Space.emplace_back(std::make_shared<Variable>(type_state, u_long(), Address_Space.size(),
                            std::get<1>(p_table[var_name]),std::get<2>(p_table[var_name]), std::get<3>(p_table[var_name]),
                                    var_name));
                    break;
                case 9:
                    Address_Space.emplace_back(std::make_shared<Variable>(type_state, longlong(), Address_Space.size(),
                            std::get<1>(p_table[var_name]),std::get<2>(p_table[var_name]), std::get<3>(p_table[var_name]),
                                    var_name));
                    break;
                case 10:
                    Address_Space.emplace_back(std::make_shared<Variable>(type_state, u_longlong(), Address_Space.size(),
                            std::get<1>(p_table[var_name]),std::get<2>(p_table[var_name]), std::get<3>(p_table[var_name]),
                                    var_name));
                    break;
                case 11:
                    Address_Space.emplace_back(std::make_shared<Variable>(type_state, float(), Address_Space.size(),
                            std::get<1>(p_table[var_name]),std::get<2>(p_table[var_name]), std::get<3>(p_table[var_name]),
                                    var_name));
                    break;
                case 12:
                    Address_Space.emplace_back(std::make_shared<Variable>(type_state, double(), Address_Space.size(),
                            std::get<1>(p_table[var_name]),std::get<2>(p_table[var_name]), std::get<3>(p_table[var_name]),
                                    var_name));
                    break;
                case 13:
                    Address_Space.emplace_back(std::make_shared<Variable>(type_state, longdouble(), Address_Space.size(),
                            std::get<1>(p_table[var_name]),std::get<2>(p_table[var_name]), std::get<3>(p_table[var_name]),
                                    var_name));
                    break;
                case 14:
                    Address_Space.emplace_back(std::make_shared<Variable>(type_state, std::string(), Address_Space.size(),
                            std::get<1>(p_table[var_name]),std::get<2>(p_table[var_name]), std::get<3>(p_table[var_name]),
                                    var_name));
                    break;
                default:
                    Address_Space.emplace_back(std::make_shared<Variable>(type_state, int(), Address_Space.size(),
                            std::get<1>(p_table[var_name]),std::get<2>(p_table[var_name]), std::get<3>(p_table[var_name]),
                                    var_name));
                    break;
            }
            function_stack.top().get_var_table()[var_name] = Address_Space.size() - 1;
        }
        int parameters_number = function_stack.top().get_arguments_number();
        std::string parameter_name;
        for (int i = parameters_number - 1; i >= 0; i--){
            parameter_name = function_stack.top().fun_parameters[i];
            int t_id = static_cast<int>(stack.top());
            stack.pop();
            if (t_id != function_stack.top().get_type_matching_table()[parameter_name]){
                throw "Invalid type of function parameter";
            }
            switch (t_id){
                case 1:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]]->cast_variable().get_pointer<char>()) = static_cast<char>(stack.top());
                    break;
                case 2:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]]->cast_variable().get_pointer<u_char>()) = static_cast<u_char>(stack.top());
                    break;
                case 3:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]]->cast_variable().get_pointer<short>()) = static_cast<short>(stack.top());
                    break;
                case 4:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]]->cast_variable().get_pointer<u_short>()) = static_cast<u_short>(stack.top());
                    break;
                case 5:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]]->cast_variable().get_pointer<int>()) = static_cast<int>(stack.top());
                    break;
                case 6:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]]->cast_variable().get_pointer<u_int>()) = static_cast<u_int>(stack.top());
                    break;
                case 7:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]]->cast_variable().get_pointer<long>()) = static_cast<long>(stack.top());
                    break;
                case 8:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]]->cast_variable().get_pointer<u_long>()) = static_cast<u_long>(stack.top());
                    break;
                case 9:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]]->cast_variable().get_pointer<longlong>()) = static_cast<longlong>(stack.top());
                    break;
                case 10:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]]->cast_variable().get_pointer<u_longlong>()) = static_cast<u_longlong>(stack.top());
                    break;
                case 11:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]]->cast_variable().get_pointer<float>()) = static_cast<float>(stack.top());
                    break;
                case 12:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]]->cast_variable().get_pointer<double>()) = static_cast<double>(stack.top());
                    break;
                case 13:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]]->cast_variable().get_pointer<longdouble>()) = static_cast<longdouble>(stack.top());
                    break;
                case 14:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]]->cast_variable().get_pointer<std::string>()) = static_cast<std::string>(stack.top());
                    break;
                default:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]]->cast_variable().get_pointer<int>()) = static_cast<int>(stack.top());
                    break;
            }
            stack.pop();
        }
        RefactorMemory(Address_Space, function_stack.top().get_address_space_point());
        return function_stack.top().get_enter_point();
    }
};

class Return: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        int a = function_stack.top().get_exit_point();
        int b = Address_Space.size() - 1;
        int c = function_stack.top().get_address_space_point();
        for (int i = b; i >= c; i--){
            Address_Space.pop_back();
        }
        //Почистить адресное пространство -->
        function_stack.pop(); //Поднять функцию из стека-->
        return a; //Вернуть exit point -->
    }
};

class Scanf: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        int t_id = static_cast<int>(stack.top());
        stack.pop();
        if (t_id != 14) {
            throw "Invalid type of function parameter";
        }
        const char* type_scan = static_cast<const char*>(stack.top());
        stack.pop();
        if (0==strcmp(type_scan, "char")) {
            stack.push(Object(Read<char>()));
        }
        else if (0==strcmp(type_scan, "unsigned char")) {
            stack.push(Object(Read<u_char>()));
        }
        else if (0==strcmp(type_scan, "unsigned char")) {
            stack.push(Object(Read<u_char>()));
        }
        else if (0==strcmp(type_scan, "short")) {
            stack.push(Object(Read<short>()));
        }
        else if (0==strcmp(type_scan, "unsigned short")) {
            stack.push(Object(Read<u_short>()));
        }
        else if (0==strcmp(type_scan, "int")) {
            stack.push(Object(Read<int>()));
        }
        else if (0==strcmp(type_scan, "unsigned int")) {
            stack.push(Object(Read<u_int>()));
        }
        else if (0==strcmp(type_scan, "long")) {
            stack.push(Object(Read<long>()));
        }
        else if (0==strcmp(type_scan, "unsigned long")) {
            stack.push(Object(Read<u_long>()));
        }
        else if (0==strcmp(type_scan, "long long")){
            stack.push(Object(Read<longlong>()));
        }
        else if (0==strcmp(type_scan, "unsigned long long")) {
            stack.push(Object(Read<u_longlong>()));
        }
        else if (0==strcmp(type_scan, "float")) {
            stack.push(Object(Read<float>()));
        }
        else if (0==strcmp(type_scan, "double")) {
            stack.push(Object(Read<double>()));
        }
        else if (0==strcmp(type_scan, "long double")) {
            stack.push(Object(Read<longdouble>()));
        }
        else{
            throw "Invalid argument of scanf";
        }
        return EIP + 1;
    }
};

class Printf: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        int t_id = static_cast<int>(stack.top());
        stack.pop();
        switch (t_id){
            case 1:
            Write(static_cast<char>(stack.top()));
            break;
            case 2:
                Write(static_cast<u_char>(stack.top()));
                break;
            case 3:
                Write(static_cast<short>(stack.top()));
                break;
            case 4:
                Write(static_cast<u_short>(stack.top()));
                break;
            case 5:
                Write(static_cast<int>(stack.top()));
                break;
            case 6:
                Write(static_cast<u_int>(stack.top()));
                break;
            case 7:
                Write(static_cast<long>(stack.top()));
                break;
            case 8:
                Write(static_cast<u_long>(stack.top()));
                break;
            case 9:
                Write(static_cast<longlong>(stack.top()));
                break;
            case 10:
                Write(static_cast<u_longlong>(stack.top()));
                break;
            case 11:
                Write(static_cast<float>(stack.top()));
                break;
            case 12:
                Write(static_cast<double>(stack.top()));
                break;
            case 13:
                Write(static_cast<longdouble>(stack.top()));
                break;
            case 14:
                Write(static_cast<const char*>(stack.top()));
                break;
            default:
                throw "Invalid printf operand";
        }
        return EIP + 1;
    }
};

class Malloc: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        int t_id = static_cast<int>(stack.top());
        stack.pop();
        if (t_id != 5){
            throw "Invalid malloc operand";
        }
        int value_2 = static_cast<int>(stack.top());
        stack.pop();
        if (value_2 < 1){
            throw "Invalid malloc operand";
        }
        t_id = static_cast<int>(stack.top());
        stack.pop();
        if (t_id < 15){
            throw "Invalid malloc operand";
        }
        int value_1 = static_cast<int>(stack.top());
        if (value_1 > Address_Space.size() - 1){
            ///////////////////////////////////////////////////////////////////////
            throw "Trying to access nonexistent address";
            ///////////////////////////////////////////////////////////////////////
        }
        stack.pop();
        Address_Space[value_1]->get_vars_in_subordination().push(value_2);
        Address_Space[value_1]->get_type_pointing().push(t);
        RefactorMemory(Address_Space, value_1);
        return EIP + 1;
    }
    int t;
public:
    explicit Malloc(int point_to){
        t = point_to;
    }
};


#endif //INTERPRET_COMMANDS_H
