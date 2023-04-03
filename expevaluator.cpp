#include <iostream>
#include <stack>
#include <map>
#include <cctype>
#include "expevaluator.h"
#include "interpreter.h"
#include "lexanalyzer.h"

using namespace std;

Interpreter pystubi;
LexicalAnalyzer LexAnalyzer;

void expEvaluator::vectorClear() //clears all vectors
{
	inFixVect.clear();
	postFixVect.clear();
	stackVect.clear();
	stringToVect.clear();
}

int expEvaluator::tokenizeLine(string s) //tokenize and adds the string into the stringToVect vector
{

	vectorClear(); //clears all vectors

	int numberCount = 0;
	int logicalCount = 0;
	int relationalCount = 0;
	int leftParenCount = 0; //keeps track of the left parenthesis
	int rightParenCount = 0; //keep track of the right parenthesis

	categoryType token;
	string x;
	if (s.empty())
	{
		return -1;
	}

	tokenLineType t = LexAnalyzer.generate(s);

	stringToVect = t;


	if (stringToVect.size() == 1) //if the size is one, it is implied that the person is either inputing an key for a value in the symbolTable or an operator to look for precedence
	{
		if (stringToVect[0].second == categoryType::IDENTIFIER)
		{
			return 1;
		}
		else if (stringToVect[0].second == categoryType::ARITH_OP || stringToVect[0].second == categoryType::ASSIGNMENT_OP || stringToVect[0].second == categoryType::LOGICAL_OP || stringToVect[0].second == categoryType::RELATIONAL_OP)
		{
			return 2;
		}
		else
		{
			return 0;
		}
	}
	else if (stringToVect.size() == 3) //if the size is 3, they are either inputting key and values for the symbolTable or it is a small expression to be solved
	{
		if (stringToVect[0].second == categoryType::IDENTIFIER && stringToVect[1].second == categoryType::ASSIGNMENT_OP && stringToVect[2].second == categoryType::NUMERIC_LITERAL)
		{
			return 3;
		}
		else if (stringToVect[0].second == categoryType::NUMERIC_LITERAL && stringToVect[1].second == categoryType::LOGICAL_OP || stringToVect[1].second == categoryType::ARITH_OP || stringToVect[1].second == categoryType::RELATIONAL_OP && stringToVect[2].second == categoryType::NUMERIC_LITERAL)
		{
			return 4;
		}
		else if (stringToVect[0].second == categoryType::IDENTIFIER && (stringToVect[1].second == categoryType::LOGICAL_OP || stringToVect[1].second == categoryType::ARITH_OP || stringToVect[1].second == categoryType::RELATIONAL_OP) && (stringToVect[2].second == categoryType::NUMERIC_LITERAL || stringToVect[2].second == categoryType::IDENTIFIER))
		{
			return 4;
		}
		else if (stringToVect[0].second == categoryType::IDENTIFIER && stringToVect[1].second == categoryType::ASSIGNMENT_OP && stringToVect[2].second == categoryType::IDENTIFIER)
		{
			return 5;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		if (leftParenCount != rightParenCount) //If the parenthesis are not balanced, it returns an error
		{
			return 0;
		}
		else if (stringToVect[0].second == categoryType::COMMENT)
		{
			return 5;
		}
		else if (stringToVect[0].second == categoryType::IDENTIFIER && stringToVect[1].second == categoryType::ASSIGNMENT_OP && stringToVect[2].second == categoryType::KEYWORD)
		{
			return 5;
		}
		else if (stringToVect[0].second == categoryType::KEYWORD)
		{
			return 5;
		}
		else if (stringToVect[0].second == categoryType::INDENT)
		{
			return 5;
		}
		else if (stringToVect[0].second == categoryType::IDENTIFIER && stringToVect[1].second == categoryType::ASSIGNMENT_OP && (stringToVect[2].second == categoryType::NUMERIC_LITERAL || stringToVect[2].second == categoryType::IDENTIFIER) && stringToVect[3].second == categoryType::ARITH_OP && (stringToVect[4].second == categoryType::NUMERIC_LITERAL || stringToVect[4].second == categoryType::IDENTIFIER))
		{
			return 5;
		}
		if (stringToVect.back().second == categoryType::NUMERIC_LITERAL || stringToVect.back().second == categoryType::RIGHT_PAREN || stringToVect.back().second == categoryType::IDENTIFIER)
		{
			return 4;
		}
		if (stringToVect[0].second == categoryType::IDENTIFIER && stringToVect[1].second == categoryType::ASSIGNMENT_OP && (stringToVect[2].second == categoryType::NUMERIC_LITERAL || stringToVect[3].second == categoryType::IDENTIFIER))
		{
			return 3;
		}
		else
		{
			return 0;
		}
	}

}


void expEvaluator::choice(int& v, string& q)
{
	if (v == 1)
	{
		string value = retrieveValue(q);
		cout << "Key: " << q << endl;
		cout << "Value: " << value << endl;
	}
	else if (v == 2)
	{
		int precedence = precedenceOperatorReturn(q);
		cout << "Precedence Of Operator: " << precedence << endl;
	}
	else if (v == 3)
	{
		storeVariables();
	}
	else if (v == 4)
	{
		inFixTopostFix();
		int answer = postFixEvaluation();
		cout << "Answer: " << answer << endl;
	}
	else if (v == 0)
	{
		cout << "Error: Incorrect Syntax" << endl;
	}
}

void expEvaluator::inFixTopostFix() //converts inFix expression to postFix expression
{

	inFixVect = stringToVect;

	int precedenceX = 0;
	int precedenceY = 0;

	for (auto x : inFixVect)
	{
		if (x.second == categoryType::NUMERIC_LITERAL)
		{
			postFixVect.push_back(x);
		}
		else if (x.second == categoryType::IDENTIFIER)
		{
			postFixVect.push_back(x);
		}
		else if (x.second == categoryType::LEFT_PAREN)
		{
			stackVect.push_back(x);
		}
		else if (x.second == categoryType::RIGHT_PAREN)
		{
			while (stackVect.back().first != "(")
			{
				postFixVect.push_back(stackVect.back());
				stackVect.pop_back();

				if (stackVect.back().first == "(")
				{
					stackVect.pop_back();
					break;
				}
			}
		}
		else if (x.second == categoryType::ARITH_OP || x.second == categoryType::RELATIONAL_OP || x.second == categoryType::LOGICAL_OP)
		{
			if (stackVect.size() > 0)  //Sets the precedence of precedenceX and precedenceX
			{
				if (stackVect.back().first == "*" || stackVect.back().first == "/" || stackVect.back().first == "%")
				{
					precedenceY = 5;
				}
				else if (stackVect.back().first == "+" || stackVect.back().first == "-")
				{
					precedenceY = 4;
				}
				else if (stackVect.back().first == "<" || stackVect.back().first == ">" || stackVect.back().first == ">=" || stackVect.back().first == "<=" || stackVect.back().first == "!=" || stackVect.back().first == "==")
				{
					precedenceY = 3;
				}
				else if (stackVect.back().first == "and")
				{
					precedenceY = 2;
				}
				else if (stackVect.back().first == "or")
				{
					precedenceY = 1;
				}
				else if (stackVect.back().first == "not")
				{
					precedenceY = 0;
				}

				if (x.first == "*" || x.first == "/" || x.first == "%")
				{
					precedenceX = 5;
				}
				else if (x.first == "+" || x.first == "-")
				{
					precedenceX = 4;
				}
				else if (x.first == "<" || x.first == ">" || x.first == ">=" || x.first == "<=" || x.first == "!=" || x.first == "==")
				{
					precedenceX = 3;
				}
				else if (x.first == "and")
				{
					precedenceX = 2;
				}
				else if (x.first == "or")
				{
					precedenceX = 1;
				}
				else if (x.first == "not")
				{
					precedenceX = 0;
				}
			}

			while (!stackVect.empty() && (stackVect.back().second == categoryType::ARITH_OP || stackVect.back().second == categoryType::RELATIONAL_OP || stackVect.back().second == categoryType::LOGICAL_OP) && precedenceY >= precedenceX)
			{
				postFixVect.push_back(stackVect.back());
				stackVect.pop_back();

				if (stackVect.empty())
				{
					break;
				}
				else
				{

					//Redefines the precedence of precedenceX and precedenceY
					if (stackVect.back().first == "*" || stackVect.back().first == "/" || stackVect.back().first == "%")
					{
						precedenceY = 5;
					}
					else if (stackVect.back().first == "+" || stackVect.back().first == "-")
					{
						precedenceY = 4;
					}
					else if (stackVect.back().first == "<" || stackVect.back().first == ">" || stackVect.back().first == ">=" || stackVect.back().first == "<=" || stackVect.back().first == "!=" || stackVect.back().first == "==")
					{
						precedenceY = 3;
					}
					else if (stackVect.back().first == "and")
					{
						precedenceY = 2;
					}
					else if (stackVect.back().first == "or")
					{
						precedenceY = 1;
					}
					else if (stackVect.back().first == "not")
					{
						precedenceY = 0;
					}
				}

			}
			stackVect.push_back(x);
		}
	}

	while (!stackVect.empty())
	{
		postFixVect.push_back(stackVect.back());
		stackVect.pop_back();
	}

}

void expEvaluator::inFixTopostFixVector(vector<pair<string, categoryType>> v) //converts inFix expression to postFix expression
{

	inFixVect = v;

	int precedenceX = 0;
	int precedenceY = 0;

	for (auto x : inFixVect)
	{
		if (x.second == categoryType::NUMERIC_LITERAL)
		{
			postFixVect.push_back(x);
		}
		else if (x.second == categoryType::IDENTIFIER)
		{
			postFixVect.push_back(x);
		}
		else if (x.second == categoryType::LEFT_PAREN)
		{
			stackVect.push_back(x);
		}
		else if (x.second == categoryType::RIGHT_PAREN)
		{
			while (stackVect.back().first != "(")
			{
				postFixVect.push_back(stackVect.back());
				stackVect.pop_back();

				if (stackVect.back().first == "(")
				{
					stackVect.pop_back();
					break;
				}
			}
		}
		else if (x.second == categoryType::ARITH_OP || x.second == categoryType::RELATIONAL_OP || x.second == categoryType::LOGICAL_OP)
		{
			if (stackVect.size() > 0)  //Sets the precedence of precedenceX and precedenceX
			{
				if (stackVect.back().first == "*" || stackVect.back().first == "/" || stackVect.back().first == "%")
				{
					precedenceY = 5;
				}
				else if (stackVect.back().first == "+" || stackVect.back().first == "-")
				{
					precedenceY = 4;
				}
				else if (stackVect.back().first == "<" || stackVect.back().first == ">" || stackVect.back().first == ">=" || stackVect.back().first == "<=" || stackVect.back().first == "!=" || stackVect.back().first == "==")
				{
					precedenceY = 3;
				}
				else if (stackVect.back().first == "and")
				{
					precedenceY = 2;
				}
				else if (stackVect.back().first == "or")
				{
					precedenceY = 1;
				}
				else if (stackVect.back().first == "not")
				{
					precedenceY = 0;
				}

				if (x.first == "*" || x.first == "/" || x.first == "%")
				{
					precedenceX = 5;
				}
				else if (x.first == "+" || x.first == "-")
				{
					precedenceX = 4;
				}
				else if (x.first == "<" || x.first == ">" || x.first == ">=" || x.first == "<=" || x.first == "!=" || x.first == "==")
				{
					precedenceX = 3;
				}
				else if (x.first == "and")
				{
					precedenceX = 2;
				}
				else if (x.first == "or")
				{
					precedenceX = 1;
				}
				else if (x.first == "not")
				{
					precedenceX = 0;
				}
			}

			while (!stackVect.empty() && (stackVect.back().second == categoryType::ARITH_OP || stackVect.back().second == categoryType::RELATIONAL_OP || stackVect.back().second == categoryType::LOGICAL_OP) && precedenceY >= precedenceX)
			{
				postFixVect.push_back(stackVect.back());
				stackVect.pop_back();

				if (stackVect.empty())
				{
					break;
				}
				else
				{

					//Redefines the precedence of precedenceX and precedenceY
					if (stackVect.back().first == "*" || stackVect.back().first == "/" || stackVect.back().first == "%")
					{
						precedenceY = 5;
					}
					else if (stackVect.back().first == "+" || stackVect.back().first == "-")
					{
						precedenceY = 4;
					}
					else if (stackVect.back().first == "<" || stackVect.back().first == ">" || stackVect.back().first == ">=" || stackVect.back().first == "<=" || stackVect.back().first == "!=" || stackVect.back().first == "==")
					{
						precedenceY = 3;
					}
					else if (stackVect.back().first == "and")
					{
						precedenceY = 2;
					}
					else if (stackVect.back().first == "or")
					{
						precedenceY = 1;
					}
					else if (stackVect.back().first == "not")
					{
						precedenceY = 0;
					}
				}

			}
			stackVect.push_back(x);
		}
	}

	while (!stackVect.empty())
	{
		postFixVect.push_back(stackVect.back());
		stackVect.pop_back();
	}

}

int expEvaluator::postFixEvaluation() //evaluates the postfix expression
{
	stack<double> s;
	int v, value;
	int operand1 = 0;
	int operand2 = 0;

	for (auto x : postFixVect)
	{
		if (x.second == categoryType::NUMERIC_LITERAL)
		{
			v = stoi(x.first); //converts string to integer
			s.push(v);
		}
		if (x.second == categoryType::IDENTIFIER)
		{
			string value = retrieveValue(x.first);
			v = stoi(value); //converts string to integer
			s.push(v);
		}
		else if (x.second == categoryType::ARITH_OP)
		{
			if (s.size() >= 2)
			{
				operand2 = s.top();
				s.pop();
				operand1 = s.top();
				s.pop();
			}


			if (x.first == "+")
			{
				value = operand1 + operand2;
				s.push(value);
			}
			else if (x.first == "-")
			{
				value = operand1 - operand2;
				s.push(value);
			}
			else if (x.first == "*")
			{
				value = operand1 * operand2;
				s.push(value);
			}
			else if (x.first == "/")
			{
				value = operand1 / operand2;
				s.push(value);
			}
			else if (x.first == "%")
			{
				value = operand1 % operand2;
				s.push(value);
			}
		}
		else if (x.second == categoryType::LOGICAL_OP || x.second == categoryType::RELATIONAL_OP)
		{
			bool b; //Since we are dealing with logical and relational operators, we initialize a bool value
			if (s.size() >= 2)
			{
				operand2 = s.top();
				s.pop();
				operand1 = s.top();
				s.pop();
			}

			if (x.first == "and") //Converting the logical operatorrs
			{
				b = operand1 && operand2;
				value = b;
				s.push(value);
			}
			else if (x.first == "not")
			{
				b = operand1 != operand2;
				value = b;
				s.push(value);
			}
			else if (x.first == "or")
			{
				b = operand1 || operand2;
				value = b;
				s.push(value);
			}

			if (x.first == "<")
			{
				b = operand1 < operand2;
				value = b;
				s.push(value);
			}
			else if (x.first == "==")
			{
				b = operand1 == operand2;
				value = b;
				s.push(value);
			}
			else if (x.first == ">")
			{
				b = operand1 > operand2;
				value = b;
				s.push(value);
			}
			else if (x.first == "<=")
			{
				b = operand1 <= operand2;
				value = b;
				s.push(value);
			}
			else if (x.first == ">=")
			{
				b = operand1 >= operand2;
				value = b;
				s.push(value);
			}
			else if (x.first == "!=")
			{
				b = operand1 != operand2;
				value = b;
				s.push(value);
			}
		}
	}


	int answer = s.top();
	return answer;
}



void expEvaluator::storeVariables() //stores the front (identifier) and back (value) of stringToVect into the symbolTable
{
	string key, value;

	key = stringToVect[0].first;
	value = stringToVect[2].first;

	symbolTable[key] = value;

}

void expEvaluator::storeVarTLT(tokenLineType& t) //stores the front (identifier) and back (value) of stringToVect into the symbolTable
{

	string key, value;

	key = t.front().first;
	value = t.back().first;

	symbolTable[key] = value;

}


void expEvaluator::storeVarT(string& key, string& value) //stores the front (identifier) and back (value) of stringToVect into the symbolTable
{
	symbolTable[key] = value;

}

string expEvaluator::retrieveValue(string& key) //retrieves the value from the symbolTable
{
	return symbolTable[key];
}


void expEvaluator::clearSymbolTable() //clears the symbolTable
{
	symbolTable.clear();
}


int expEvaluator::precedenceOperatorReturn(string p) //returns the precedence of the operator
{
	int precedence = 0;

	if (p == "*" || p == "/" || p == "%")
	{
		precedence = 5;
	}
	else if (p == "+" || p == "-")
	{
		precedence = 4;
	}
	else if (p == "<" || p == ">" || p == ">=" || p == "<=" || p == "!=" || p == "==")
	{
		precedence = 3;
	}
	else if (p == "and")
	{
		precedence = 2;
	}
	else if (p == "or")
	{
		precedence = 1;
	}
	else if (p == "not")
	{
		precedence = 0;
	}

	return precedence;
}

void expEvaluator::showVariables()
{
	cout << "<---------Symbol Table Variables and Values--------->" << endl;

	for (auto x : symbolTable)
	{
		cout << x.first << " = " << x.second << endl;
	}

}

bool expEvaluator::checkKey(string key)
{

	for (auto x : symbolTable)
	{
		if (key == x.first)
		{
			string KEY = key;
			string VALUE = x.second;
			symbolTable[KEY] = VALUE;
			return true;
		}
	}

	return false;
}
