//#ifndef INTERPRET_DATA_H
//#define INTERPRET_DATA_H

#include "Functions.h"

enum LexemeClass{
    C_LexemConstant,
    C_LexemKeyword,
    C_LexemIdentifier,
    C_LexemSeparator
};

enum LexemeType{
    //Константы
    LexemDecIntConst,
    LexemOctIntConst,
    LexemHexIntConst,
    LexemStringConst,
    LexemDoubleConst,
    LexemCharConst,
    //Идентификатор
    LexemIdentifier,
    //Ключевые слова
    LexemAuto,
    LexemBreak,
    LexemCase,
    LexemChar,
    LexemConst,
    LexemContinue,
    LexemDefault,
    LexemDo,
    LexemDouble,
    LexemElse,
    LexemEnum,
    LexemExtern,
    LexemFloat,
    LexemFor,
    LexemGoto,
    LexemIf,
    LexemInt,
    LexemLong,
    LexemRegister,
    LexemReturn,
    LexemShort,
    LexemSigned,
    LexemSizeof,
    LexemStatic,
    LexemStruct,
    LexemSwitch,
    LexemTypedef,
    LexemUnion,
    LexemUnsigned,
    LexemVoid,
    LexemVolatile,
    LexemWhile,
    //Разделители
            LexemSemicolon,
    LexemColon,
    LexemDot,
    LexemComma,
    LexemQuestionMark,
    LexemLeftParenthesis,
    LexemRightParenthesis,
    LexemLeftBrace,
    LexemRightBrace,
    LexemLess,
    LexemGreater,
    LexemLeftBracket,
    LexemRightBracket,
    LexemExclamationMark,
    LexemSlash,
    LexemSlashEqual,
    LexemStraightSlash,
    LexemBackslash,
    LexemTilde,
    LexemLattice,
    LexemPercent,
    LexemAmpersand,
    LexemSuffix,
    LexemEqually,
    LexemEqual,
    LexemNotEqual,
    LexemPlus,
    LexemMinus,
    LexemAsterisk,
    LexemDoubleAmpersand,
    LexemAmpersandEqual,
    LexemDoubleStraightSlash,
    LexemStraightSlashEqual,
    LexemPercentEqual,
    LexemSuffixEqual,
    LexemAsteriskEqual,
    LexemLessEqual,
    LexemMoveLeft,
    LexemMoveLeftEqual,
    LexemGreaterEqual,
    LexemMoveRight,
    LexemMoveRightEqual,
    LexemPlusEqual,
    LexemPlusPlus,
    LexemMinusEqual,
    LexemMinusMinus,
    LexemPoint,
    LexemComment,
    LexemPlusPlus_Post,
    LexemMinusMinus_Post,
};

enum C_Object{
    constant,
    variable,
};

const std::map<std::string, enum LexemeType> key_words =
        {{"auto",LexemAuto},{"break",LexemBreak},{"case",LexemCase},{"char",LexemChar},{"const",LexemConst},
         {"continue",LexemContinue},{"default",LexemDefault},{"do",LexemDo},{"double",LexemDouble},{"else",LexemElse},
         {"enum",LexemEnum},{"extern",LexemExtern},{"float",LexemFloat},{"for",LexemFor},{"goto",LexemGoto},
         {"if",LexemIf},{"int",LexemInt},{"long",LexemLong},{"register",LexemRegister},{"return",LexemReturn},
         {"short",LexemShort},{"signed",LexemSigned},{"sizeof",LexemSizeof},{"static",LexemStatic},
         {"struct",LexemStruct},{"switch",LexemSwitch},{"typedef",LexemTypedef},{"union",LexemUnion},
         {"unsigned",LexemUnsigned},{"void",LexemVoid},{"volatile",LexemVolatile},{"while",LexemWhile}};

struct Lexeme{
    LexemeClass l_class;
    LexemeType type;
    std::string value;
    int line_number;

    int GetDecInt(){
        return stoi(value, nullptr, 10);
    }
    int GetOctInt(){
        return stoi(value, nullptr, 8);
    }
    int GetHexInt(){
        return stoi(value, nullptr, 16);
    }
    const char* GetString(){
        return value.c_str();
    }
    char GetChar(){
        return value[0];
    }
    double GetDouble(){
        return stod(value);
    }
};

typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;
typedef long long longlong;
typedef unsigned long long u_longlong;
typedef long double longdouble;

class Variable;
class Type;
class Function;

//std::vector <std::pair<Variable,bool>> Address_Space;
//std::stack <Function> function_stack;

class Data{
public:
    virtual ~Data() = default;
    virtual Data* copy_data() = 0;
    virtual Data* copy_data_pointer() = 0;
};

template <typename T>
class D_Storage: public Data{
private:
    T info;
public:
    explicit D_Storage(const T& info_cast): info(info_cast){}
    D_Storage() = default;
    Data* copy_data () override{
        return new D_Storage<T>(info);
    }
    Data* copy_data_pointer() override;
    T get_info(){
        return info;
    }

};

template <typename T>
class D_Pointer: public Data{
private:
    T info;
public:
    explicit D_Pointer(const T& info_cast): info(info_cast){}
    D_Pointer() = default;
    Data* copy_data () override{
        return new D_Pointer<T>(info);
    }
    Data* copy_data_pointer() override {
        throw std::logic_error("Logic error at D_Pointer::copy_data_pointer");
    }
    T get_info(){
        return info;
    }

};

template <typename T>
Data* D_Storage<T>::copy_data_pointer() {
    return new D_Pointer<T*>(&info);
}

//#endif //INTERPRET_DATA_H