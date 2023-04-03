#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector>
#include <utility>
#include <queue>
#include <stack>
#include "lexanalyzer.h"
#include "interface.h"
#include "expevaluator.h"


using namespace std;

class Interpreter
{
private:
	friend class Interface;
	friend class LexAnalyzer;
	friend class expEvaluator;
	bool check = false; //True implies that there is a vector whose conditional statement was true
	int programCodeIndex = 0;

public:
	bool interpretLine(string&, expEvaluator&, programType, int); //interprets line of code (whether it is user-input or Python file code)

	//-------------------------------------------------------------------------------------------------------------------------------------

	void interpretIf(stack<string>&, expEvaluator&); //analyzes the if/else blocks and evaluates the conditional statements

	void readIfVector(vector<vector<string>>, vector<bool>, expEvaluator&); //reads and executes the if/else blocks

	int programCodeIndexReturn(); //retures the programCodeIndex

	void programCodeIndexInsert(int); //updates the programCodeIndex

	//-------------------------------------------------------------------------------------------------------------------------------------

	void interpretWhile(stack<string>&, expEvaluator&); //analyzes, reads, and executes the while loop

	
};

#endif

