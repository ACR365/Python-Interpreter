#include <cctype> //reading string character by character
#include <iostream>
#include "lexanalyzer.h"
#include "expevaluator.h"
using namespace std;

expEvaluator expEval;

tokenLineType LexicalAnalyzer::generate(string line)
{
	tokenLineType lineTokens; //will store both the token and the type
	string token; //token is where we will store the string
	categoryType type; //where we store the categoryType 

	for (int i = 0; i != line.size(); i++) //inner loop: will get there line character by character
	{
		//Keywords
		if (line[i] == 'p' && line[i + 1] == 'r' && line[i + 2] == 'i' && line[i + 3] == 'n' && line[i + 4] == 't') //will look ahead to see if the rest of the character spells any keywords
		{
			type = categoryType::KEYWORD;
			token = "print"; //token automatically gets filled with keyword
			i = i + 4; //updates the index to the location of the last lookahead
			lineTokens.push_back(make_pair(token, type)); //makes pair of token and type and pushes it back into lineTokens
		}
		else if (line[i] == 'e' && line[i + 1] == 'l' && line[i + 2] == 'i' && line[i + 3] == 'f')
		{
			type = categoryType::KEYWORD;
			token = "elif";
			i = i + 3;
			lineTokens.push_back(make_pair(token, type));
		}
		else if (line[i] == 'i' && line[i + 1] == 'f')
		{
			type = categoryType::KEYWORD;
			token = "if";
			i = i + 1;
			lineTokens.push_back(make_pair(token, type));
		}
		else if (line[i] == 'e' && line[i + 1] == 'l' && line[i + 2] == 's' && line[i + 3] == 'e')
		{
			type = categoryType::KEYWORD;
			token = "else";
			i = i + 3;
			lineTokens.push_back(make_pair(token, type));
		}
		else if (line[i] == 'f' && line[i + 1] == 'o' && line[i + 2] == 'r')
		{
			type = categoryType::KEYWORD;
			token = "for";
			i = i + 2;
			lineTokens.push_back(make_pair(token, type));
		}
		else if (line[i] == 'f' && line[i + 1] == 'r' && line[i + 2] == 'o' && line[i + 3] == 'm')
		{
			type = categoryType::KEYWORD;
			token = "from";
			i = i + 3;
			lineTokens.push_back(make_pair(token, type));
		}
		else if (line[i] == 'i' && line[i + 1] == 'm' && line[i + 2] == 'p' && line[i + 3] == 'o' && line[i + 4] == 'r' && line[i + 5] == 't')
		{
			type = categoryType::KEYWORD;
			token = "import";
			i = i + 5;
			lineTokens.push_back(make_pair(token, type));
		}
		else if (line[i] == 'r' && line[i + 1] == 'e' && line[i + 2] == 't' && line[i + 3] == 'u' && line[i + 4] == 'r' && line[i + 5] == 'n')
		{
			type = categoryType::KEYWORD;
			token = "return";
			i = i + 5;
			lineTokens.push_back(make_pair(token, type));
		}
		else if (line[i] == 'b' && line[i + 1] == 'r' && line[i + 2] == 'e' && line[i + 3] == 'a' && line[i + 4] == 'k')
		{
			type = categoryType::KEYWORD;
			token = "break";
			i = i + 4;
			lineTokens.push_back(make_pair(token, type));
		}
		else if (line[i] == 'w' && line[i + 1] == 'h' && line[i + 2] == 'i' && line[i + 3] == 'l' && line[i + 4] == 'e')
		{
			type = categoryType::KEYWORD;
			token = "while";
			i = i + 4;
			lineTokens.push_back(make_pair(token, type));
		}
		else if (line[i] == 'i' && line[i + 1] == 'n' && line[i + 2] == 'p' && line[i + 3] == 'u' && line[i + 4] == 't')
		{
			type = categoryType::KEYWORD;
			token = "input";
			i = i + 4;
			lineTokens.push_back(make_pair(token, type));
		}
		else if (line[i] == 'd' && line[i + 1] == 'e' && line[i + 2] == 'f')
		{
			type = categoryType::KEYWORD;
			token = "def";
			i = i + 2;
			lineTokens.push_back(make_pair(token, type));
		}
		else if (line[i] == 'i' && line[i + 1] == 'n' && line[i + 2] == 't')
		{
			type = categoryType::KEYWORD;
			token = "int";
			i = i + 2;
			lineTokens.push_back(make_pair(token, type));
		}
		//String Literals
		else if (line[i] == '\"') //checks for for the single or double quotes
		{
			int y = 0; //the value will store the index of x when the loop ends
			type = categoryType::STRING_LITERAL;
			token = token + line[i];

			for (int x = (i + 1); x != line.size(); x++) //starts at i + 1; goes until it reaches another single or double quote
			{
				if (line[x] == '\"')
				{
					y = x;
					break;
				}
				else
				{
					token = token + line[x];
				}
			}
			token = token + line[y]; //adds the single or double quotes at the end
			i = y; //updates i to be y
			lineTokens.push_back(make_pair(token, type));
		}
		else if (line[i] == '\'') //checks for for the single or double quotes
		{
			int q = 0; //the value will store the index of x when the loop ends
			type = categoryType::STRING_LITERAL;
			token = token + line[i];

			for (int x = (i + 1); x != line.size(); x++) //starts at i + 1; goes until it reaches another single or double quote
			{
				if (line[x] == '\'')
				{
					q = x;
					break;
				}
				else
				{
					token = token + line[x];
				}
			}
			token = token + line[q]; //adds the single or double quotes at the end
			i = q; //updates i to be q
			lineTokens.push_back(make_pair(token, type));
		}
		else if (line[i] == 'a' && line[i + 1] == 'n' && line[i + 2] == 'd') //Logical Operators
		{
			type = categoryType::LOGICAL_OP;
			token = "and";
			i = i + 2;
			lineTokens.push_back(make_pair(token, type));
		}
		else if (line[i] == 'n' && line[i + 1] == 'o' && line[i + 2] == 't')
		{
			type = categoryType::LOGICAL_OP;
			token = "not";
			i = i + 2;
			lineTokens.push_back(make_pair(token, type));
		}
		else if (line[i] == 'o' && line[i + 1] == 'r')
		{
			type = categoryType::LOGICAL_OP;
			token = "or";
			i = i + 2;
			lineTokens.push_back(make_pair(token, type));
		}
		//Identifiers
		else if (isalpha(line[i]))
		{
			type = categoryType::IDENTIFIER;
			token = token + line[i];
			if (isalpha(line[i + 1])) //looks ahead to check for more letters
			{
				while (isalpha(line[i + 1]))
				{
					token = token + line[i + 1]; //will add the letter next to the character being examined
					i++;
				}
			}

			bool b = expEval.checkKey(token);

			if (b == false)
			{
				lineTokens.push_back(make_pair(token, type));
			}
		}
		//Operators
		else if (ispunct(line[i]))
		{
			if (line[i] == '<' && line[i + 1] == '=' || line[i] == '>' && line[i + 1] == '=') //Relational Operators
			{
				int y = 0;
				type = categoryType::RELATIONAL_OP;
				for (int x = i; x != line.size(); x++)
				{
					if (isspace(line[x]))
					{
						y = x;
						break;
					}
					else
					{
						token = token + line[x];
					}
				}
				i = y;
				lineTokens.push_back(make_pair(token, type));
			}
			else if (line[i] == '=' && line[i + 1] == '=' || line[i] == '!' && line[i + 1] == '=') //Relational Operators
			{
				int z = 0;
				type = categoryType::RELATIONAL_OP;
				for (int x = i; x != line.size(); x++)
				{
					if (isspace(line[x]))
					{
						z = x;
						break;
					}
					else
					{
						token = token + line[x];
					}
				}
				i = z;
				lineTokens.push_back(make_pair(token, type));
			}
			else if (line[i] == '<' || line[i] == '>')
			{
				type = categoryType::RELATIONAL_OP;
				token = line[i];
				lineTokens.push_back(make_pair(token, type));
			}
			else if (line[i] == '=') //Assignment Operators
			{
				type = categoryType::ASSIGNMENT_OP;
				token = line[i];
				lineTokens.push_back(make_pair(token, type));
			}
			else if (line[i] == '+' || line[i] == '-' || line[i] == '*' || line[i] == '/' || line[i] == '%') //Arithmetic Operators
			{
				type = categoryType::ARITH_OP;
				token = line[i];
				lineTokens.push_back(make_pair(token, type));
			}
			//Comments
			else if (line[i] == '#') //looks for #
			{
				type = categoryType::COMMENT;
				while (i != line.size()) //looks at the whole as a comment
				{
					token = token + line[i];
					i++;
				}
				lineTokens.push_back(make_pair(token, type));
				break;
			}

			//More Punctations
			else if (line[i] == ',')
			{
				type = categoryType::COMMA;
				token = line[i];
				lineTokens.push_back(make_pair(token, type));
			}
			else if (line[i] == '(')
			{
				type = categoryType::LEFT_PAREN;
				token = line[i];
				lineTokens.push_back(make_pair(token, type));
			}
			else if (line[i] == ':')
			{
				type = categoryType::COLON;
				token = line[i];
				lineTokens.push_back(make_pair(token, type));
			}
			else if (line[i] == ')')
			{
				type = categoryType::RIGHT_PAREN;
				token = line[i];
				lineTokens.push_back(make_pair(token, type));
			}
		}

		//Numerical Literals
		else if (isdigit(line[i]))
		{
			type = categoryType::NUMERIC_LITERAL;
			token = token + line[i];

			if (isdigit(line[i + 1])) //looks ahead to see if there are any more digits
			{
				while (isdigit(line[i + 1]))
				{
					token = token + line[i + 1];
					i++;
				}
			}
			lineTokens.push_back(make_pair(token, type));
		}

		//Line Indent
		else if (isspace(line[i]) && i == 0) //if the whitespace is the first in the line, then it is an indent
		{
			type = categoryType::INDENT;
			token = token + line[i];
			if (isspace(line[i + 1])) //looks ahead to check for more whitespaces
			{
				while (isspace(line[i + 1]))
				{
					token = token + line[i + 1];
					i++;
				}
			}
			lineTokens.push_back(make_pair(token, type));
		}
		else if (isspace(line[i]) && i != 0)
		{
			continue; //continue will essentially ignore the whitespaces and move to the next character
		}
		else
		{
			type = categoryType::UNKNOWN;
			token = line[i];
			lineTokens.push_back(make_pair(token, type));
		}
		token.erase(); //make sure that the tokens don't clash with each other
	}
	return lineTokens;
}

void LexicalAnalyzer::showTokens()
{
	pairType pair; //will store the pair from lineInfoType
	tokenLineType tokenInfoLine; //stores the line for tokenInfo

	cout << "<-------------TOKEN INFORMATION------------->" << endl;
	for (int t = 0; t != tokenInfo.size(); t++)
	{
		tokenInfoLine = tokenInfo[t]; //stores line from tokenInfo to tokenInfoLine
		cout << "Line #" << t << ":" << endl;
		for (int i = 0; i != tokenInfoLine.size(); i++)
		{
			pair = tokenInfoLine[i]; //stores pair from tokenInfoLine to pair
			cout << "Token[" << i << "]:  ";
			cout << pair.first << " - "; //prints the pair.first

			switch (pair.second) //switch statement for pair.second since it is an integral type
			{
			case categoryType::ARITH_OP:
				cout << "ARITHMETIC OPERATOR" << endl;
				break;
			case categoryType::ASSIGNMENT_OP:
				cout << "ASSIGNMENT OPERATOR" << endl;
				break;
			case categoryType::COLON:
				cout << "COLON" << endl;
				break;
			case categoryType::COMMA:
				cout << "COMMA" << endl;
				break;
			case categoryType::COMMENT:
				cout << "COMMENT" << endl;
				break;
			case categoryType::IDENTIFIER:
				cout << "IDENTIFIER" << endl;
				break;
			case categoryType::INDENT:
				cout << "INDENT" << endl;
				break;
			case categoryType::KEYWORD:
				cout << "KEYWORD" << endl;
				break;
			case categoryType::LEFT_PAREN:
				cout << "LEFT PARENTHESIS" << endl;
				break;
			case categoryType::LOGICAL_OP:
				cout << "LOGICAL OPERATOR" << endl;
				break;
			case categoryType::NUMERIC_LITERAL:
				cout << "NUMERIC LITERAL" << endl;
				break;
			case categoryType::RELATIONAL_OP:
				cout << "RELATIONAL OPERATOR" << endl;
				break;
			case categoryType::RIGHT_PAREN:
				cout << "RIGHT PARENTHESIS" << endl;
				break;
			case categoryType::STRING_LITERAL:
				cout << "STRING LITERAL" << endl;
				break;
			case categoryType::UNKNOWN:
				cout << "UNKNOWN" << endl;
				break;
			}
		}
		cout << "-------------------------------------------------------------" << endl;
	}
}

void LexicalAnalyzer::tokenInfoFill(tokenType t)
{
	tokenInfo = t;
}

void LexicalAnalyzer::clearTokens()
{
	for (int x = 0; x != tokenInfo.size(); x++)
	{
		tokenInfo[x].clear(); //Clears every line of tokenInfo
	}
	tokenInfo.clear();
}

