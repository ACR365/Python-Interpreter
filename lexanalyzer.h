#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <utility>
#include <string>
#include <vector>
#include "Interface.h"

using namespace std;

enum class categoryType
{
    KEYWORD,
    IDENTIFIER,
    STRING_LITERAL,
    NUMERIC_LITERAL,
    ASSIGNMENT_OP,
    ARITH_OP,
    LOGICAL_OP,
    RELATIONAL_OP,
    LEFT_PAREN,
    RIGHT_PAREN,
    COLON,
    COMMA,
    COMMENT,
    INDENT,
    UNKNOWN
};

typedef vector<vector<pair<string, categoryType>>> tokenType; //stores everything
typedef vector<pair<string, categoryType>> tokenLineType; //stores the line from tokenInfo 
typedef pair<string, categoryType> pairType; //stores the pair of string and categoryType

class LexicalAnalyzer
{
    public:
        tokenLineType generate(string); //generates the tokens

        void tokenInfoFill(tokenType);

        void showTokens(); //shows the tokens and their types

        void clearTokens(); //clears all the tokens in tokenInfo

    private:
        tokenType tokenInfo;
        friend class Interface;
        friend class Interpreter;
};

#endif



