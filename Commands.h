//#ifndef INTERPRET_COMMANDS_H
//#define INTERPRET_COMMANDS_H

#include "Variables.h"


typedef std::vector <Variable> HDD;
typedef std::stack <Function> RAM;
typedef std::map <std::string, Function> Table;
typedef std::map <std::string, std::tuple <int, std::queue <int>, bool>> Par_Table;

void RefactorMemory(HDD& Address_Space, int refactoring_point){ //TODO refactoring

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
        //std::cout << "Push" << std::endl;
    }
    explicit Push(int point){
        //std::cout << "Push" << std::endl;
        value = Object(point);
    }
    explicit Push(Lexeme& lexeme){
        //std::cout << "Push" << std::endl;
        lex = lexeme;
    }
    int Execute ( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override{
        if (lex.type != LexemIdentifier){
            stack.push(value);
        }
        else {
            stack.push(Address_Space[function_stack.top().get_var_table()[lex.value]].cast_variable());
        }
        return EIP+1;
    }
};

class Pop: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        stack.pop();
        return EIP+1;
    }

public:
    Pop(){
        //std::cout << "Pop" << std::endl;
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
public:
    Add(){
        //std::cout << "Add" << std::endl;
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
    Sub(){
        //std::cout << "Sub" << std::endl;
    }
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
    Mul(){
        //std::cout << "Mul" << std::endl;
    }
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
public:
    Div(){
        //std::cout << "Div" << std::endl;
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
public:
    Mod(){
        //std::cout << "Mod" << std::endl;
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
public:
    MoveLeft(){
        //std::cout << "MoveLeft" << std::endl;
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
public:
    MoveRight(){
        //std::cout << "MoveRight" << std::endl;
    }
};

template <typename T1, typename T2>
class Less: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(static_cast<bool>(value_1) < static_cast<bool>(value_2) ? 1 : 0));
        return EIP+1;
    }
public:
    Less(){
        //std::cout << "Less" << std::endl;
    }
};

template <typename T1, typename T2>
class LessEqual: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(static_cast<bool>(value_1) <= static_cast<bool>(value_2) ? 1 : 0));
        return EIP+1;
    }
public:
    LessEqual(){
        //std::cout << "LessEqual" << std::endl;
    }
};

template <typename T1, typename T2>
class Greater: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(static_cast<bool>(value_1) > static_cast<bool>(value_2) ? 1 : 0));
        return EIP+1;
    }
public:
    Greater(){
        //std::cout << "Greater" << std::endl;
    }
};

template <typename T1, typename T2>
class GreaterEqual: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(static_cast<bool>(value_1) >= static_cast<bool>(value_2) ? 1 : 0));
        return EIP+1;
    }
public:
    GreaterEqual(){
        //std::cout << "GreaterEqual" << std::endl;
    }
};

template <typename T1, typename T2>
class Equal: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(static_cast<bool>(value_1) == static_cast<bool>(value_2) ? 1 : 0));
        return EIP+1;
    }
public:
    Equal(){
        //std::cout << "Equal" << std::endl;
    }
};

template <typename T1, typename T2>
class NotEqual: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(static_cast<bool>(value_1) != static_cast<bool>(value_2) ? 1 : 0));
        return EIP+1;
    }
public:
    NotEqual(){
        //std::cout << "NotEqual" << std::endl;
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
public:
    ByteAnd(){
        //std::cout << "ByteAnd" << std::endl;
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
public:
    ByteXor(){
        //std::cout << "ByteXor" << std::endl;
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
public:
    ByteOr(){
        //std::cout << "ByteOr" << std::endl;
    }
};

template <typename T1, typename T2>
class LogicalAnd: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(static_cast<bool>(value_1) && static_cast<bool>(value_2) ? 1 : 0));
        return EIP+1;
    }
public:
    LogicalAnd(){
        //std::cout << "LogicalAnd" << std::endl;
    }
};

template <typename T1, typename T2>
class LogicalOr: public Commander{
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        T2 value_2 = static_cast<T2>(stack.top());
        stack.pop();
        T1 value_1 = static_cast<T1>(stack.top());
        stack.pop();
        stack.push(Object(static_cast<bool>(value_1) || static_cast<bool>(value_2) ? 1 : 0));
        return EIP+1;
    }
public:
    LogicalOr(){
        //std::cout << "LogicalOr" << std::endl;
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
        stack.push(Object(static_cast<bool>(value_1) ? value_2 : value_3));
        return EIP+1;
    }
public:
    Conditional(){
        //std::cout << "Conditional" << std::endl;
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
        stack.push(Object(*value_1 = value_2));
        return EIP+1;
    }
public:
    Equally(){
        //std::cout << "Equally" << std::endl;
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
public:
    AddEqual(){
        //std::cout << "AddEqual" << std::endl;
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
public:
    SubEqual(){
        //std::cout << "SubEqual" << std::endl;
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
public:
    MulEqual(){
        //std::cout << "MulEqual" << std::endl;
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
public:
    DivEqual(){
        //std::cout << "DivEqual" << std::endl;
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
public:
    ModEqual(){
        //std::cout << "ModEqual" << std::endl;
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
public:
    MoveLeftEqual(){
        //std::cout << "MoveLeftEqual" << std::endl;
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
public:
    MoveRightEqual(){
        //std::cout << "MoveRightEqual" << std::endl;
    }
};

template <typename T1, typename T2>
class LogicalAndEqual: public Commander{
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
        stack.push(Object(*value_1 &= value_2));
        return EIP+1;
    }
public:
    LogicalAndEqual(){
        //std::cout << "LogicalAndEqual" << std::endl;
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
public:
    LogicalXorEqual(){
        //std::cout << "LogicalXorEqual" << std::endl;
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
public:
    LogicalOrEqual(){
        //std::cout << "LogicalOrEqual" << std::endl;
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
public:
    Comma(){
        //std::cout << "Comma" << std::endl;
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
public:
    UnaryPlus(){
        //std::cout << "UnaryPlus" << std::endl;
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
public:
    UnaryMinus(){
        //std::cout << "UnaryMinus" << std::endl;
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
public:
    PrefixIncrement(){
        //std::cout << "PrefixIncrement" << std::endl;
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
public:
    PrefixDecrement(){
        //std::cout << "PrefixDecrement" << std::endl;
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
public:
    PostfixIncrement(){
        //std::cout << "PostfixIncrement" << std::endl;
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
public:
    PostfixDecrement(){
        //std::cout << "PostfixDecrement" << std::endl;
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
public:
    ByteNot(){
        //std::cout << "ByteNot" << std::endl;
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
public:
    Not(){
        //std::cout << "Not" << std::endl;
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
public:
    PointerTo(){
        //std::cout << "PointerTo" << std::endl;
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
        stack.push(Object(Address_Space[value_1].cast_variable()));
        return EIP+1;
    }
public:
    RemovePointer(){
        //std::cout << "RemovePointer" << std::endl;
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
public:
    SizeofExpression(){
        //std::cout << "SizeofExpression" << std::endl;
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
public:
    TypeCast(){
        //std::cout << "TypeCast" << std::endl;
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
        stack.push(Address_Space[value_1 + value_2].cast_variable());
        return EIP+1;
    }
public:
    Addressing(){
        //std::cout << "Addressing" << std::endl;
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
public:
    If(){
        //std::cout << "If" << std::endl;
    }
};

class Goto: public Commander{
    int value_1;
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        return value_1;
    }
public:
    Goto(int& k){
        value_1 = k;
        //std::cout << "Goto" << std::endl;
    }
};

class Call: public Commander{
public:
    explicit Call(std::string& s){
        //std::cout << "Call" << std::endl;
        fun_name = s;
    }
private:
    std::string fun_name;
    int Execute( std::stack <Object>& stack, int EIP, HDD& Address_Space, RAM& function_stack, Table& table ) const override {
        function_stack.push(table[fun_name]);
        function_stack.top().get_address_space_point() = Address_Space.size();
        function_stack.top().get_exit_point() = EIP + 1;
        std::string var_name;
        Par_Table p_table = function_stack.top().get_var_parameters_table();
        for (auto iter = function_stack.top().get_var_table().begin(); iter != function_stack.top().get_var_table().end();
        iter++){
            int type_state = std::get<0>(p_table[var_name]);
            var_name = iter->first;
            switch (type_state){
                case 1:
                    Address_Space.emplace_back(type_state, char(), Address_Space.size(), std::get<1>(p_table[var_name]),
                            std::get<2>(p_table[var_name]), var_name);
                    break;
                case 2:
                    Address_Space.emplace_back(type_state, u_char(), Address_Space.size(), std::get<1>(p_table[var_name]),
                            std::get<2>(p_table[var_name]), var_name);
                    break;
                case 3:
                    Address_Space.emplace_back(type_state, short(), Address_Space.size(), std::get<1>(p_table[var_name]),
                            std::get<2>(p_table[var_name]), var_name);
                    break;
                case 4:
                    Address_Space.emplace_back(type_state, u_short(), Address_Space.size(), std::get<1>(p_table[var_name]),
                            std::get<2>(p_table[var_name]), var_name);
                    break;
                case 5:
                    Address_Space.emplace_back(type_state, int(), Address_Space.size(), std::get<1>(p_table[var_name]),
                            std::get<2>(p_table[var_name]), var_name);
                    break;
                case 6:
                    Address_Space.emplace_back(type_state, u_int(), Address_Space.size(), std::get<1>(p_table[var_name]),
                            std::get<2>(p_table[var_name]), var_name);
                    break;
                case 7:
                    Address_Space.emplace_back(type_state, long(), Address_Space.size(), std::get<1>(p_table[var_name]),
                            std::get<2>(p_table[var_name]), var_name);
                    break;
                case 8:
                    Address_Space.emplace_back(type_state, u_long(), Address_Space.size(), std::get<1>(p_table[var_name]),
                            std::get<2>(p_table[var_name]), var_name);
                    break;
                case 9:
                    Address_Space.emplace_back(type_state, longlong(), Address_Space.size(), std::get<1>(p_table[var_name]),
                            std::get<2>(p_table[var_name]), var_name);
                    break;
                case 10:
                    Address_Space.emplace_back(type_state, u_longlong(), Address_Space.size(), std::get<1>(p_table[var_name]),
                            std::get<2>(p_table[var_name]), var_name);
                    break;
                case 11:
                    Address_Space.emplace_back(type_state, float(), Address_Space.size(), std::get<1>(p_table[var_name]),
                            std::get<2>(p_table[var_name]), var_name);
                    break;
                case 12:
                    Address_Space.emplace_back(type_state, double(), Address_Space.size(), std::get<1>(p_table[var_name]),
                            std::get<2>(p_table[var_name]), var_name);
                    break;
                case 13:
                    Address_Space.emplace_back(type_state, longdouble(), Address_Space.size(), std::get<1>(p_table[var_name]),
                            std::get<2>(p_table[var_name]), var_name);
                    break;
                case 0:
                    throw std::logic_error("Logic error: cat't create void variable");
                default:
                    Address_Space.emplace_back(type_state, int(), Address_Space.size(), std::get<1>(p_table[var_name]),
                            std::get<2>(p_table[var_name]), var_name);
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
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]].cast_variable().get_pointer<char>()) = static_cast<char>(stack.top());
                    break;
                case 2:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]].cast_variable().get_pointer<u_char>()) = static_cast<u_char>(stack.top());
                    break;
                case 3:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]].cast_variable().get_pointer<short>()) = static_cast<short>(stack.top());
                    break;
                case 4:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]].cast_variable().get_pointer<u_short>()) = static_cast<u_short>(stack.top());
                    break;
                case 5:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]].cast_variable().get_pointer<int>()) = static_cast<int>(stack.top());
                    break;
                case 6:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]].cast_variable().get_pointer<u_int>()) = static_cast<u_int>(stack.top());
                    break;
                case 7:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]].cast_variable().get_pointer<long>()) = static_cast<long>(stack.top());
                    break;
                case 8:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]].cast_variable().get_pointer<u_long>()) = static_cast<u_long>(stack.top());
                    break;
                case 9:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]].cast_variable().get_pointer<longlong>()) = static_cast<longlong>(stack.top());
                    break;
                case 10:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]].cast_variable().get_pointer<u_longlong>()) = static_cast<u_longlong>(stack.top());
                    break;
                case 11:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]].cast_variable().get_pointer<float>()) = static_cast<float>(stack.top());
                    break;
                case 12:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]].cast_variable().get_pointer<double>()) = static_cast<double>(stack.top());
                    break;
                case 13:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]].cast_variable().get_pointer<longdouble>()) = static_cast<longdouble>(stack.top());
                    break;
                default:
                    *(Address_Space[function_stack.top().get_var_table()[parameter_name]].cast_variable().get_pointer<int>()) = static_cast<int>(stack.top());
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
public:
    Return(){
        //std::cout << "Return" << std::endl;
    }
};


//#endif //INTERPRET_COMMANDS_H
