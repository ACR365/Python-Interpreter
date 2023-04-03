#ifndef EXPEVALUATOR_H
#define EXPEVALUATOR_H

#include <map>
#include <string>
#include <vector>
#include <utility>
#include "lexanalyzer.h"
#include "interpreter.h"

using namespace std;

class expEvaluator
{
private:
	typedef map<string, string> symbolTableType;
	symbolTableType symbolTable;

	typedef vector<pair<string, categoryType>> tokenPair;
	tokenPair inFixVect, postFixVect, stackVect, stringToVect;

	friend class Interface;
	friend class LexicalAnalyzer;
	friend class Interpreter;

public:

	void vectorClear(); //clears all vectors

	int tokenizeLine(string); //tokenize and adds the string into the stringToVect vector

	void choice(int&, string&);

	void inFixTopostFix(); //converts inFix expression to postFix expression

	void inFixTopostFix(tokenLineType); //converts inFix expression to postFix expression

	void inFixTopostFixVector(vector<pair<string, categoryType>>); //converts inFix expression to postFix expression

	int postFixEvaluation(); //evaluates the postfix expression

	void storeVariables(); //stores the front (identifier) and back (value) of stringToVect into the symbolTable

	void storeVarTLT(tokenLineType&); //stores the front (identifier) and back (value) of tokenLineType into the symbolTable

	void storeVarT(string&, string&); //stores the front (identifier) and back (value) into the symbolTable

	string retrieveValue(string&); //retrieves the value from the symbolTable

	void clearSymbolTable(); //clears the symbolTable

	int precedenceOperatorReturn(string); //returns the precedence of the operator

	void showVariables(); //show variables of the symbolTable

	bool checkKey(string); //checks if identifier is in symbolTable


};





#endif
