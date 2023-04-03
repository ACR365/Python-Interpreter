#include <iostream>
#include <string>
#include <map>
#include <queue>
#include <algorithm>
#include "interpreter.h"
#include "lexanalyzer.h"
#include "interface.h"
#include "expevaluator.h"

using namespace std;

LexicalAnalyzer lexAnalyzer;
queue<string> ifStatement;

bool Interpreter::interpretLine(string& line, expEvaluator& expEvaluation, programType p, int q) //interprets line of code (whether it is user-input or Python file code)
{
	tokenLineType t = lexAnalyzer.generate(line); //tokenize the line of code
	string print; //global print variable to store the string from the print

	for (int i = 0; i != t.size(); i++)
	{
		if (t[i].second == categoryType::COMMENT) //ignores the comment and continues to the next line
		{
			return true;
		}
		if (t[i].second == categoryType::INDENT)
		{
			continue;
		}
		else if (t.size() > 3 && t[i].second == categoryType::IDENTIFIER && t[i + 1].second == categoryType::ASSIGNMENT_OP && (t[i + 2].second == categoryType::NUMERIC_LITERAL || t[i + 2].second == categoryType::IDENTIFIER) && t[i + 3].second == categoryType::ARITH_OP && (t[i + 4].second == categoryType::NUMERIC_LITERAL || t[i + 4].second == categoryType::IDENTIFIER)) //if t[i] is IDENTIFIER
		{
			vector<pair<string, categoryType>> v2;
			string key = t[i].first;
			for (int x = (i + 2); x <= (i + 4); x++)
			{
				v2.push_back(make_pair(t[x].first, t[x].second));
			}

			expEvaluation.inFixTopostFixVector(v2); //converts inFix to postFix
			int a = expEvaluation.postFixEvaluation(); //solves postFix expression
			string value = to_string(a);

			expEvaluation.storeVarT(key, value);
		}
		else if (t[i].second == categoryType::IDENTIFIER && t[i + 1].second == categoryType::ASSIGNMENT_OP && t[i + 2].second == categoryType::NUMERIC_LITERAL) //if the code encounters identifer (=) number || Example: x + 3
		{
			expEvaluation.storeVarTLT(t); //stores the identifier (key) and number (value) in the symbolTable
		}
		else if (t[i].second == categoryType::IDENTIFIER && t[i + 1].second == categoryType::ASSIGNMENT_OP && t[i + 2].second == categoryType::IDENTIFIER) //if the code encounters identifer (=) number || Example: x + 3
		{
			string value = expEvaluation.retrieveValue(t[i + 2].first);
			expEvaluation.storeVarT(t[i].first, value);
		}
		else if (t[i].second == categoryType::IDENTIFIER && t[i + 1].second == categoryType::ARITH_OP && (t[i + 2].second == categoryType::NUMERIC_LITERAL || t[i + 2].second == categoryType::IDENTIFIER)) //if the code encounters identifer(+) number || Example: x + y
		{

			for (int x = 0; x != i + 2; x++)
			{
				bool b = expEvaluation.checkKey(t[x].first); //checks if the identifier is in the symbolTable
				if (b == false)
				{
					cout << "Variable is not uninitalized!" << endl;
					return false;
				}
				else if (b == true)
				{
					x = i + 1;
				}

			}
			string key = t[i].first; //stores the identifer into the key

			vector<pair<string, categoryType>> v; //essentially creates a tokenLineType v
			v.push_back(make_pair(t[i].first, t[i].second)); //stores the three into vector v
			v.push_back(make_pair(t[i + 1].first, t[i + 1].second));
			v.push_back(make_pair(t[i + 2].first, t[i + 2].second));

			expEvaluation.inFixTopostFixVector(v); //converts inFix to postFix
			int a = expEvaluation.postFixEvaluation(); //solves postFix expression

			expEvaluation.vectorClear();
			v.clear();
		}
		else if (t[i].second == categoryType::IDENTIFIER && t[i + 1].second == categoryType::ASSIGNMENT_OP && t[i + 2].second == categoryType::KEYWORD) //if the code encounters x (=) keyword || ex y = int....
		{
			string KEY = t[i].first;
			if (t[i + 2].first == "int") //sets the data type
			{
				int VALUE = 0;
			}
			else if (t[i + 2].first == "float")
			{
				double VALUE = 0;
			}

			int y;
			for (int x = i + 2; x != t.size(); x++)
			{
				if (t[x].second == categoryType::KEYWORD && t[x].first == "input") //when it finds input, breaks out of loop
				{
					y = x;
					break;
				}
			}

			i = y;
			string STRING;
			for (int x = i; x != t.size(); x++)
			{
				if (t[x].second == categoryType::STRING_LITERAL) //when it finds the string literal, removes the quotes on each side
				{
					i = x;
					string s = t[x].first.substr(1, t[x].first.size());
					STRING = s.substr(0, s.size() - 1);
					break;
				}
			}

			cout << STRING;

			string VALUE;
			getline(cin, VALUE); //gets the full line
			expEvaluation.storeVarT(KEY, VALUE);


		}
		else if (t[i].second == categoryType::KEYWORD) //if the code encounters keywords like print, if, elif.....
		{
			if (t[i].first == "print")
			{
				i = i + 2;

				if (t[i].second == categoryType::IDENTIFIER)
				{
					string key = t[i].first;
					string value = expEvaluation.retrieveValue(key);
					print = print + value;
				}

				if (t[i].second == categoryType::STRING_LITERAL)
				{
					string p = t[i].first;

					for (int x = 1; x != p.size(); x++) //goes through string literals character by character
					{
						if (p[x] == '"' || p[x] == '\'') //if it encounters (") or ('), breaks out of loop
						{
							break;
						}
						else
						{
							print = print + p[x];
						}
					}
				}
				if (t[i + 1].second == categoryType::COMMA) //If there is a comma next to the string, adds space to print and essentially runs it again
				{
					print = print + " ";

					i = i + 2;
					while (t[i].second != categoryType::RIGHT_PAREN)
					{
						if (t[i].second == categoryType::STRING_LITERAL)
						{
							string p = t[i].first;
							string l = p.substr(1, p.size());
							print = print + l.substr(0, l.size() - 1);
						}
						else if (t[i].second == categoryType::IDENTIFIER && t[i + 1].second == categoryType::ARITH_OP && (t[i + 2].second == categoryType::NUMERIC_LITERAL || t[i + 2].second == categoryType::IDENTIFIER))
						{
							string key = t[i].first;

							vector<pair<string, categoryType>> v;
							v.push_back(make_pair(t[i].first, t[i].second));
							v.push_back(make_pair(t[i + 1].first, t[i + 1].second));
							v.push_back(make_pair(t[i + 2].first, t[i + 2].second));

							expEvaluation.inFixTopostFixVector(v);
							int a = expEvaluation.postFixEvaluation();
							string value = to_string(a);

							print = print + value;
							expEvaluation.vectorClear();
							v.clear();
							i = i + 2;
						}
						else if (t[i].second == categoryType::IDENTIFIER)
						{
							string key = t[i].first;
							string k = expEvaluation.retrieveValue(key);
							print = print + " " + k;
						}
						else if (t[i].second == categoryType::COMMA)
						{
							i++;
							continue;
						}

						if (t[i].second == categoryType::RIGHT_PAREN) //breaks out of comma loop
						{
							break;
						}
						i++;
					}
				}
				cout << print << endl;
			}
			else if (t[i].first == "if" || t[i].first == "elif" || t[i].first == "else") //if t[i].first is "if", "else", or "elif" (If Loop)
			{
				programCodeIndex = q; //inputs q as the programCodeIndex
				stack<string> ifStatement; //creates a stack to store the if block
				string programCodeInsert = p[programCodeIndex];
				ifStatement.push(programCodeInsert); //pushes the conditional statement back
				programCodeIndex++;

				tokenLineType check;
				while (!p[programCodeIndex].empty()) //will go through the rest of programCode
				{
					check = lexAnalyzer.generate(p[programCodeIndex]);
					if (check.empty()) //if it encounters an empty space, breaks out of it
					{
						break;
					}

					if ((check[0].second == categoryType::INDENT) || (check[0].second == categoryType::KEYWORD && check[0].first == "if") || (check[0].second == categoryType::KEYWORD && check[0].first == "elif") || (check[0].second == categoryType::KEYWORD && check[0].first == "else")) //if check[i].first is an indent or one of the three statements, push it back on the stack
					{
						programCodeInsert = p[programCodeIndex];
						ifStatement.push(programCodeInsert);
						programCodeIndex++;
					}
					else
					{
						break; //else, breaks out of the loop
					}
				}

				interpretIf(ifStatement, expEvaluation);
				return true;

			}
			else if (t[i].first == "while") //While Loop
			{
				stack<string> whileStatement; //creates a stack of string to hold the while block
				string programCodeIns = p[programCodeIndex];
				whileStatement.push(programCodeIns); //same idea as the ifStatement block above
				programCodeIndex++;

				tokenLineType check;
				while (!p[programCodeIndex].empty())
				{
					check = lexAnalyzer.generate(p[programCodeIndex]);
					if (check.empty())
					{
						break;
					}

					if (check[0].second == categoryType::INDENT) //since we are implementing basic while loop, just looks for the indent in front of the line of code
					{
						programCodeIns = p[programCodeIndex];
						whileStatement.push(programCodeIns);
						programCodeIndex++;
					}
					else
					{
						break;
					}
				}

				interpretWhile(whileStatement, expEvaluation);
				return true;
			}
			else
			{
				continue;
			}
		}

		return true;
	}
}

void Interpreter::interpretIf(stack<string>& ifStatements, expEvaluator& expEvaluation) //analyzes the if/else blocks and evaluates the conditional statements
{

	vector<string> v;
	vector<vector<string>> ifLoopStore; //will store the if blocks if the conditional statements are true
	vector<bool> ifLoopBoolStore;		//will store the booleans of the conditional statements

	vector<string> elseStatement; //special vector on for else block

	while (!ifStatements.empty()) 
	{
		string line = ifStatements.top();
		tokenLineType t = lexAnalyzer.generate(line);

		if (t.empty())
		{
			ifStatements.pop();
		}
		else
		{
			if (t[0].second == categoryType::KEYWORD && (t[0].first == "if" || t[0].first == "elif")) //if the KEYWORD = "if" or "elif"
			{
				typedef vector<pair<string, categoryType>> tokens; //creates an object to evaluate the conditonal statement
				tokens boolean;
				int i = 1;
				while (t[i].second != categoryType::COLON)
				{
					boolean.push_back(make_pair(t[i].first, t[i].second));
					i++;
				}

				expEvaluation.inFixTopostFixVector(boolean); //evaluates the conditional statement
				int b = expEvaluation.postFixEvaluation();
				bool check = b;
				if (b == true) //if true, it will process through that v and is pushed on the vector of vector of stirng
				{
					ifLoopStore.push_back(v);
					ifLoopBoolStore.push_back(b);
					v.clear();
					ifStatements.pop();
				}
				else if (b == false) //if false, it will not be process and clears vector v for the next block
				{
					v.clear();
					ifStatements.pop();
				}
			}
			else if (t[0].second == categoryType::KEYWORD && t[0].first == "else") 
			{
				elseStatement = v; //pushes v on elseStatement
				ifStatements.pop();
				v.clear(); //clears the vector v
			}
			else //else, pushes line on v
			{
				v.push_back(line);
				ifStatements.pop();
			}
		}

	}

	if (ifLoopStore.empty()) //if there were no if statements, will execute the elseStatement only
	{
		ifLoopStore.push_back(elseStatement);
		ifLoopBoolStore.push_back(1);
	}

	readIfVector(ifLoopStore, ifLoopBoolStore, expEvaluation);
}

void Interpreter::readIfVector(vector<vector<string>> VECTOR, vector<bool> BOOL, expEvaluator& expEvaluation) //reads and executes the if/else blocks
{
	typedef stack<vector<string>> vsString; //stack of vector<string> to reverse the order in which the if blocks are processed
	vsString v;
	stack<bool> b; //stack of boolean to reverse the order as well

	for (int i = 0; i != VECTOR.size(); i++)
	{
		v.push(VECTOR[i]); //reverses the order of the stacks
		b.push(BOOL[i]);
	}

	while (!v.empty())
	{
		bool boolean = b.top();
		vector<string> vs = v.top();
		if (boolean == true && check == false) //if the statement is true and there so far, no other block has been evaluated
		{
			check = true; //will update boolean check from the private boolean member in the Interpreter class
			vector<string> vec = vs;

			programType pcode;
			for (int j = 0; j != vec.size(); j++) //essentially, same procedure when reading code in interactive mode
			{
				int g = expEvaluation.tokenizeLine(vec[j]);
				if (g == -1)
				{
					continue;
				}
				else if (g == 5)
				{
					interpretLine(vec[j], expEvaluation, pcode, 0);
					v.pop();
					b.pop();
				}
				else
				{
					expEvaluation.choice(g, vec[j]);
					v.pop();
					b.pop();
				}
			}

		}
		else if ((v.size() == 1) && check != true) //if at the end, that means that is where the else statement is and if check is still false, execute the else statement
		{
			vector<string> vec = vs;

			programType p;
			for (int j = 0; j != vec.size(); j++)
			{
				int g = expEvaluation.tokenizeLine(vec[j]);

				if (g == -1)
				{
					continue;
				}
				else if (g == 5)
				{
					interpretLine(vec[j], expEvaluation, p, 0);
					v.pop();
					b.pop();
				}
				else
				{
					expEvaluation.choice(g, vec[j]);
					v.pop();
					b.pop();
				}
			}
		}

	}
}

int Interpreter::programCodeIndexReturn() //retures the programCodeIndex
{
	return programCodeIndex; 
}

void Interpreter::interpretWhile(stack<string>& s, expEvaluator& expEvaluation) //analyzes, reads, and executes the while loop
{
	vector<string> v;
	stack<string> whileLoop; //stores the while loop block

	while (!s.empty()) //used to reverse the order of the stack<string> s, so the while loop is on top
	{
		string top = s.top();
		if (top.empty())
		{
			s.pop();
		}
		else
		{
			whileLoop.push(top);
			s.pop();
		}

	}
	
	while (!whileLoop.empty()) //stores the new stack in vector v
	{
		v.push_back(whileLoop.top());
		whileLoop.pop();
	}
	
	tokenLineType t = lexAnalyzer.generate(v[0]); //holds the conditonal statement for the while loop
	
	typedef vector<pair<string, categoryType>> tokens;
	tokens boolean;
	int i = 1;
	while (t[i].second != categoryType::COLON)
	{
		boolean.push_back(make_pair(t[i].first, t[i].second));
		i++;
	}

	expEvaluation.inFixTopostFixVector(boolean); //evalutes the conditional statement
	int b = expEvaluation.postFixEvaluation();
	bool check = b;

	programType p;
	vector<string> whileVector; //stores the block after the conditonal statement
	for (int x = 1; x != v.size(); x++) //stores the while block in a new vector whileVector
	{
		whileVector.push_back(v[x]);
	}

	while (check) //while the conditonal is true, going through whileVector and executes the lines
	{
		for (int j = 0; j != whileVector.size(); j++)
		{
			int i = expEvaluation.tokenizeLine(whileVector[j]);

			if (i == -1)
			{
				continue;
			}
			else if (i == 5)
			{
				interpretLine(whileVector[j], expEvaluation, p, 0);
			}
			else
			{
				expEvaluation.choice(i, whileVector[j]);
			}
		}

		//will evaluate and update check
		tokens c;
		i = 1;
		while (t[i].second != categoryType::COLON)
		{
			c.push_back(make_pair(t[i].first, t[i].second));
			i++;
		}

		expEvaluation.inFixTopostFixVector(c);
		b = expEvaluation.postFixEvaluation();
		check = b;
	}
}

void Interpreter::programCodeIndexInsert(int x) //updates the programCodeIndex
{
	programCodeIndex = x;
}



