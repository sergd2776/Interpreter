#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <stack>

#include "Types.h"
//using namespace std;


class Scanner{
public:
    explicit Scanner( const char* sourceFileName ){
        file_name = sourceFileName;
        fin.open( sourceFileName );
        Line = 1;
        OpenApostrophe = 0;
        OpenQuotationMark = 0;
        OpenComment = 0;
    }
    ~Scanner(){
        fin.close();
    }
    void Run( std::vector <Lexeme> &program_data ){
        char c = ' ';
        bool b = true;
        try {
            while ( b ) {
                std::pair <char,int> error;
                b = TakeLexeme(program_data, c);
                if (program_data.back().type == LexemIdentifier) {
                    auto i = key_words.find(program_data.back().value);
                    if (i != key_words.end()) {
                        program_data.back().l_class = C_LexemKeyword;
                        program_data.back().type = i->second;
                    }
                }
                if (program_data.back().type == LexemComment) {
                    program_data.pop_back();
                }
            }
        }
        catch(const std::pair <char,int> &k){
            if (OpenApostrophe != 0){
                std::cout << file_name << ": " << OpenApostrophe << ": lexical error: missing terminating ' character"
                << std::endl;
            }
            else if (OpenQuotationMark != 0){
                std::cout << file_name << ": " << OpenQuotationMark << ": lexical error: missing terminating \" character"
                << std::endl;
            }
            else if (OpenComment != 0){
                std::cout << file_name << ": " << OpenComment << ": lexical error: unclosed comment" << std::endl;
            }
            else {
                std::cout << file_name << ": " << k.second << ": lexical error: illegal character '" << k.first << '\''
                << std::endl;
            }
            exit(1);
        }
    }
private:
    int OpenComment, OpenQuotationMark, OpenApostrophe;
    int Line;
    Lexeme buff;
    std::ifstream fin;
    std::string file_name;
    typedef bool ( Scanner::*State )( char c );
    State state = nullptr;
    bool stateInitial( char c );
    bool stateInt( char c );
    bool stateDouble_1( char c );
    bool stateOctorHex( char c );
    bool stateDouble_2( char c );
    bool stateOct( char c );
    bool stateHex_0_1( char c );
    bool stateHex( char c );
    bool stateDouble_0_1( char c );
    bool stateDouble_0_2( char c );
    bool stateDouble_0_3( char c );
    bool stateDouble_3( char c );
    bool stateIdentifier_1( char c );
    bool stateBackslash( char c );
    bool stateLattice( char c );
    bool stateQuestionMark( char c );
    bool stateDot( char c );
    bool stateComma( char c );
    bool stateSemicolon( char c );
    bool stateColon( char c );
    bool stateLeftParenthesis( char c );
    bool stateRightParenthesis( char c );
    bool stateLeftBracket( char c );
    bool stateRightBracket( char c );
    bool stateTilde( char c );
    bool stateLeftBrace( char c );
    bool stateRightBrace( char c );
    bool stateStraightSlash( char c );
    bool stateStraightSlashEqual( char c );
    bool stateDoubleStraightSlash( char c );
    bool stateExclamationMark( char c );
    bool stateNotEqual( char c );
    bool stateSlash( char c );
    bool stateSlashEqual( char c );
    bool stateSingleComment_1( char c );
    bool stateSingleComment_2( char c );
    bool statePluralComment_0_1( char c );
    bool statePluralComment_0_2( char c );
    bool statePluralComment( char c );
    bool statePercent( char c );
    bool statePercentEqual( char c );
    bool stateSuffix( char c );
    bool stateSuffixEqual( char c );
    bool statePlus( char c );
    bool statePlusEqual( char c );
    bool statePlusPlus( char c );
    bool stateMinus( char c );
    bool stateMinusEqual( char c );
    bool stateMinusMinus( char c );
    bool statePoint( char c );
    bool stateAsterisk( char c );
    bool stateAsteriskEqual( char c );
    bool stateLess( char c );
    bool stateLessEqual( char c );
    bool stateMoveLeft( char c );
    bool stateMoveLeftEqual( char c );
    bool stateGreater( char c );
    bool stateGreaterEqual( char c );
    bool stateMoveRight( char c );
    bool stateMoveRightEqual( char c );
    bool stateEqually( char c );
    bool stateEqual( char c );
    bool stateAmpersand( char c );
    bool stateDoubleAmpersand( char c );
    bool stateAmpersandEqual( char c );
    bool stateString_0_1( char c );
    bool stateString_0_2( char c );
    bool stateString( char c );
    bool stateChar_0_1( char c );
    bool stateChar_0_2( char c );
    bool stateChar_0_3( char c );
    bool stateChar_1( char c );

    bool TakeLexeme( std::vector <Lexeme> &program_data, char& c );
};

bool Scanner::stateInitial( char c ){
    if ((c == ' ') || (c == '\t') || (c == '\v') || (c == '\r')){
        return true;
    }
    if (c == '\n'){
        Line++;
        return true;
    }
    if ((c >= '1') && (c <= '9')){
        state = &Scanner::stateInt;
        buff.l_class = C_LexemConstant;
        buff.type = LexemDecIntConst;
        buff.value += c;
        return true;
    }
    if (c == '0'){
        state = &Scanner::stateOctorHex;
        buff.l_class = C_LexemConstant;
        buff.type = LexemDecIntConst;
        buff.value += c;
        return true;
    }
    if (c == '~'){
        state = &Scanner::stateTilde;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemTilde;
        buff.value += c;
        return true;
    }
    if (c == '}'){
        state = &Scanner::stateRightBrace;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemRightBrace;
        buff.value += c;
        return true;
    }
    if (c == '{'){
        state = &Scanner::stateLeftBrace;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemLeftBrace;
        buff.value += c;
        return true;
    }
    if (c == ']'){
        state = &Scanner::stateRightBracket;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemRightBracket;
        buff.value += c;
        return true;
    }
    if (c == '['){
        state = &Scanner::stateLeftBracket;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemLeftBracket;
        buff.value += c;
        return true;
    }
    if (c == ')'){
        state = &Scanner::stateRightParenthesis;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemRightParenthesis;
        buff.value += c;
        return true;
    }
    if (c == '('){
        state = &Scanner::stateLeftParenthesis;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemLeftParenthesis;
        buff.value += c;
        return true;
    }
    if (c == ':'){
        state = &Scanner::stateColon;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemColon;
        return true;
    }
    if (c == ';'){
        state = &Scanner::stateSemicolon;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemSemicolon;
        buff.value += c;
        return true;
    }
    if (c == ','){
        state = &Scanner::stateComma;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemComma;
        buff.value += c;
        return true;
    }
    if (c == '.'){
        state = &Scanner::stateDot;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemDot;
        buff.value += c;
        return true;
    }
    if (c == '?'){
        state = &Scanner::stateQuestionMark;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemQuestionMark;
        buff.value += c;
        return true;
    }
    if (c == '#'){
        state = &Scanner::stateLattice;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemLattice;
        buff.value += c;
        return true;
    }
    if (c == '\\'){
        state = &Scanner::stateBackslash;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemBackslash;
        buff.value += c;
        return true;
    }
    if (c == '\''){
        state = &Scanner::stateChar_0_1;
        buff.l_class = C_LexemConstant;
        buff.type = LexemCharConst;
        OpenApostrophe = Line;
        return true;
    }
    if (c == '"'){
        state = &Scanner::stateString_0_1;
        buff.l_class = C_LexemConstant;
        buff.type = LexemStringConst;
        OpenQuotationMark = Line;
        return true;
    }
    if ((c == '_') || ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))){
        state = &Scanner::stateIdentifier_1;
        buff.l_class = C_LexemIdentifier;
        buff.type = LexemIdentifier;
        buff.value += c;
        return true;
    }
    if (c == '&'){
        state = &Scanner::stateAmpersand;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemAmpersand;
        buff.value += c;
        return true;
    }
    if (c == '='){
        state = &Scanner::stateEqually;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemEqually;
        buff.value += c;
        return true;
    }
    if (c == '>'){
        state = &Scanner::stateGreater;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemGreater;
        buff.value += c;
        return true;
    }
    if (c == '<'){
        state = &Scanner::stateLess;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemLess;
        buff.value += c;
        return true;
    }
    if (c == '*'){
        state = &Scanner::stateAsterisk;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemAsterisk;
        buff.value += c;
        return true;
    }
    if (c == '-'){
        state = &Scanner::stateMinus;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemMinus;
        buff.value += c;
        return true;
    }
    if (c == '+'){
        state = &Scanner::statePlus;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemPlus;
        buff.value += c;
        return true;
    }
    if (c == '^'){
        state = &Scanner::stateSuffix;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemSuffix;
        buff.value += c;
        return true;
    }
    if (c == '%'){
        state = &Scanner::statePercent;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemPercent;
        buff.value += c;
        return true;
    }
    if (c == '/'){
        state = &Scanner::stateSlash;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemSlash;
        buff.value += c;
        return true;
    }
    if (c == '!'){
        state = &Scanner::stateExclamationMark;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemExclamationMark;
        buff.value += c;
        return true;
    }
    if (c == '|'){
        state = &Scanner::stateStraightSlash;
        buff.l_class = C_LexemSeparator;
        buff.type = LexemStraightSlash;
        buff.value += c;
        return true;
    }
    std::pair <char,int> k;
    k.first = c;
    k.second = Line;
    throw k;
    return false;
}
bool Scanner::stateInt( char c ){
    if ((c >= '0') && (c <= '9')){
        buff.value += c;
        return true;
    }
    if (c == '.'){
        state = &Scanner::stateDouble_1;
        buff.type = LexemDoubleConst;
        buff.value += c;
        return true;
    }
    if ((c == 'e') || (c == 'E')){
        state = &Scanner::stateDouble_0_2;
        buff.type = LexemDoubleConst;
        buff.value += c;
        return true;
    }
    return false;
}
bool Scanner::stateDouble_1( char c ) {
    if ((c >= '0') && (c <= '9')){
        state = &Scanner::stateDouble_2;
        buff.value += c;
        return true;
    }
    if ((c == 'e') || (c == 'E')){
        state = &Scanner::stateDouble_0_2;
        buff.value += c;
        return true;
    }
    return false;
}
bool Scanner::stateOctorHex( char c ) {
    if (c == '.'){
        state = &Scanner::stateDouble_1;
        buff.type = LexemDoubleConst;
        buff.value += c;
        return true;
    }
    if ((c >= '0') && (c <= '7')){
        state = &Scanner::stateOct;
        buff.type = LexemOctIntConst;
        buff.value += c;
        return true;
    }
    if ((c == '8') || (c == '9')){
        state = &Scanner::stateDouble_0_1;
        buff.type = LexemDoubleConst;
        buff.value += c;
        return true;
    }
    if ((c == 'x') || (c == 'X')){
        state = &Scanner::stateHex_0_1;
        buff.type = LexemHexIntConst;
        buff.value += c;
        return true;
    }
    if ((c == 'e') || (c == 'E')){
        state = &Scanner::stateDouble_0_2;
        buff.type = LexemDoubleConst;
        buff.value += c;
        return true;
    }
    return false;
}
bool Scanner::stateDouble_2( char c ) {
    if ((c == 'e') || (c == 'E')){
        state = &Scanner::stateDouble_0_2;
        buff.value += c;
        return true;
    }
    if ((c >= '0') && (c <= '9')){
        buff.value += c;
        return true;
    }
    return false;
}
bool Scanner::stateOct( char c ) {
    if ((c == 'e') || (c == 'E')){
        state = &Scanner::stateDouble_0_2;
        buff.type = LexemDoubleConst;
        buff.value += c;
        return true;
    }
    if ((c >= '0') && (c <= '7')){
        buff.value += c;
        return true;
    }
    if ((c == '8') || (c == '9')){
        state = &Scanner::stateDouble_0_1;
        buff.type = LexemDoubleConst;
        buff.value += c;
        return true;
    }
    if (c == '.'){
        state = &Scanner::stateDouble_1;
        buff.type = LexemDoubleConst;
        buff.value += c;
        return true;
    }
    return false;
}
bool Scanner::stateHex_0_1( char c ) {
    if (((c >= '0') && (c <= '9')) || ((c >= 'A') && (c <= 'F'))){
        state = &Scanner::stateHex;
        buff.value += c;
        return true;
    }
    std::pair <char,int> k;
    k.first = c;
    k.second = Line;
    throw k;
    return false;
}
bool Scanner::stateHex( char c ) {
    if (((c >= '0') && (c <= '9')) || ((c >= 'A') && (c <= 'F'))){
        buff.value += c;
        return true;
    }
    return false;
}
bool Scanner::stateDouble_0_1( char c ) {
    if ((c == 'e') || (c == 'E')){
        state = &Scanner::stateDouble_0_2;
        buff.value += c;
        return true;
    }
    if ((c >= '0') && (c <= '9')){
        buff.value += c;
        return true;
    }
    if (c == '.'){
        state = &Scanner::stateDouble_1;
        buff.value += c;
        return true;
    }
    std::pair <char,int> k;
    k.first = c;
    k.second = Line;
    throw k;
    return false;
}
bool Scanner::stateDouble_0_2( char c ) {
    if ((c == '+') || (c == '-')){
        state = &Scanner::stateDouble_0_3;
        buff.value += c;
        return true;
    }
    if ((c >= '0') && (c <= '9')){
        state = &Scanner::stateDouble_3;
        buff.value += c;
        return true;
    }
    std::pair <char,int> k;
    k.first = c;
    k.second = Line;
    throw k;
    return false;
}
bool Scanner::stateDouble_0_3( char c ) {
    if ((c >= '0') && (c <= '9')){
        state = &Scanner::stateDouble_3;
        buff.value += c;
        return true;
    }
    std::pair <char,int> k;
    k.first = c;
    k.second = Line;
    throw k;
    return false;
}
bool Scanner::stateDouble_3( char c ) {
    if ((c >= '0') && (c <= '9')){
        buff.value += c;
        return true;
    }
    return false;
}
bool Scanner::stateIdentifier_1( char c ) {
    if (((c >= '0') && (c <= '9')) || ((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')) || (c == '_')){
        buff.value += c;
        return true;
    }
    return false;
}
bool Scanner::stateBackslash( char c ) {
    return false;
}
bool Scanner::stateLattice( char c ) {
    return false;
}
bool Scanner::stateQuestionMark( char c ) {
    return false;
}
bool Scanner::stateDot( char c ) {
    return false;
}
bool Scanner::stateComma( char c ) {
    return false;
}
bool Scanner::stateSemicolon( char c ) {
    return false;
}
bool Scanner::stateColon( char c ) {
    return false;
}
bool Scanner::stateLeftParenthesis( char c ) {
    return false;
}
bool Scanner::stateRightParenthesis( char c ) {
    return false;
}
bool Scanner::stateLeftBracket( char c ) {
    return false;
}
bool Scanner::stateRightBracket( char c ) {
    return false;
}
bool Scanner::stateLeftBrace( char c ) {
    return false;
}
bool Scanner::stateRightBrace( char c ) {
    return false;
}
bool Scanner::stateTilde( char c ) {
    return false;
}
bool Scanner::stateStraightSlash( char c ) {
    if (c == '='){
        state = &Scanner::stateStraightSlashEqual;
        buff.type = LexemStraightSlashEqual;
        buff.value += c;
        return true;
    }
    if (c == '|'){
        state = &Scanner::stateDoubleStraightSlash;
        buff.type = LexemDoubleStraightSlash;
        buff.value += c;
        return true;
    }
    return false;
}
bool Scanner::stateStraightSlashEqual( char c ) {
    return false;
}
bool Scanner::stateDoubleStraightSlash( char c ) {
    return false;
}
bool Scanner::stateExclamationMark( char c ) {
    if (c == '='){
        state = &Scanner::stateNotEqual;
        buff.type = LexemNotEqual;
        buff.value += c;
        return true;
    }
    return false;
}
bool Scanner::stateNotEqual( char c ) {
    return false;
}
bool Scanner::stateSlash( char c ) {
    if (c == '='){
        state = &Scanner::stateSlashEqual;
        buff.type = LexemSlashEqual;
        buff.value += c;
        return true;
    }
    if (c == '/'){
        state = &Scanner::stateSingleComment_1;
        buff.type = LexemComment;
        return true;
    }
    if (c == '*'){
        state = &Scanner::statePluralComment_0_1;
        buff.type = LexemComment;
        OpenComment = Line;
        return true;
    }
    return false;
}
bool Scanner::stateSlashEqual( char c ) {
    return false;
}
bool Scanner::stateSingleComment_1( char c ) {
    if (c == '\n'){
        state = &Scanner::stateSingleComment_2;
        Line++;
    }
    return true;
}
bool Scanner::stateSingleComment_2( char c ) {
    return false;
}
bool Scanner::statePluralComment_0_1( char c ) {
    if (c == '*'){
        state = &Scanner::statePluralComment_0_2;
    }
    if (c == '\n'){
        Line++;
    }
    return true;
}
bool Scanner::statePluralComment_0_2( char c ) {
    if (c == '\n'){
        Line++;
        state = &Scanner::statePluralComment_0_1;
        return true;
    }
    if (c == '/'){
        state = &Scanner::statePluralComment;
        OpenComment = 0;
        return true;
    }
    if (c != '*'){
        state = &Scanner::statePluralComment_0_1;
    }
    return true;
}
bool Scanner::statePluralComment( char c ) {
    return false;
}
bool Scanner::statePercent( char c ) {
    if (c == '='){
        state = &Scanner::statePercentEqual;
        buff.type = LexemPercentEqual;
        buff.value += c;
        return true;
    }
    return false;
}
bool Scanner::statePercentEqual( char c ) {
    return false;
}
bool Scanner::stateSuffix( char c ) {
    if (c == '='){
        state = &Scanner::stateSuffixEqual;
        buff.type = LexemSuffixEqual;
        buff.value += c;
        return true;
    }
    return false;
}
bool Scanner::stateSuffixEqual( char c ) {
    return false;
}
bool Scanner::statePlus( char c ) {
    if (c == '+'){
        state = &Scanner::statePlusPlus;
        buff.type = LexemPlusPlus;
        buff.value += c;
        return true;
    }
    if (c == '='){
        state = &Scanner::statePlusEqual;
        buff.type = LexemPlusEqual;
        buff.value += c;
        return true;
    }
    return false;
}
bool Scanner::statePlusPlus( char c ) {
    return false;
}
bool Scanner::statePlusEqual( char c ) {
    return false;
}
bool Scanner::stateMinus( char c ) {
    if (c == '-'){
        state = &Scanner::stateMinusMinus;
        buff.type = LexemMinusMinus;
        buff.value += c;
        return true;
    }
    if (c == '='){
        state = &Scanner::stateMinusEqual;
        buff.type = LexemMinusEqual;
        buff.value += c;
        return true;
    }
    if (c == '>'){
        state = &Scanner::statePoint;
        buff.type = LexemPoint;
        buff.value += c;
        return true;
    }
    return false;
}
bool Scanner::stateMinusMinus( char c ) {
    return false;
}
bool Scanner::stateMinusEqual( char c ) {
    return false;
}
bool Scanner::statePoint( char c ) {
    return false;
}
bool Scanner::stateAsterisk( char c ) {
    if (c == '='){
        state = &Scanner::stateAsteriskEqual;
        buff.type = LexemAsteriskEqual;
        buff. value += c;
        return true;
    }
    return false;
}
bool Scanner::stateAsteriskEqual( char c ) {
    return false;
}
bool Scanner::stateLess( char c ) {
    if (c == '='){
        state = &Scanner::stateLessEqual;
        buff.type = LexemLessEqual;
        buff.value += c;
        return true;
    }
    if (c == '<'){
        state = &Scanner::stateMoveLeft;
        buff.type = LexemMoveLeft;
        return true;
    }
    return false;
}
bool Scanner::stateLessEqual( char c ) {
    return false;
}
bool Scanner::stateMoveLeft( char c ) {
    if (c == '='){
        state = &Scanner::stateMoveLeftEqual;
        buff.type = LexemMoveLeftEqual;
        buff.value += c;
        return true;
    }
    return false;
}
bool Scanner::stateMoveLeftEqual( char c ) {
    return false;
}
bool Scanner::stateGreater( char c ) {
    if (c == '='){
        state = &Scanner::stateGreaterEqual;
        buff.type = LexemGreaterEqual;
        buff.value += c;
        return true;
    }
    if (c == '>'){
        state = &Scanner::stateMoveRight;
        buff.type = LexemMoveRight;
        buff.value += c;
        return true;
    }
    return false;
}
bool Scanner::stateGreaterEqual( char c ) {
    return false;
}
bool Scanner::stateMoveRight( char c ) {
    if (c == '='){
        state = &Scanner::stateMoveRightEqual;
        buff.type = LexemMoveRightEqual;
        buff.value += c;
        return true;
    }
    return false;
}
bool Scanner::stateMoveRightEqual( char c ) {
    return false;
}
bool Scanner::stateEqually( char c ) {
    if (c == '='){
        state = &Scanner::stateEqual;
        buff.type = LexemEqual;
        buff.value += c;
        return true;
    }
    return false;
}
bool Scanner::stateEqual( char c ) {
    return false;
}
bool Scanner::stateAmpersand( char c ) {
    if (c == '&'){
        state = &Scanner::stateDoubleAmpersand;
        buff.type = LexemDoubleAmpersand;
        buff.value += c;
        return true;
    }
    if (c == '='){
        state = &Scanner::stateAmpersandEqual;
        buff.type = LexemAmpersandEqual;
        buff.value += c;
        return true;
    }
    return false;
}
bool Scanner::stateDoubleAmpersand( char c ) {
    return false;
}
bool Scanner::stateAmpersandEqual( char c ) {
    return false;
}
bool Scanner::stateString_0_1( char c ) {
    if (c == '"'){
        state = &Scanner::stateString;
        OpenQuotationMark = 0;
        return true;
    }
    if (c == '\\'){
        state = &Scanner::stateString_0_2;
        return true;
    }
    if (c != '\n'){
        buff.value += c;
        return true;
    }
    std::pair <char,int> k;
    k.first = c;
    k.second = Line;
    throw k;
    return false;
}
bool Scanner::stateString_0_2( char c ) {
    if (c == 'b'){
        state = &Scanner::stateString_0_1;
        buff.value += '\b';
        return true;
    }
    if (c == 'f'){
        state = &Scanner::stateString_0_1;
        buff.value += '\f';
        return true;
    }
    if (c == 'n'){
        state = &Scanner::stateString_0_1;
        buff.value += '\n';
        return true;
    }
    if (c == 'r'){
        state = &Scanner::stateString_0_1;
        buff.value += '\r';
        return true;
    }
    if (c == 't'){
        state = &Scanner::stateString_0_1;
        buff.value += '\t';
        return true;
    }
    if (c == '"'){
        state = &Scanner::stateString_0_1;
        buff.value += '\"';
        return true;
    }
    if (c == '\''){
        state = &Scanner::stateString_0_1;
        buff.value += '\'';
        return true;
    }
    if (c == '\\'){
        state = &Scanner::stateString_0_1;
        buff.value += '\\';
        return true;
    }
    if (c == 'v'){
        state = &Scanner::stateString_0_1;
        buff.value += '\v';
        return true;
    }
    if (c == 'a'){
        state = &Scanner::stateString_0_1;
        buff.value += '\a';
        return true;
    }
    if (c == '?'){
        state = &Scanner::stateString_0_1;
        buff.value += '\?';
        return true;
    }
    std::pair <char,int> k;
    k.first = c;
    k.second = Line;
    throw k;
    return false;
}
bool Scanner::stateString( char c ) {
    if ((c == '\t') || (c == '\r') || (c == '\v') || (c == ' ')){
        return true;
    }
    if (c == '\n'){
        Line++;
        return true;
    }
    if (c == '"'){
        state = &Scanner::stateString_0_1;
        OpenQuotationMark = Line;
        return true;
    }
    return false;
}
bool Scanner::stateChar_0_1( char c ) {
    if (c == '\\'){
        state = &Scanner::stateChar_0_3;
        return true;
    }
    if (c != '\n'){
        state = &Scanner::stateChar_0_2;
        buff.value += c;
        return true;
    }
    std::pair <char,int> k;
    k.first = c;
    k.second = Line;
    throw k;
    return false;
}
bool Scanner::stateChar_0_2( char c ) {
    if (c == '\''){
        state = &Scanner::stateChar_1;
        OpenApostrophe = 0;
        return true;
    }
    std::pair <char,int> k;
    k.first = c;
    k.second = Line;
    throw k;
    return false;
}
bool Scanner::stateChar_0_3( char c ) {
    if (c == 'b'){
        state = &Scanner::stateChar_0_2;
        buff.value += '\b';
        return true;
    }
    if (c == 'f'){
        state = &Scanner::stateChar_0_2;
        buff.value += '\f';
        return true;
    }
    if (c == 'n'){
        state = &Scanner::stateChar_0_2;
        buff.value += '\n';
        return true;
    }
    if (c == 'r'){
        state = &Scanner::stateChar_0_2;
        buff.value += '\r';
        return true;
    }
    if (c == 't'){
        state = &Scanner::stateChar_0_2;
        buff.value += '\t';
        return true;
    }
    if (c == '"'){
        state = &Scanner::stateChar_0_2;
        buff.value += '\"';
        return true;
    }
    if (c == '\''){
        state = &Scanner::stateChar_0_2;
        buff.value += '\'';
        return true;
    }
    if (c == '\\'){
        state = &Scanner::stateChar_0_2;
        buff.value += '\\';
        return true;
    }
    if (c == 'v'){
        state = &Scanner::stateChar_0_2;
        buff.value += '\v';
        return true;
    }
    if (c == 'a'){
        state = &Scanner::stateChar_0_2;
        buff.value += '\a';
        return true;
    }
    if (c == '?'){
        state = &Scanner::stateChar_0_2;
        buff.value += '\?';
        return true;
    }
    std::pair <char,int> k;
    k.first = c;
    k.second = Line;
    throw k;
    return false;
}
bool Scanner::stateChar_1( char c ) {
    return false;
}

bool Scanner::TakeLexeme( std::vector <Lexeme> &program_data, char& c ){
    buff.value = "";
    state = &Scanner::stateInitial;
    std::pair <char,int> error;
    while (!fin.eof()){
        if ( !(this->*state)( c )) {
            break;
        }
        fin.get(c);
    }
    if ((state == &Scanner::stateHex_0_1)
    || (state == &Scanner::stateDouble_0_1)
    || (state == &Scanner::stateDouble_0_2)
    || (state == &Scanner::stateDouble_0_3)
    || (state == &Scanner::statePluralComment_0_1)
    || (state == &Scanner::statePluralComment_0_2)
    || (state == &Scanner::stateString_0_1)
    || (state == &Scanner::stateString_0_2)
    || (state == &Scanner::stateChar_0_1)
    || (state == &Scanner::stateChar_0_2)
    || (state == &Scanner::stateChar_0_3)
    || ((state == &Scanner::stateInitial) && (c != ' ') && (c != '\t') && (c != '\v') && (c != '\r') && (c != '\n'))){
        error.first = c;
        error.second = Line;
        throw error;
    }
    buff.line_number = Line;
    program_data.push_back(buff);
    return !fin.eof();
}

class Parser {
public:
    explicit Parser( std::vector <Lexeme> program_data, const char* sourceFileName ){
        check_information = program_data;
        error.first.value = "end of file";
        lexeme_pointer = 0;
        file_name = sourceFileName;
        commands.emplace_back(new Call("main"));
        function_table["malloc"].get_arguments_number() = 1;
        function_table["printf"].get_arguments_number() = 1;
        function_table["scanf"].get_arguments_number() = 1;
    }
    ~Parser() = default;

    void Run() {
        try {
            if (!HasLexeme()){
                error.first.value = "end of file";
                error.second = "translation unit";
                throw error;
            }
            if (!Translation_Unit()){
                throw error;
            }
            if (HasLexeme()) {
                error.first = check_information[lexeme_pointer];
                error.second = "end of file";
                throw error;
            }
        }
        catch (const std::pair <Lexeme,std::string> &k){
            std::cout << file_name << ": " << k.first.line_number << ": " << k.first.value << " found, but " << k.second
            << " expected" << std::endl;
            exit(1);
        }
        catch (const char* k){
            std::cout << k << std::endl;
            exit(1);
        }
        catch (const std::pair<int, std::string> &k){
            std::cout << file_name << ": " << k.first << ": " << "Semantic error: " << k.second << std::endl;
            exit(1);
        }
    }

    std::vector <std::shared_ptr<Commander>>& get_commands(){
        return commands;
    }
    std::map <std::string, Function>& get_function_table(){
        return function_table;
    }
private:
    std::vector <Lexeme> check_information;
    std::pair <Lexeme, std::string> error;
    int lexeme_pointer;
    std::string file_name;
    Function fun;
    std::vector <std::shared_ptr<Commander>> commands;
    std::map <std::string, Function> function_table = {{"printf", Function()},
                                                       {"scanf", Function()},
                                                       {"malloc", Function()}};
    int cyclic_instruction_start = -1;
    bool func_def_flag = false;
    std::queue <int> break_list = {};
    Type_Table type_table;
    bool HasLexeme();
    int CreateType(const std::string name, int point_to, int pointer_degree){
        auto new_t = Type(point_to, pointer_degree + 1);
        int new_ID = type_table.AddMember(name, new_t);
        type_table.add_to_command_table_2(new_ID, new_ID, LexemEqually, new Equally<int, int>);
        type_table.set_address(point_to, new_ID);
        return new_ID;
    }

    bool Translation_Unit(); //Done
    bool External_Declaration(); //Done
    bool Function_Definition(); //Done
    bool Primary_Expression(int& a); //Done
    bool Postfix_Expression(int& a); //Done
    bool Expression_Arguments_List(int& count, int& par_type); //Done
    bool Unary_Expression(int& a); //Done
    bool Unary_Operator(LexemeType& lex); //Done
    bool Expression_Cast_To_Type(int& a); //Done
    bool Multiplicative_Expression(int& a); //Done
    bool Additive_Expression(int& a); //Done
    bool Shift_Expression(int& a); //Done
    bool Relation_Expression(int& a); //Done
    bool Equality_Expression(int& a); //Done
    bool AND_Expression(int& a); //Done
    bool XOR_Expression(int& a); //Done
    bool OR_Expression(int& a); //Done
    bool AND_Logical_Expression(int& a); //Done
    bool OR_Logical_Expression(int& a); //Done
    bool Conditional_Expression(int& a); //Done
    bool Assignment_Expression(int& a); //Done
    bool Assignment_Operator(LexemeType& lex); //Done
    bool Expression(int& a); //Done
    bool Constant_Expression(int& a); //Done
    bool Declaration(); //Done
    bool Declaration_Specifiers(std::string& word_set); //Done
    bool Initializer_Declarator_List(std::string& word); //Done
    bool Initializer_Declarator(std::string& word); //Done
    bool Memory_Class_Specifier(); //Not implemented
    bool Type_Specifier(std::string& word); //Done
    bool Type_Qualifier(); //TODO Very unlikely, not in the near future
    bool Structures_Or_Unions_Specifier(); //Not implemented
    bool Structure_Or_Union(); //Not implemented
    bool Structure_Declaration_List(); //Not implemented
    bool Structure_Declaration(); //Not implemented
    bool Specifier_Qualifier_List(std::string& word_set); //Done
    bool Structure_Declarator_List(); //Not implemented
    bool Structure_Declarator(); //Not implemented
    bool Enum_Specifier(); //Not implemented
    bool Enum_List(); //Not implemented
    bool Enumerator(); //Not implemented
    bool Declarator(int& type_state, int& a, bool flag); //Done
    bool Declarator_Itself(int& type_state, int& a, bool flag); //Done
    bool Pointer(int& count); //Done
    bool Type_Qualifier_List(); //Not implemented
    bool Parameter_Type_List(int& a); //Done
    bool Parameter_List(int& a); //Done
    bool Parameter_Declaration();
    bool Identifier_List(); //Not implemented
    bool Initializer(int& a); //Done
    bool Initializer_List(); //Not implemented
    bool Type_Name(int& type_ID);
    bool Abstract_Declarator();
    bool Abstract_Declarator_Itself();
    bool Typedef_Name(); //Not implemented
    bool Instruction(bool& ret);
    bool Marked_Instruction(bool& ret); //Not implemented
    bool Expression_Instruction();
    bool Composite_Instruction(bool& ret);
    bool Declaration_List();
    bool Instruction_List(bool& ret);
    bool Choice_Instruction(bool& ret);
    bool Cyclic_Instruction(bool& ret);
    bool Transfer_Instruction(bool& ret);
    bool Function_Call(int& a); //Done
};

bool Parser::HasLexeme() {
    return lexeme_pointer < check_information.size();
}

bool Parser::Translation_Unit() {
    bool err = true;
    if (!External_Declaration()){
        return false;
    }
    while(HasLexeme() && err){
        err = External_Declaration();
    }
    return err;
}
bool Parser::External_Declaration() {
    //int saved_lexeme_pointer = lexeme_pointer;
    fun = Function();
    if (Function_Definition()){
        func_def_flag = false;
        function_table[fun.get_fun_name()] = fun;
        return true;
    }
    else{
        return false;
    }
    //lexeme_pointer = saved_lexeme_pointer;
    //return Declaration();
}
bool Parser::Function_Definition() {
    int saved_lexeme_pointer = lexeme_pointer;
    int type_state, a;
    bool ret = false;
    std::string t_name;
    if (!Declaration_Specifiers(t_name)){
        lexeme_pointer = saved_lexeme_pointer;
    }
    if (t_name.empty()){
        type_state = 0;
    }
    else if (!type_table.FindMember(t_name)){
        throw "Unknown type name";
    }
    else {
        type_state = type_table.table_names[t_name].first;
    }
    if (!Declarator(type_state, a, false)){
        return false;
    }
    saved_lexeme_pointer = lexeme_pointer;
    int saved_commands_pointer = commands.size();
    if (!Declaration_List()){
        lexeme_pointer = saved_lexeme_pointer;
        commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());
    }
    if (!Composite_Instruction(ret)) {
        return false;
    }
    if ((!ret) && (a == 0)){
        commands.emplace_back(new Return());
    }
    else if ((!ret) && (a != 0)){
        throw "Expected \"return\" at the end of non-void function";
    }
    return true;
}
bool Parser::Primary_Expression(int& a) {
    if(!HasLexeme()){
        error.second = "operand or expression";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type == LexemIdentifier){
        if (fun.get_var_table().find(check_information[lexeme_pointer].value) == fun.get_var_table().end()){
            ////////////////////////////////////////////////////////////////////////////////////////////////
            throw "Unknown identifier";                                                                   //
            //return false;                                                                               //
            ////////////////////////////////////////////////////////////////////////////////////////////////
        }
        else{
            commands.emplace_back(new Push(check_information[lexeme_pointer]));
            a = std::get<0>(fun.get_var_parameters_table()[check_information[lexeme_pointer].value]);
        }
        lexeme_pointer++;
        return true;
    }
    else if (check_information[lexeme_pointer].l_class == C_LexemConstant){
        commands.emplace_back(new Push(check_information[lexeme_pointer], a));
        lexeme_pointer++;
        return true;
    }
    if (check_information[lexeme_pointer].type != LexemLeftParenthesis){
        error.second = "'('";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    if (!Expression(a)){
        return false;
    }
    if(!HasLexeme()){
        error.second = "')'";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemRightParenthesis){
        error.second = "')'";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    return true;
}
bool Parser::Function_Call(int &a) {
    int saved_lexeme_pointer, saved_commands_pointer;
    std::string name;
    if(!HasLexeme()){
        error.second = "operand or expression";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].l_class != C_LexemIdentifier){
        error.second = "operand or expression";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    if (function_table.find(check_information[lexeme_pointer].value) == function_table.end()){
        return false;
    }
    name = check_information[lexeme_pointer].value;
    lexeme_pointer++;
    if (check_information[lexeme_pointer].type != LexemLeftParenthesis) {
        return false;
    }
    lexeme_pointer++;
    int params = 0, par_scanf;
    saved_lexeme_pointer = lexeme_pointer;
    saved_commands_pointer = commands.size();
    if (!Expression_Arguments_List(params, par_scanf)) {
        lexeme_pointer = saved_lexeme_pointer;
        commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());
    }
    if (params != function_table[name].get_arguments_number()){
        ///////////////////////////////////////////////////////////////
        throw "Invalid number of function parameters";
        ///////////////////////////////////////////////////////////////
    }
    if (!HasLexeme() || check_information[lexeme_pointer].type != LexemRightParenthesis) {
        error.second = "closing )";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    if (name == "scanf"){
        commands.emplace_back(new Scanf());
        a = -1;
    }
    else {
        a = function_table[name].get_return_type();
        commands.emplace_back(new Call(name));
    }
    return true;
}
bool Parser::Postfix_Expression(int& a) {
    int saved_lexeme_pointer, saved_commands_pointer, res_type_1, res_type_2;
    saved_commands_pointer = commands.size();
    saved_lexeme_pointer = lexeme_pointer;
    if (!Function_Call(res_type_1)) {
        lexeme_pointer = saved_lexeme_pointer;
        commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());
        if (!Primary_Expression(res_type_1)) {
            return false;
        }
    }
    saved_lexeme_pointer = lexeme_pointer;
    saved_commands_pointer = commands.size();
    while (HasLexeme()) {
        if (check_information[lexeme_pointer].type == LexemLeftBracket) {
            lexeme_pointer++;
            if (!Expression(res_type_2)) {
                break;
            }
            if (!HasLexeme() || check_information[lexeme_pointer].type != LexemRightBracket) {
                break;
            }
            lexeme_pointer++;
            if (res_type_1 < 15){
            ////////////////////////////////////////////////////////////////////////////////////////////////
                throw "Value is not an array or pointer";                                                 //                 //
            ////////////////////////////////////////////////////////////////////////////////////////////////
            }
            else{
                commands.emplace_back(type_table.get_command_table_1(res_type_2, LexemLeftBracket));
                std::cout << "Addressing" << std::endl;
                res_type_1 = type_table.table_names[type_table.table_ID[res_type_1]].second.get_point_to_type();
            }
        }
        else if (check_information[lexeme_pointer].type == LexemMinusMinus) {
            commands.emplace_back(type_table.get_command_table_1(res_type_1, LexemMinusMinus_Post));
            std::cout << "PostfixDecrement" << std::endl;
            if (res_type_1 < 5){
                res_type_1 = 5;
            }
            lexeme_pointer++;
        }
        else if (check_information[lexeme_pointer].type == LexemPlusPlus) {
            commands.emplace_back(type_table.get_command_table_1(res_type_1, LexemPlusPlus_Post));
            std::cout << "PostfixIncrement" << std::endl;
            if (res_type_1 < 5){
                res_type_1 = 5;
            }
            lexeme_pointer++;
        }
        else if (check_information[lexeme_pointer].type == LexemPoint //not implemented yet
            || check_information[lexeme_pointer].type == LexemDot) {
            throw "Not emplemented yet";
            lexeme_pointer++;
            if (!HasLexeme() || check_information[lexeme_pointer].l_class != C_LexemIdentifier) {
                break;
            }
            lexeme_pointer++;
        }
        else{
            break;
        }
        saved_lexeme_pointer = lexeme_pointer;
        saved_commands_pointer = commands.size();
    }
    a = res_type_1;
    lexeme_pointer = saved_lexeme_pointer;
    commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());
    return true;
}
bool Parser::Expression_Arguments_List(int& count, int& par_type) {
    int res_type;
    if (!Assignment_Expression(res_type)){
        return false;
    }
    commands.emplace_back(new Push(res_type));
    par_type = res_type;
    count++;
    if (HasLexeme() && check_information[lexeme_pointer].type == LexemComma){
        lexeme_pointer++;
        return Expression_Arguments_List(count, par_type);
    }
    return true;
}
bool Parser::Unary_Expression(int& a) {
    int res_type_1;
    int saved_commands_pointer = commands.size();
    int saved_lexeme_pointer = lexeme_pointer;
    if (Postfix_Expression(res_type_1)) {
        a = res_type_1;
        return true;
    }
    lexeme_pointer = saved_lexeme_pointer;
    commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());
    if (!HasLexeme()) {
        error.second = "operator";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type == LexemMinusMinus
        || check_information[lexeme_pointer].type == LexemPlusPlus) {
        LexemeType s_1 = check_information[lexeme_pointer].type;
        lexeme_pointer++;
        if (!Unary_Expression(res_type_1)){
            return false;
        }
        a = res_type_1;
        commands.emplace_back(type_table.get_command_table_1(a, s_1));
        std::cout << "Prefix Inc or Dec" << std::endl;
        if (a < 5){
            a = 5;
        }
        return true;
    }
    if (check_information[lexeme_pointer].type == LexemSizeof) {
        lexeme_pointer++;
        saved_commands_pointer = commands.size();
        saved_lexeme_pointer = lexeme_pointer;
        if (HasLexeme() && check_information[lexeme_pointer].type == LexemLeftParenthesis) {
            lexeme_pointer++;
            int type_ID;
            if (Type_Name(type_ID) && HasLexeme() && check_information[lexeme_pointer].type == LexemRightParenthesis) {
                commands.emplace_back(new SizeofType(type_ID));
                std::cout << "SizeofType" << std::endl;
                a = 5;
                lexeme_pointer++;
                return true;
            }
        }
        lexeme_pointer = saved_lexeme_pointer;
        commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());
        if (!Unary_Expression(res_type_1)){
            return false;
        }
        std::cout << "SizeofExpression" << std::endl;

        commands.emplace_back(type_table.get_command_table_1(res_type_1, LexemSizeof));
        a = 5;
        return true;
    }
    LexemeType lexemeType;
    if (!Unary_Operator(lexemeType)){
        return false;
    }
    if (!Expression_Cast_To_Type(res_type_1)){
        return false;
    }
    a = res_type_1;
    if ((lexemeType != LexemAsterisk) && (lexemeType != LexemAmpersand)) {
        commands.emplace_back(type_table.get_command_table_1(a, lexemeType));
        std::cout << "Unary Operator" << std::endl;
        if (a < 5) {
            a = 5;
        }
    }
    else if (lexemeType == LexemAmpersand){
        commands.emplace_back(new PointerTo());
        std::cout << "PointerTo" << std::endl;
        if (type_table.table_names[type_table.table_ID[a]].second.get_address_type() < 0){
            a = CreateType(type_table.table_ID[a] + "*", res_type_1, type_table.table_names[type_table.table_ID[res_type_1]].second.get_pointer_degree());
        }
        else{
            a = type_table.table_names[type_table.table_ID[res_type_1]].second.get_address_type();
        }
    }
    else{
        if (a < 15){
            ////////////////////////////////////////////////////////////////////////////////////////////////
            throw "Value is not an array or pointer";                                                     //
            ////////////////////////////////////////////////////////////////////////////////////////////////
        }
        commands.emplace_back(new RemovePointer());
        std::cout << "RemovePointer" << std::endl;
        a = type_table.table_names[type_table.table_ID[res_type_1]].second.get_point_to_type();
    }
    return true;
}
bool Parser::Unary_Operator(LexemeType& lex) {
    if (!HasLexeme()){
        error.second = "unary operator";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemAmpersand
        && check_information[lexeme_pointer].type != LexemExclamationMark
        && check_information[lexeme_pointer].type != LexemTilde
        && check_information[lexeme_pointer].type != LexemMinus
        && check_information[lexeme_pointer].type != LexemPlus
        && check_information[lexeme_pointer].type != LexemAsterisk){
        error.second = "unary operator";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lex = check_information[lexeme_pointer].type;
    lexeme_pointer++;
    return true;
}
bool Parser::Expression_Cast_To_Type(int& a) {
    int res_type_1;
    int saved_lexeme_pointer = lexeme_pointer;
    int saved_commands_pointer = commands.size();
    if (HasLexeme() && check_information[lexeme_pointer].type == LexemLeftParenthesis) {
        lexeme_pointer++;
        int type_ID;
        if (Type_Name(type_ID) && HasLexeme() && check_information[lexeme_pointer].type == LexemRightParenthesis) {
            lexeme_pointer++;
            if (!Expression_Cast_To_Type(res_type_1)){
                return false;
            }
            commands.emplace_back(type_table.get_command_table_type_cast(type_ID, res_type_1));
            std::cout << "TypeCast" << std::endl;
            a = type_ID;
            return true;
        }
    }
    lexeme_pointer = saved_lexeme_pointer;
    commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());
    return Unary_Expression(a);
}
bool Parser::Multiplicative_Expression(int& a) {
    int saved_lexeme_pointer, saved_commands_pointer, res_type_1, res_type_2;
    if (!Expression_Cast_To_Type(res_type_1)){
        return false;
    }
    saved_lexeme_pointer = lexeme_pointer;
    saved_commands_pointer = commands.size();
    while (HasLexeme()){
        if (check_information[lexeme_pointer].type != LexemAsterisk
            && check_information[lexeme_pointer].type != LexemSlash
            && check_information[lexeme_pointer].type != LexemPercent){
            break;
        }
        LexemeType lexemeType = check_information[lexeme_pointer].type;
        lexeme_pointer++;
        if (!Expression_Cast_To_Type(res_type_2)){
            break;
        }
        commands.emplace_back(type_table.get_command_table_2(res_type_1, res_type_2, lexemeType));
        std::cout << "Multiplicative operator" << std::endl;
        if (res_type_2 > res_type_1){
            res_type_1 = res_type_2;
        }
        saved_lexeme_pointer = lexeme_pointer;
        saved_commands_pointer = commands.size();
    }
    lexeme_pointer = saved_lexeme_pointer;
    commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());
    a = res_type_1;
    return true;
}
bool Parser::Additive_Expression(int& a) {
    int saved_lexeme_pointer, saved_commands_pointer, res_type_1, res_type_2;
    if (!Multiplicative_Expression(res_type_1)){
        return false;
    }
    saved_lexeme_pointer = lexeme_pointer;
    saved_commands_pointer = commands.size();
    while (HasLexeme()){
        if (check_information[lexeme_pointer].type != LexemPlus
            && check_information[lexeme_pointer].type != LexemMinus){
            break;
        }
        LexemeType lexemeType = check_information[lexeme_pointer].type;
        lexeme_pointer++;
        if (!Multiplicative_Expression(res_type_2)){
            break;
        }
        std::cout << "Additive operator" << std::endl;
        commands.emplace_back(type_table.get_command_table_2(res_type_1, res_type_2, lexemeType));
        std::cout << "Additive operator" << std::endl;
        if (res_type_2 > res_type_1){
            res_type_1 = res_type_2;
        }
        saved_lexeme_pointer = lexeme_pointer;
        saved_commands_pointer = commands.size();
    }
    lexeme_pointer = saved_lexeme_pointer;
    commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());
    a = res_type_1;
    return true;
}
bool Parser::Shift_Expression(int& a) {
    int saved_lexeme_pointer, saved_commands_pointer, res_type_1, res_type_2;
    if (!Additive_Expression(res_type_1)){
        return false;
    }
    saved_lexeme_pointer = lexeme_pointer;
    saved_commands_pointer = commands.size();
    while (HasLexeme()){
        if (check_information[lexeme_pointer].type != LexemMoveRight
            && check_information[lexeme_pointer].type != LexemMoveLeft){
            break;
        }
        LexemeType lexemeType = check_information[lexeme_pointer].type;
        lexeme_pointer++;
        if (!Additive_Expression(res_type_2)){
            break;
        }
        commands.emplace_back(type_table.get_command_table_2(res_type_1, res_type_2, lexemeType));
        std::cout << "Shift operator" << std::endl;
        if (res_type_2 > res_type_1){
            res_type_1 = res_type_2;
        }
        saved_lexeme_pointer = lexeme_pointer;
        saved_commands_pointer = commands.size();
    }
    lexeme_pointer = saved_lexeme_pointer;
    commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());
    a = res_type_1;
    return true;
}
bool Parser::Relation_Expression(int& a) {
    int saved_lexeme_pointer, saved_commands_pointer, res_type_1, res_type_2;
    if (!Shift_Expression(res_type_1)){
        return false;
    }
    saved_lexeme_pointer = lexeme_pointer;
    saved_commands_pointer = commands.size();
    while (HasLexeme()){
        if (check_information[lexeme_pointer].type != LexemLess
            && check_information[lexeme_pointer].type != LexemLessEqual
            && check_information[lexeme_pointer].type != LexemGreater
            && check_information[lexeme_pointer].type != LexemGreaterEqual){
            break;
        }
        LexemeType lexemeType = check_information[lexeme_pointer].type;
        lexeme_pointer++;
        if (!Shift_Expression(res_type_2)){
            break;
        }
        commands.emplace_back(type_table.get_command_table_2(res_type_1, res_type_2, lexemeType));
        std::cout << "Relation operator" << std::endl;
        if (res_type_2 > res_type_1){
            res_type_1 = res_type_2;
        }
        saved_lexeme_pointer = lexeme_pointer;
        saved_commands_pointer = commands.size();
    }
    lexeme_pointer = saved_lexeme_pointer;
    commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());
    a = res_type_1;
    return true;
}
bool Parser::Equality_Expression(int& a) {
    int saved_lexeme_pointer, saved_commands_pointer, res_type_1, res_type_2;
    if (!Relation_Expression(res_type_1)){
        return false;
    }
    saved_lexeme_pointer = lexeme_pointer;
    saved_commands_pointer = commands.size();
    while (HasLexeme()){
        if (check_information[lexeme_pointer].type != LexemEqual
            && check_information[lexeme_pointer].type != LexemNotEqual){
            break;
        }
        LexemeType lexemeType = check_information[lexeme_pointer].type;
        lexeme_pointer++;
        if (!Relation_Expression(res_type_2)){
            break;
        }
        commands.emplace_back(type_table.get_command_table_2(res_type_1, res_type_2, lexemeType));
        std::cout << "Equality operator" << std::endl;
        if (res_type_2 > res_type_1){
            res_type_1 = res_type_2;
        }
        saved_lexeme_pointer = lexeme_pointer;
        saved_commands_pointer = commands.size();
    }
    lexeme_pointer = saved_lexeme_pointer;
    commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());
    a = res_type_1;
    return true;
}
bool Parser::AND_Expression(int& a) {
    int saved_lexeme_pointer, saved_commands_pointer, res_type_1, res_type_2;
    if (!Equality_Expression(res_type_1)){
        return false;
    }
    saved_lexeme_pointer = lexeme_pointer;
    saved_commands_pointer = commands.size();
    while (HasLexeme()){
        if (check_information[lexeme_pointer].type != LexemAmpersand){
            break;
        }
        LexemeType lexemeType = check_information[lexeme_pointer].type;
        lexeme_pointer++;
        if (!Equality_Expression(res_type_2)){
            break;
        }
        commands.emplace_back(type_table.get_command_table_2(res_type_1, res_type_2, lexemeType));
        std::cout << "ByteAnd" << std::endl;
        if (res_type_2 > res_type_1){
            res_type_1 = res_type_2;
        }
        saved_lexeme_pointer = lexeme_pointer;
        saved_commands_pointer = commands.size();
    }
    lexeme_pointer = saved_lexeme_pointer;
    commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());
    a = res_type_1;
    return true;
}
bool Parser::XOR_Expression(int& a) {
    int saved_lexeme_pointer, saved_commands_pointer, res_type_1, res_type_2;
    if (!AND_Expression(res_type_1)){
        return false;
    }
    saved_lexeme_pointer = lexeme_pointer;
    saved_commands_pointer = commands.size();
    while (HasLexeme()){
        if (check_information[lexeme_pointer].type != LexemSuffix){
            break;
        }
        LexemeType lexemeType = check_information[lexeme_pointer].type;
        lexeme_pointer++;
        if (!AND_Expression(res_type_2)){
            break;
        }
        commands.emplace_back(type_table.get_command_table_2(res_type_1, res_type_2, lexemeType));
        std::cout << "ByteXor" << std::endl;
        if (res_type_2 > res_type_1){
            res_type_1 = res_type_2;
        }
        saved_lexeme_pointer = lexeme_pointer;
        saved_commands_pointer = commands.size();
    }
    lexeme_pointer = saved_lexeme_pointer;
    commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());
    a = res_type_1;
    return true;
}
bool Parser::OR_Expression(int& a) {
    int saved_lexeme_pointer, saved_commands_pointer, res_type_1, res_type_2;
    if (!XOR_Expression(res_type_1)){
        return false;
    }
    saved_lexeme_pointer = lexeme_pointer;
    saved_commands_pointer = commands.size();
    while (HasLexeme()){
        if (check_information[lexeme_pointer].type != LexemStraightSlash){
            break;
        }
        LexemeType lexemeType = check_information[lexeme_pointer].type;
        lexeme_pointer++;
        if (!XOR_Expression(res_type_2)){
            break;
        }
        commands.emplace_back(type_table.get_command_table_2(res_type_1, res_type_2, lexemeType));
        std::cout << "ByteOr" << std::endl;
        if (res_type_2 > res_type_1){
            res_type_1 = res_type_2;
        }
        saved_lexeme_pointer = lexeme_pointer;
        saved_commands_pointer = commands.size();
    }
    lexeme_pointer = saved_lexeme_pointer;
    commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());
    a = res_type_1;
    return true;
}
bool Parser::AND_Logical_Expression(int& a) {
    int saved_lexeme_pointer, saved_commands_pointer, res_type_1, res_type_2;
    if (!OR_Expression(res_type_1)){
        return false;
    }
    saved_lexeme_pointer = lexeme_pointer;
    saved_commands_pointer = commands.size();
    while (HasLexeme()){
        if (check_information[lexeme_pointer].type != LexemDoubleAmpersand){
            break;
        }
        LexemeType lexemeType = check_information[lexeme_pointer].type;
        lexeme_pointer++;
        if (!OR_Expression(res_type_2)){
            break;
        }
        commands.emplace_back(type_table.get_command_table_2(res_type_1, res_type_2, lexemeType));
        std::cout << "LogicalAnd" << std::endl;
        if (res_type_2 > res_type_1){
            res_type_1 = res_type_2;
        }
        saved_lexeme_pointer = lexeme_pointer;
        saved_commands_pointer = commands.size();
    }
    lexeme_pointer = saved_lexeme_pointer;
    commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());
    a = res_type_1;
    return true;
}
bool Parser::OR_Logical_Expression(int& a) {
    int saved_lexeme_pointer, saved_commands_pointer, res_type_1, res_type_2;
    if (!AND_Logical_Expression(res_type_1)){
        return false;
    }
    saved_lexeme_pointer = lexeme_pointer;
    saved_commands_pointer = commands.size();
    while (HasLexeme()){
        if (check_information[lexeme_pointer].type != LexemDoubleStraightSlash){
            break;
        }
        LexemeType lexemeType = check_information[lexeme_pointer].type;
        lexeme_pointer++;
        if (!AND_Logical_Expression(res_type_2)){
            break;
        }
        commands.emplace_back(type_table.get_command_table_2(res_type_1, res_type_2, lexemeType));
        std::cout << "LogicalOr" << std::endl;
        if (res_type_2 > res_type_1){
            res_type_1 = res_type_2;
        }
        saved_lexeme_pointer = lexeme_pointer;
        saved_commands_pointer = commands.size();
    }
    lexeme_pointer = saved_lexeme_pointer;
    commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());
    a = res_type_1;
    return true;
}
bool Parser::Conditional_Expression(int& a) {
    int saved_lexeme_pointer, saved_commands_pointer, res_type_1, res_type_2, res_type_3;
    if (!OR_Logical_Expression(res_type_1)){
        return false;
    }
    saved_lexeme_pointer = lexeme_pointer;
    saved_commands_pointer = commands.size();
    if (!HasLexeme() || check_information[lexeme_pointer].type != LexemQuestionMark){
        a = res_type_1;
        return true;
    }
    lexeme_pointer++;
    if (!Expression(res_type_2)){
        a = res_type_1;
        lexeme_pointer = saved_lexeme_pointer;
        commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());
        return true;
    }
    if (!HasLexeme() || check_information[lexeme_pointer].type != LexemColon){
        a = res_type_1;
        lexeme_pointer = saved_lexeme_pointer;
        commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());
        return true;
    }
    lexeme_pointer++;
    if (!Conditional_Expression(res_type_3)){
        a = res_type_1;
        lexeme_pointer = saved_lexeme_pointer;
        commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());
        return true;
    }
    a = res_type_2 > res_type_3 ? res_type_2 : res_type_3;
    commands.emplace_back(type_table.get_command_table_3(res_type_1, a));
    std::cout << "Condition operand" << std::endl;
    return true;
} //TODO Change set of commands at command_matching_table_3 (or maybe not...??? ohh, am not sure)
bool Parser::Assignment_Expression(int& a) {
    int saved_lexeme_pointer = lexeme_pointer;
    int res_type_1, res_type_2;
    int saved_commands_pointer = commands.size();
    LexemeType lexemeType;
    if (Unary_Expression(res_type_1) && Assignment_Operator(lexemeType) && Assignment_Expression(res_type_2)) {
        if (res_type_2 == -1){
            res_type_2 = res_type_1;
        }
        commands.emplace_back(type_table.get_command_table_2(res_type_1, res_type_2, lexemeType));
        std::cout << "Assignment operator" << std::endl;
        a = res_type_1;
        return true;
    }
    lexeme_pointer = saved_lexeme_pointer;
    commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());
    return Conditional_Expression(a);
}
bool Parser::Assignment_Operator(LexemeType& lex) {
    if (!HasLexeme()){
        error.second = "assignment operator";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemSlashEqual
        && check_information[lexeme_pointer].type != LexemPercentEqual
        && check_information[lexeme_pointer].type != LexemPlusEqual
        && check_information[lexeme_pointer].type != LexemMinusEqual
        && check_information[lexeme_pointer].type != LexemMoveRightEqual
        && check_information[lexeme_pointer].type != LexemMoveLeftEqual
        && check_information[lexeme_pointer].type != LexemAsteriskEqual
        && check_information[lexeme_pointer].type != LexemAmpersandEqual
        && check_information[lexeme_pointer].type != LexemSuffixEqual
        && check_information[lexeme_pointer].type != LexemStraightSlashEqual
        && check_information[lexeme_pointer].type != LexemEqually){
        error.second = "assignment operator";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lex = check_information[lexeme_pointer].type;
    lexeme_pointer++;
    return true;
}
bool Parser::Expression(int& a) {
    int saved_lexeme_pointer, saved_commands_pointer, res_type_1, res_type_2;
    if (!Assignment_Expression(res_type_1)){
        return false;
    }
    saved_lexeme_pointer = lexeme_pointer;
    saved_commands_pointer = commands.size();
    while (HasLexeme()){
        if (check_information[lexeme_pointer].type != LexemComma){
            break;
        }
        LexemeType lexemeType = check_information[lexeme_pointer].type;
        lexeme_pointer++;
        if (!Assignment_Expression(res_type_2)){
            break;
        }
        commands.emplace_back(type_table.get_command_table_2(res_type_1, res_type_2, lexemeType));
        std::cout << "LogicalOr" << std::endl;
        if (res_type_2 > res_type_1){
            res_type_1 = res_type_2;
        }
        saved_lexeme_pointer = lexeme_pointer;
        saved_commands_pointer = commands.size();
    }
    lexeme_pointer = saved_lexeme_pointer;
    commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());
    a = res_type_1;
    return true;
}
bool Parser::Constant_Expression(int& a) {
    if (!HasLexeme()){
        error.first.value = "end of file";
        error.second = "Constant expression";
    }
    if (check_information[lexeme_pointer].type != LexemDecIntConst){
        error.first = check_information[lexeme_pointer];
        error.second = "Constant expression";
        return false;
    }
    a = check_information[lexeme_pointer].GetDecInt();
    lexeme_pointer++;
    return true;
    //return Conditional_Expression(a);
} ///////////////////////////////////////////??????????????????????????????????
bool Parser::Declaration() {
    std::string word_set_1;
    int saved_lexeme_pointer;
    if (!Declaration_Specifiers(word_set_1)){
        return false;
    }
    int saved_commands_pointer = commands.size();
    saved_lexeme_pointer = lexeme_pointer;
    if (!Initializer_Declarator_List(word_set_1)){
        lexeme_pointer = saved_lexeme_pointer;
        commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());
    }
    if (!HasLexeme()){
        error.second = "';'";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemSemicolon){
        error.second = "';'";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    return true;
}
bool Parser::Declaration_Specifiers(std::string& word_set) {
    std::string type_specifiers, word;
    int saved_lexeme_pointer = lexeme_pointer;
    if (Type_Specifier(word)){
        int saved_lexeme_pointer_01 = lexeme_pointer;
        if (!Declaration_Specifiers(type_specifiers)){
            lexeme_pointer = saved_lexeme_pointer_01;
        }
        if (type_specifiers.empty()){
            word_set = word;
        }
        else {
            word_set = word + " " + type_specifiers;
        }
        return true;
    }
    lexeme_pointer = saved_lexeme_pointer;
    if (Type_Qualifier()){
        throw "Type qualifiers are not implemented yet";
        int saved_lexeme_pointer_02 = lexeme_pointer;
        if (!Declaration_Specifiers(word)){
            lexeme_pointer = saved_lexeme_pointer_02;
        }
        return true;
    }
    lexeme_pointer = saved_lexeme_pointer;
    if (Memory_Class_Specifier()){
        throw "Memory class specifiers are not implemented yet";
        int saved_lexeme_pointer_03 = lexeme_pointer;
        if (!Declaration_Specifiers(word)){
            lexeme_pointer = saved_lexeme_pointer_03;
        }
        return true;
    }
    return false;
}
bool Parser::Initializer_Declarator_List(std::string& word) {
    int saved_lexeme_pointer;
    if (!Initializer_Declarator(word)){
        return false;
    }
    int saved_commands_pointer = commands.size();
    saved_lexeme_pointer = lexeme_pointer;
    while (HasLexeme()){
        if (check_information[lexeme_pointer].type != LexemComma){
            break;
        }
        lexeme_pointer++;
        if (!Initializer_Declarator(word)){
            break;
        }
        saved_commands_pointer = commands.size();
        saved_lexeme_pointer = lexeme_pointer;
    }
    commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());
    lexeme_pointer = saved_lexeme_pointer;
    return true;
}
bool Parser::Initializer_Declarator(std::string& word) {
    int saved_lexeme_pointer;
    int res_type_1, res_type_2, type_state;
    if (!type_table.FindMember(word)){
        throw "Unknown type name";
    }
    type_state = type_table.table_names[word].first;
    if (!Declarator(type_state, res_type_1, false)){
        return false;
    }
    saved_lexeme_pointer = lexeme_pointer;
    int saved_commands_pointer = commands.size();
    if (!HasLexeme() || check_information[lexeme_pointer].type != LexemEqually){
        commands.emplace_back(new Pop());
        return true;
    }
    lexeme_pointer++;
    if (!Initializer(res_type_2)) {
        lexeme_pointer = saved_lexeme_pointer;
        commands.erase(commands.cbegin() + saved_commands_pointer, commands.cend());
        commands.emplace_back(new Pop());
        return false;
    }
    else {
        commands.emplace_back(type_table.get_command_table_2(res_type_1, res_type_2, LexemEqually));
    }
    commands.emplace_back(new Pop());
    return true;
}
bool Parser::Memory_Class_Specifier() {
    if (!HasLexeme()){
        error.second = "memory class specifier";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemAuto
        && check_information[lexeme_pointer].type != LexemRegister
        && check_information[lexeme_pointer].type != LexemStatic
        && check_information[lexeme_pointer].type != LexemExtern
        && check_information[lexeme_pointer].type != LexemTypedef){
        error.second = "memory class specifier";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    throw "Memory class specifiers are not implemented yet";
    lexeme_pointer++;
    return true;
}
bool Parser::Type_Specifier(std::string& word) {
    int saved_lexeme_pointer = lexeme_pointer;
    if (Structures_Or_Unions_Specifier()){
        throw "Structures or unions are not implemented yet";
        return true;
    }
    lexeme_pointer = saved_lexeme_pointer;
    if (Enum_Specifier()){
        throw "Enum list is not implemented yet";
        return true;
    }
    /*
    lexeme_pointer = saved_lexeme_pointer;
    if (Typedef_Name()){
        return true;
    }
     */
    lexeme_pointer = saved_lexeme_pointer;
    if (!HasLexeme()){
        error.second = "type specifier";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type == LexemVoid
        || check_information[lexeme_pointer].type == LexemChar
        || check_information[lexeme_pointer].type == LexemShort
        || check_information[lexeme_pointer].type == LexemInt
        || check_information[lexeme_pointer].type == LexemLong
        || check_information[lexeme_pointer].type == LexemFloat
        || check_information[lexeme_pointer].type == LexemDouble
        || check_information[lexeme_pointer].type == LexemSigned
        || check_information[lexeme_pointer].type == LexemUnsigned){
        word = check_information[lexeme_pointer].GetString();
        lexeme_pointer++;
        return true;
    }
    return false;
}
bool Parser::Type_Qualifier() {
    if (!HasLexeme()){
        error.second = "type qualifier";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemConst
        && check_information[lexeme_pointer].type != LexemVolatile){
        error.second = "type qualifier";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    throw "Type qualifiers are not implemented yet";
    lexeme_pointer++;
    return true;
}
bool Parser::Structures_Or_Unions_Specifier() {
    int saved_lexeme_pointer;
    if (!Structure_Or_Union()){
        return false;
    }
    if (HasLexeme() && check_information[lexeme_pointer].type == LexemIdentifier){
        lexeme_pointer++;
        saved_lexeme_pointer = lexeme_pointer;
        if (HasLexeme() && check_information[lexeme_pointer].type == LexemLeftBrace) {
            lexeme_pointer++;
            if (Structure_Declaration_List() && HasLexeme() && check_information[lexeme_pointer].type == LexemRightBrace) {
                lexeme_pointer++;
                return true;
            }
        }
        lexeme_pointer = saved_lexeme_pointer;
        return true;
    }
    if (!HasLexeme()){
        error.second = "identifier or structure declaration list";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemLeftBrace){
        error.second = "structure declaration list";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    if (!Structure_Declaration_List()){
        return false;
    }
    if (!HasLexeme()){
        error.second = "'}'";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemRightBrace){
        error.second = "'}'";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    return true;
}
bool Parser::Structure_Or_Union() {
    if (!HasLexeme()){
        error.second = "'struct' or 'union'";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemStruct
        && check_information[lexeme_pointer].type != LexemUnion){
        error.second = "'struct' or 'union'";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    return true;
}
bool Parser::Structure_Declaration_List() {
    int saved_lexeme_pointer;
    if (!Structure_Declaration()){
        return false;
    }
    saved_lexeme_pointer = lexeme_pointer;
    while(HasLexeme()){
        if (!Structure_Declaration()){
            break;
        }
        saved_lexeme_pointer = lexeme_pointer;
    }
    lexeme_pointer = saved_lexeme_pointer;
    return true;
}
bool Parser::Structure_Declaration() {
    std:: string a;
    if (!Specifier_Qualifier_List(a)){
        return false;
    }
    if (!Structure_Declarator_List()){
        return false;
    }
    if (!HasLexeme()){
        error.second = "';'";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemSemicolon){
        error.second = "';'";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    return true;
}
bool Parser::Specifier_Qualifier_List(std::string& word_set) {
    std::string type_specifiers, word;
    int saved_lexeme_pointer = lexeme_pointer;
    if (Type_Specifier(word)){
        int saved_lexeme_pointer_01 = lexeme_pointer;
        if (!Specifier_Qualifier_List(type_specifiers)){
            lexeme_pointer = saved_lexeme_pointer_01;
        }
        if (type_specifiers.empty()){
            word_set = word;
        }
        else {
            word_set = word + " " + type_specifiers;
        }
        return true;
    }
    lexeme_pointer = saved_lexeme_pointer;
    if (Type_Qualifier()){
        throw "Type qualifiers are not implemented yet";
        int saved_lexeme_pointer_02 = lexeme_pointer;
        if (!Specifier_Qualifier_List(type_specifiers)){
            lexeme_pointer = saved_lexeme_pointer_02;
        }
        return true;
    }
    return false;
}
bool Parser::Structure_Declarator_List() {
    throw "Structures or unions are not implemented yet";
    int saved_lexeme_pointer;
    if (!Structure_Declarator()){
        return false;
    }
    saved_lexeme_pointer = lexeme_pointer;
    while (HasLexeme()){
        if (check_information[lexeme_pointer].type != LexemComma){
            break;
        }
        lexeme_pointer++;
        if (!Structure_Declarator()){
            break;
        }
        saved_lexeme_pointer = lexeme_pointer;
    }
    lexeme_pointer = saved_lexeme_pointer;
    return true;
}
bool Parser::Structure_Declarator() {
    throw "Structures or unions are not implemented yet";
    int saved_lexeme_pointer;
    int a, b;
    if (Declarator(a,b,false)){
        saved_lexeme_pointer = lexeme_pointer;
        if (HasLexeme() && check_information[lexeme_pointer].type == LexemColon) {
            lexeme_pointer++;
            if (Constant_Expression(a)) {
                return true;
            }
        }
        lexeme_pointer = saved_lexeme_pointer;
        return true;
    }
    if (!HasLexeme()){
        error.second = "declarator or constant expression";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemColon){
        error.second = "declarator or constant expression";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    return Constant_Expression(a);
}
bool Parser::Enum_Specifier() {
    int saved_lexeme_pointer;
    if (!HasLexeme()){
        error.second = "enum specifier";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemEnum){
        error.second = "enum specifier";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    if (HasLexeme() && check_information[lexeme_pointer].type == LexemIdentifier){
        lexeme_pointer++;
        saved_lexeme_pointer = lexeme_pointer;
        if (HasLexeme() && check_information[lexeme_pointer].type == LexemLeftBrace) {
            lexeme_pointer++;
            if (Enum_List() && HasLexeme() && check_information[lexeme_pointer].type == LexemRightBrace) {
                lexeme_pointer++;
                return true;
            }
        }
        lexeme_pointer = saved_lexeme_pointer;
        return true;
    }
    if (!HasLexeme()){
        error.second = "identifier or structure declaration list";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemLeftBrace){
        error.second = "structure declaration list";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    if (!Enum_List()){
        return false;
    }
    if (!HasLexeme()){
        error.second = "'}'";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemRightBrace){
        error.second = "'}'";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    return true;
}
bool Parser::Enum_List() {
    int saved_lexeme_pointer;
    if (!Enumerator()){
        return false;
    }
    saved_lexeme_pointer = lexeme_pointer;
    while (HasLexeme()){
        if (check_information[lexeme_pointer].type != LexemComma){
            break;
        }
        lexeme_pointer++;
        if (!Enumerator()){
            break;
        }
        saved_lexeme_pointer = lexeme_pointer;
    }
    lexeme_pointer = saved_lexeme_pointer;
    return true;
}
bool Parser::Enumerator() {
    int saved_lexeme_pointer;
    int a;
    if (!HasLexeme()){
        error.second = "enumerator";
        error.first.value = "end of line";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemIdentifier){
        error.second = "enumerator";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    saved_lexeme_pointer = lexeme_pointer;
    if (!HasLexeme() || check_information[lexeme_pointer].type != LexemEqually){
        return true;
    }
    lexeme_pointer++;
    if (!Constant_Expression(a)){
        lexeme_pointer = saved_lexeme_pointer;
    }
    return true;
}
bool Parser::Declarator(int& type_state, int& a, bool flag) {
    int saved_lexeme_pointer = lexeme_pointer;
    int count = 0;
    if (!Pointer(count)){
        lexeme_pointer = saved_lexeme_pointer;
    }
    if ((type_state == 0) && (count > 0)){
        throw "Can't take type pointing at \"void\"";
    }
    std::string name = type_table.table_ID[type_state];
    for (int i = 1; i <= count; i++){
        name += "*";
        if (!type_table.FindMember(name)){
            CreateType(name, type_state, type_table.table_names[type_table.table_ID[type_state]].second.get_pointer_degree());
        }
        type_state = type_table.table_names[name].first;
    }
    return Declarator_Itself(type_state, a, flag);
}
bool Parser::Declarator_Itself(int& type_state, int& a, bool flag) {
    int saved_lexeme_pointer;
    std::string var_name;
    Lexeme lex;
    if (!HasLexeme()){
        error.second = "declarator or identifier";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type == LexemIdentifier){
        lex = check_information[lexeme_pointer];
        var_name = lex.GetString();
        if ((function_table.find(var_name) != (function_table.end()))
        || (fun.get_var_table().find(var_name) != (fun.get_var_table().end()))){
            throw "Redefinition of name";
        }
        lexeme_pointer++;
    }
    else if (check_information[lexeme_pointer].type == LexemLeftParenthesis) {
        lexeme_pointer++;
        if (!Declarator(type_state, a, flag)){
            return false;
        }
        if (!HasLexeme()){
            error.second = "')'";
            error.first.value = "end of file";
            return false;
        }
        if (check_information[lexeme_pointer].type == LexemRightParenthesis){
            lexeme_pointer++;
        }
        else{
            error.second = "')'";
            error.first = check_information[lexeme_pointer];
            return false;
        }
        throw "Parenthesises in declarations are not implemented yet";
    }
    else{
        error.second = "declarator or identifier";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    //Function definition
    if (check_information[lexeme_pointer].type == LexemLeftParenthesis){
        if (func_def_flag){
            throw "Can't define function in the other's function body";
        }
        else{
            func_def_flag = true;
            fun.get_enter_point() = commands.size();
        }
        int saved_lexeme_pointer_02, parameter_number = 0;
        lexeme_pointer++;
        saved_lexeme_pointer_02 = lexeme_pointer;
        if (!Parameter_Type_List(parameter_number)){
            lexeme_pointer = saved_lexeme_pointer_02;
            if (!Identifier_List()) {
                lexeme_pointer = saved_lexeme_pointer_02;
            }
            else{
                throw "Such function definitions are not implemented yet";
            }
        }
        if (!HasLexeme() || check_information[lexeme_pointer].type != LexemRightParenthesis) {
            return false;
        }
        lexeme_pointer++;
        if ((var_name == "main") && (parameter_number != 0)){
            throw "Defining main function with arguments is not implemented yet";
        }
        function_table[var_name] = Function();
        function_table[var_name].get_arguments_number() = parameter_number;
        function_table[var_name].get_return_type() = type_state;
        fun.get_return_type() = type_state;
        fun.get_arguments_number() = parameter_number;
        fun.get_fun_name() = var_name;
        a = type_state;
        return true;
    }
    //Mass
    if (type_state == 0){
        throw "Can't create a void variable";
    }

    std::queue <int> var_parameters;
    std::stack <int> type_parameters;
    int add_number = 0;
    saved_lexeme_pointer = lexeme_pointer;
    while (HasLexeme()) {
        ////////////////////////////////////////////////////////////////////
        if (check_information[lexeme_pointer].type == LexemLeftBracket) {
            int saved_lexeme_pointer_01;
            lexeme_pointer++;
            saved_lexeme_pointer_01 = lexeme_pointer;
            if (!Constant_Expression(a)) {
                return false;
            }
            if (a <= 0){
                throw "Expected positive integer index in the massive definition";
            }
            var_parameters.push(a);
            if (!HasLexeme() || check_information[lexeme_pointer].type != LexemRightBracket) {
                break;
            }
            add_number++;
            lexeme_pointer++;
        }
        ////////////////////////////////////////////////////////////////////////////
        else{
            break;
        }
        saved_lexeme_pointer = lexeme_pointer;
    }
    std::string t_name = type_table.table_ID[type_state];
    for (int i = 1; i <= add_number; i++){
        type_parameters.push(type_state);
        t_name += "*";
        if (!type_table.FindMember(t_name)){
            CreateType(t_name, type_state, type_table.table_names[type_table.table_ID[type_state]].second.get_pointer_degree());
        }
        type_state = type_table.table_names[t_name].first;
    }
    fun.get_var_table()[var_name] = 0;
    fun.get_var_parameters_table()[var_name] = std::make_tuple(type_state, var_parameters, type_parameters, add_number == 0);
    fun.get_type_matching_table()[var_name] = type_state;
    a = type_state;
    commands.emplace_back(new Push(lex));
    if (flag){
        fun.fun_parameters.emplace_back(var_name);
    }
    lexeme_pointer = saved_lexeme_pointer;
    return true;
}
bool Parser::Pointer(int& count) {
    int saved_lexeme_pointer;
    if (!HasLexeme()){
        error.second = "pointer";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemAsterisk){
        error.second = "pointer";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    count++;
    lexeme_pointer++;
    saved_lexeme_pointer = lexeme_pointer;
    if (!Type_Qualifier_List()){
        lexeme_pointer = saved_lexeme_pointer;
    }
    else{
        throw "Type qualifiers are not implemented yet";
    }
    saved_lexeme_pointer = lexeme_pointer;
    if (!Pointer(count)){
        lexeme_pointer = saved_lexeme_pointer;
    }
    return true;
}
bool Parser::Type_Qualifier_List() {
    int saved_lexeme_pointer;
    if (!Type_Qualifier()){
        return false;
    }
    saved_lexeme_pointer = lexeme_pointer;
    while(HasLexeme()){
        if (!Type_Qualifier()){
            break;
        }
        saved_lexeme_pointer = lexeme_pointer;
    }
    lexeme_pointer = saved_lexeme_pointer;
    return true;
}
bool Parser::Parameter_Type_List(int& a) {
    return Parameter_List(a);
}
bool Parser::Parameter_List(int& a) {
    int saved_lexeme_pointer;
    if (!Parameter_Declaration()){
        return false;
    }
    a++;
    saved_lexeme_pointer = lexeme_pointer;
    while (HasLexeme()){
        if (check_information[lexeme_pointer].type != LexemComma){
            break;
        }
        lexeme_pointer++;
        if (!Parameter_Declaration()){
            break;
        }
        a++;
        saved_lexeme_pointer = lexeme_pointer;
    }
    lexeme_pointer = saved_lexeme_pointer;
    return true;
}
bool Parser::Parameter_Declaration() {
    std::string word_set_1;
    int type_state, res_type;
    int saved_lexeme_pointer;
    if (!Declaration_Specifiers(word_set_1)){
        return false;
    }
    if (!type_table.FindMember(word_set_1)){
        throw "Unknown type name";
    }
    type_state = type_table.table_names[word_set_1].first;
    saved_lexeme_pointer = lexeme_pointer;
    if (Declarator(type_state, res_type, true)){
        commands.emplace_back(new Pop());
        return true;
    }

    lexeme_pointer = saved_lexeme_pointer;
    if (!Abstract_Declarator()){
        lexeme_pointer = saved_lexeme_pointer;
    }
    throw "Abstract declarators are not implemented yet";
    return true;
}
bool Parser::Identifier_List() {
    int saved_lexeme_pointer;
    if (!HasLexeme()){
        error.second = "identifier";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemIdentifier){
        error.second = "identifier";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    saved_lexeme_pointer = lexeme_pointer;
    while (HasLexeme()){
        if (check_information[lexeme_pointer].type != LexemComma){
            break;
        }
        lexeme_pointer++;
        if (!HasLexeme() || check_information[lexeme_pointer].type != LexemIdentifier){
            break;
        }
        lexeme_pointer++;
        saved_lexeme_pointer = lexeme_pointer;
    }
    lexeme_pointer = saved_lexeme_pointer;
    return true;
}
bool Parser::Initializer(int& a) {
    int saved_lexeme_pointer = lexeme_pointer;
    if (Assignment_Expression(a)){
        return true;
    }
    lexeme_pointer = saved_lexeme_pointer;
    if (!HasLexeme()){
        error.second = "assignment expression or initializer list";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemLeftBrace){
        error.second = "assignment expression or initializer list";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    if (!Initializer_List()){
        return false;
    }
    if (HasLexeme() && check_information[lexeme_pointer].type == LexemComma){
        lexeme_pointer++;
    }
    if (!HasLexeme()){
        error.second = "'}'";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemRightBrace){
        error.second = "'}'";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    throw "Initializer lists are not implemented yet";
    return true;
}
bool Parser::Initializer_List() {
    int a;
    int saved_lexeme_pointer;
    if (!Initializer(a)){
        return false;
    }
    saved_lexeme_pointer = lexeme_pointer;
    while (HasLexeme()){
        if (check_information[lexeme_pointer].type != LexemComma){
            break;
        }
        lexeme_pointer++;
        if (!Initializer(a)){
            break;
        }
        saved_lexeme_pointer = lexeme_pointer;
    }
    lexeme_pointer = saved_lexeme_pointer;
    return true;
}
bool Parser::Type_Name(int& type_ID) {
    int saved_lexeme_pointer;
    std::string word_set;
    if (!Specifier_Qualifier_List(word_set)){
        return false;
    }
    if (!type_table.FindMember(word_set)){
        throw "Unknown type name";
    }
    type_ID = type_table.table_names[word_set].first;
    saved_lexeme_pointer = lexeme_pointer;
    if (!Abstract_Declarator()){
        lexeme_pointer = saved_lexeme_pointer;
    }
    else{
        throw "Abstract declarators are not implemented yet";
    }
    return true;
}
bool Parser::Abstract_Declarator() {
    int saved_lexeme_pointer = lexeme_pointer;
    int a;
    if (Pointer(a)){
        int saved_lexeme_pointer_01 = lexeme_pointer;
        if (!Abstract_Declarator_Itself()){
            lexeme_pointer = saved_lexeme_pointer_01;
        }
        return true;
    }
    lexeme_pointer = saved_lexeme_pointer;
    return Abstract_Declarator_Itself();
}
bool Parser::Abstract_Declarator_Itself() {
    int saved_lexeme_pointer = lexeme_pointer;
    int counter = 0;
    if (!HasLexeme()){
        error.second = "abstract declarator";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type == LexemLeftParenthesis){
        lexeme_pointer++;
        if (Abstract_Declarator() && HasLexeme() && check_information[lexeme_pointer].type == LexemRightParenthesis) {
            lexeme_pointer++;
            return true;
        }
    }
    lexeme_pointer = saved_lexeme_pointer;
    while (true) {
        if (!HasLexeme()){
            error.second = "constant expression or parameter type list";
            error.first.value = "end of file";
            break;
        }
        if (check_information[lexeme_pointer].type == LexemLeftParenthesis) {
            lexeme_pointer++;
            if (!Parameter_Type_List(counter)) {
                lexeme_pointer = saved_lexeme_pointer + 1;
            }
            if (!HasLexeme()){
                error.second = "')'";
                error.first.value = "end of file";
                break;
            }
            if (check_information[lexeme_pointer].type != LexemRightParenthesis){
                error.second = "')'";
                error.first = check_information[lexeme_pointer];
                break;
            }
            lexeme_pointer++;
            counter++;
        }
        else if (check_information[lexeme_pointer].type == LexemLeftBracket){
            lexeme_pointer++;
            if (!Parameter_Type_List(counter)) {
                lexeme_pointer = saved_lexeme_pointer + 1;
            }
            if (!HasLexeme()){
                error.second = "']'";
                error.first.value = "end of file";
                break;
            }
            if (check_information[lexeme_pointer].type != LexemRightBracket){
                error.second = "']'";
                error.first = check_information[lexeme_pointer];
                break;
            }
            lexeme_pointer++;
            counter++;
        }
        else{
            error.second = "constant expression or parameter type list";
            error.first = check_information[lexeme_pointer];
            break;
        }
        saved_lexeme_pointer = lexeme_pointer;
    }
    if (counter == 0){
        return false;
    }
    lexeme_pointer = saved_lexeme_pointer;
    return true;
}
bool Parser::Typedef_Name() {
    if (!HasLexeme()) {
        error.second = "identifier";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemIdentifier){
        error.second = "identifier";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    return true;
}
bool Parser::Instruction(bool& ret) {
    int saved_lexeme_pointer = lexeme_pointer;
    if (Marked_Instruction(ret)){
        return true;
    }
    lexeme_pointer = saved_lexeme_pointer;
    if (Expression_Instruction()){
        return true;
    }
    lexeme_pointer = saved_lexeme_pointer;
    if (Composite_Instruction(ret)){
        return true;
    }
    lexeme_pointer = saved_lexeme_pointer;
    if (Choice_Instruction(ret)){
        return true;
    }
    lexeme_pointer = saved_lexeme_pointer;
    if (Cyclic_Instruction(ret)){
        return true;
    }
    return Transfer_Instruction(ret);
}
bool Parser::Marked_Instruction(bool& ret) {
    int saved_lexeme_pointer = lexeme_pointer;
    int a;
    if (!HasLexeme()){
        error.second = "marked instruction";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type == LexemIdentifier
            || check_information[lexeme_pointer].type == LexemDefault){
        lexeme_pointer++;
        if (HasLexeme() && check_information[lexeme_pointer].type == LexemColon){
            lexeme_pointer++;
            if (Instruction(ret)){
                return true;
            }
        }
    }
    lexeme_pointer = saved_lexeme_pointer;
    if (check_information[lexeme_pointer].type != LexemCase){
        error.second = "marked instruction";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    if (!Constant_Expression(a)){
        return false;
    }
    if (!HasLexeme()){
        error.second = "instruction";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemColon){
        error.second = "instruction";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    return Instruction(ret);
}
bool Parser::Expression_Instruction() {
    int saved_lexeme_pointer = lexeme_pointer;
    int a;
    if (!Expression(a)){
        lexeme_pointer = saved_lexeme_pointer;
    }
    else if (a != 0){
        commands.emplace_back(new Pop());
    }
    if (!HasLexeme()){
        error.second = "';'";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemSemicolon){
        error.second = "';'";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    return true;
}
bool Parser::Composite_Instruction(bool& ret) {
    int saved_lexeme_pointer;
    if (!HasLexeme()) {
        error.second = "composite instruction";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemLeftBrace){
        error.second = "composite instruction";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    while (true) {
        saved_lexeme_pointer = lexeme_pointer;
        if (Declaration_List()) {
            continue;
        }
        lexeme_pointer = saved_lexeme_pointer;
        if (!Instruction_List(ret)) {
            lexeme_pointer = saved_lexeme_pointer;
            break;
        }
    }
    if (!HasLexeme()) {
        error.second = "'}'";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemRightBrace){
        error.second = "'}'";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    return true;
}
bool Parser::Declaration_List() {
    int saved_lexeme_pointer;
    if (!Declaration()){
        return false;
    }
    saved_lexeme_pointer = lexeme_pointer;
    while(HasLexeme()){
        if (!Declaration()){
            break;
        }
        saved_lexeme_pointer = lexeme_pointer;
    }
    lexeme_pointer = saved_lexeme_pointer;
    return true;
}
bool Parser::Instruction_List(bool& ret) {
    int saved_lexeme_pointer;
    bool ret_1, ret_2;
    if (!Instruction(ret_1)){
        return false;
    }
    saved_lexeme_pointer = lexeme_pointer;
    while(HasLexeme()){
        if (!Instruction(ret_2)){
            break;
        }
        ret_1 = ret_1 || ret_2;
        saved_lexeme_pointer = lexeme_pointer;
    }
    lexeme_pointer = saved_lexeme_pointer;
    ret = ret_1;
    return true;
}
bool Parser::Choice_Instruction(bool& ret) {
    int a;
    bool ret_1, ret_2;
    int saved_lexeme_pointer = lexeme_pointer;
    if (!HasLexeme()) {
        error.second = "choice instruction";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type == LexemSwitch) {
        throw "Switch-case is not implemented yet";
        lexeme_pointer++;
        if (HasLexeme() && check_information[lexeme_pointer].type == LexemLeftParenthesis) {
            lexeme_pointer++;
            if (Expression(a) && HasLexeme() && check_information[lexeme_pointer].type == LexemRightParenthesis) {
                lexeme_pointer++;
                if (Instruction(ret)){
                    return true;
                }
            }
        }
    }
    lexeme_pointer = saved_lexeme_pointer;
    if (check_information[lexeme_pointer].type != LexemIf) {
        error.second = "choice instruction";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    if (!HasLexeme() || check_information[lexeme_pointer].type != LexemLeftParenthesis) {
        error.second = "expression";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    if (!Expression(a)) {
        return false;
    }
    if (a > 10){
        throw "Invalid type of if-statement expression";
    }
    if (!HasLexeme() || check_information[lexeme_pointer].type != LexemRightParenthesis) {
        error.second = "')'";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    int saved_clear_place_1 = commands.size();
    commands.emplace_back();
    commands.emplace_back(new If());
    if (!Instruction(ret_1)) {
        return false;
    }
    saved_lexeme_pointer = lexeme_pointer;
    if (HasLexeme() && check_information[lexeme_pointer].type == LexemElse) {
        lexeme_pointer++;
    }
    int saved_clear_place_2 = commands.size();
    commands.emplace_back();
    commands.emplace_back(new Goto());
    commands[saved_clear_place_1].reset(new Push(commands.size()));
    int saved_commands_pointer = commands.size();
    if (!Instruction(ret_2)) {
        lexeme_pointer = saved_lexeme_pointer;
        commands.erase(commands.cbegin()+saved_commands_pointer,commands.cend());

    }
    commands[saved_clear_place_2].reset(new Push(commands.size()));
    ret = ret_1 || ret_2;
    return true;

}
bool Parser::Cyclic_Instruction(bool& ret) {
    int a;
    int saved_lexeme_pointer = lexeme_pointer;
    if (!HasLexeme()) {
        error.second = "cyclic instruction";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type == LexemWhile){
        cyclic_instruction_start = commands.size();
        lexeme_pointer++;
        if (HasLexeme() && check_information[lexeme_pointer].type == LexemLeftParenthesis){
            lexeme_pointer++;
            if (Expression(a) && HasLexeme() && check_information[lexeme_pointer].type == LexemRightParenthesis){
                int saved_clear_place = commands.size();
                lexeme_pointer++;
                commands.emplace_back();
                commands.emplace_back(new If());
                if (Instruction(ret)){
                    commands.emplace_back(new Push(cyclic_instruction_start));
                    commands.emplace_back(new Goto());
                    commands[saved_clear_place].reset(new Push(commands.size()));
                    cyclic_instruction_start = -1;
                    while (!break_list.empty()){
                        commands[break_list.front()].reset(new Push(commands.size()));
                        break_list.pop();
                    }
                    return true;
                }
            }
        }
    }
    else if (check_information[lexeme_pointer].type == LexemDo){
        throw "\"Do-while\" cycle is not implemented yet";
        lexeme_pointer++;
        if (Instruction(ret) && HasLexeme() && check_information[lexeme_pointer].type == LexemWhile){
            lexeme_pointer++;
            if (HasLexeme() && check_information[lexeme_pointer].type == LexemLeftParenthesis){
                lexeme_pointer++;
                if (Expression(a) && HasLexeme() && check_information[lexeme_pointer].type == LexemRightParenthesis){
                    lexeme_pointer++;
                    return true;
                }
            }
        }
    }
    lexeme_pointer = saved_lexeme_pointer;
    if (check_information[lexeme_pointer].type != LexemFor){
        error.second = "cyclic instruction";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    if (!HasLexeme()){
        error.second = "'('";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemLeftParenthesis){
        error.second = "'('";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    for (int i = 0; i < 2; i++){
        saved_lexeme_pointer = lexeme_pointer;
        if (!Expression(a)){
            lexeme_pointer = saved_lexeme_pointer;
        }
        if (!HasLexeme()){
            error.second = "';'";
            error.first.value = "end of file";
            return false;
        }
        if (check_information[lexeme_pointer].type != LexemSemicolon){
            error.second = "';'";
            error.first = check_information[lexeme_pointer];
            return false;
        }
        lexeme_pointer++;
    }
    saved_lexeme_pointer = lexeme_pointer;
    if (!Expression(a)){
        lexeme_pointer = saved_lexeme_pointer;
    }
    if (!HasLexeme()){
        error.second = "')'";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemRightParenthesis){
        error.second = "')'";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    return Instruction(ret);
}
bool Parser::Transfer_Instruction(bool& ret) {
    int a;
    int saved_lexeme_pointer = lexeme_pointer;
    if (!HasLexeme()) {
        error.second = "transfer instruction";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type == LexemContinue){
        lexeme_pointer++;
        if (HasLexeme() && check_information[lexeme_pointer].type == LexemSemicolon){
            lexeme_pointer++;
            if (cyclic_instruction_start == -1){
                throw "Using \"continue\" not in the cyclic instruction";
            }
            commands.emplace_back(new Push(cyclic_instruction_start));
            commands.emplace_back(new Goto());
            return true;
        }
    }
    if (check_information[lexeme_pointer].type == LexemBreak){
        lexeme_pointer++;
        if (HasLexeme() && check_information[lexeme_pointer].type == LexemSemicolon){
            lexeme_pointer++;
            if (cyclic_instruction_start == -1){
                throw "Using \"break\" not in the cyclic instruction";
            }
            break_list.push(commands.size());
            commands.emplace_back();
            commands.emplace_back(new Goto());
            return true;
        }
    }
    else if (check_information[lexeme_pointer].type == LexemGoto){
        lexeme_pointer++;
        if (HasLexeme() && check_information[lexeme_pointer].type == LexemIdentifier){
            lexeme_pointer++;
            if (HasLexeme() && check_information[lexeme_pointer].type == LexemSemicolon){
                lexeme_pointer++;
                throw "Goto is not implemented yet";
                return true;
            }
        }
    }
    lexeme_pointer = saved_lexeme_pointer;
    if (check_information[lexeme_pointer].type != LexemReturn){
        error.second = "transfer instruction";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    lexeme_pointer++;
    saved_lexeme_pointer = lexeme_pointer;
    if (!Expression(a)){
        lexeme_pointer = saved_lexeme_pointer;
    }
    if (!HasLexeme()) {
        error.second = "';'";
        error.first.value = "end of file";
        return false;
    }
    if (check_information[lexeme_pointer].type != LexemSemicolon) {
        error.second = "';'";
        error.first = check_information[lexeme_pointer];
        return false;
    }
    commands.emplace_back(new Return());
    ret = true;
    lexeme_pointer++;
    return true;
}

class Executer{
    HDD Address_Space;
    RAM Function_Stack;
    int EIP;
    Table Function_Table;
    std::stack <Object> stack;
    std::vector <std::shared_ptr<Commander>> commands;
public:
    ~Executer() = default;
    Executer( Table& function_table, std::vector <std::shared_ptr<Commander>>& c ){
        Function_Table = function_table;
        EIP = 0;
        commands = c;
    }

    void Run(){
        try {
            if (Function_Table.find("main") == Function_Table.end()) {
                throw "Expected definition of main function";
            }
            while (EIP != -1) {
                EIP = commands.at(EIP)->Execute(stack, EIP, Address_Space, Function_Stack, Function_Table);
            }
        }
        catch (const char* k){
            std::cout << k << std::endl;
            exit(1);
        }
    }
};

int main(int argc, char* argv[]) {
    std::vector <Lexeme> program_data = {};
    std::cout << "Lexical analysis status: ";
    Scanner scan(argv[1]);
    scan.Run(program_data);
    std::cout << "OK" << std::endl;
    std::cout << "Syntax analysis status: ";
    Parser parser(program_data, argv[1]);
    parser.Run();
    Executer executer(parser.get_function_table(), parser.get_commands());
    executer.Run();
    std::cout << "OK" << std::endl;
/*
    std::cout << "char: " << sizeof(unsigned char) << std::endl;
    std::cout << "int: " << sizeof(int) << std::endl;
    std::cout << "short: " << sizeof(short) << std::endl;
    std::cout << "long: " << sizeof(long) << std::endl;
    std::cout << "long long: " << sizeof(unsigned long long) << std::endl;
    std::cout << "char**: " << sizeof(char**) << std::endl;
    std::cout << "const char*: " << sizeof(const char*) << std::endl;
    std::cout << "int***: " << sizeof(int***) << std::endl;
    std::cout << "float: " << sizeof(float) << std::endl;
    std::cout << "double: " << sizeof(double) << std::endl;
*/

    //cout << "Lexeme list:" << endl;
    //for (int i = 0; i < program_data.size(); i++){
        //cout << program_data[i].value << endl;
    //}
    return 0;
}