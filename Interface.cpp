#include <iostream>
#include <fstream> //File Operations (Reading the Python file)
#include <vector>
#include <string>
#include <limits>
#include <stack>
#include "interface.h"
#include "lexanalyzer.h"
#include "expevaluator.h"
#include "interpreter.h"

using namespace std;

//(35 / 7 * 2) < (44 % 10 + 2) or (18 + 4 / 2) == (4 * 5)
//(7 + 3) <= 25 and ((4 * 5) == (14 + 3) or not(100 >= (40 * 2))) 

void Interface::startInterface() //main interface menu
{

	LexicalAnalyzer lexAnalysis;
	expEvaluator expEvaluation;
	Interpreter pystubi;

	cout << "Welcome to the PySub Interpreter 5.0 (December 2021)" << endl;
	cout << "Enter program lines or 'read(<filename>.py)' at command line interface" << endl;
	cout << "Type 'help' for more information or 'quit' to exit" << endl;

	bool loop = true; //loop checker


	while (loop)
	{
		string c = "";
		cout << ">>> ";
		getline(cin, c);
		if (c.substr(0, 4) == "read") //A parameter is passed, will access the 'read()' function
		{
			read(c);
			lexAnalysis.tokenInfo.clear(); //if there were previous tokens in there, this clears them for the new tokens
			tokenType tokens;
			for (int i = 0; i != programCode.size(); i++)
			{
				tokenLineType t;
				t = lexAnalysis.generate(programCode[i]); //immediately goes into generatinig tokens
				tokens.push_back(t); //pushes the lineTokens in tokens
			}
			lexAnalysis.tokenInfoFill(tokens);
		}
		else if (c.substr(0, 4) == "help") //whether or not a parameter is passed, will access the 'help()' function
		{
			help(c);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin.clear();
		}
		else if (c == "show" || c == "show()") //will access the 'show()' function
		{
			show();
		}
		else if (c == "show(tokens)")
		{
			lexAnalysis.showTokens(); //separate from the show()
		}
		else if (c == "show(variables)")
		{
			expEvaluation.showVariables();
		}
		else if (c[0] == 'i' && c[1] == 'f')
		{
			stack<string> ifLoop;
			string loop;

			bool b = false;
			for (auto x : c)
			{
				if (x == ':')
				{
					b = true;
				}
			}

			if (b == false)
			{
				cout << "Incorrect Syntax! Missing either conditional statement or colon (:) at the end of the statement" << endl;
			}
			else
			{
				ifLoop.push(c);
				do
				{
					cout << "...	";

					getline(cin, loop);
					ifLoop.push(loop);
				} while (!loop.empty());

				pystubi.interpretIf(ifLoop, expEvaluation);
			}
		}
		else if (c[0] == 'w' && c[1] == 'h' && c[2] == 'i' && c[3] == 'l' && c[4] == 'e')
		{
			stack<string> whileLoop;
			string wloop;

			bool b = false;
			for (auto x : c)
			{
				if (x == ':')
				{
					b = true;
				}
			}

			if (b == false)
			{
				cout << "Incorrect Syntax! Missing either conditional statement or colon (:) at the end of the statement" << endl;
			}
			else
			{
				whileLoop.push(c);
				do
				{
					cout << "...	";

					getline(cin, wloop);
					whileLoop.push(wloop);
				} while (!wloop.empty());

				pystubi.interpretWhile(whileLoop, expEvaluation);
			}
		}
		else if (c == "run" || c == "run()")
		{
			for (int i = 0; i != programCode.size(); i++)
			{
				pystubi.programCodeIndexInsert(i);
				pystubi.interpretLine(programCode[i], expEvaluation, programCode, i);
				int index = pystubi.programCodeIndexReturn();
				if (index > 0)
				{
					i = index;
				}
			}

		}
		else if (c == "quit" || c == "quit()") //will quit the interpreter interface
		{
			loop = quit();
		}
		else if (c == "clear" || c == "clear()") //will access the 'clear()' function
		{
			clear();
			lexAnalysis.clearTokens();
			expEvaluation.vectorClear();
			expEvaluation.clearSymbolTable();
		}
		else
		{

			int v = expEvaluation.tokenizeLine(c);

			if (v == -1)
			{
				cin.clear();
			}
			else if (v == 5)
			{
				pystubi.interpretLine(c, expEvaluation, programCode, 0);
			}
			else
			{
				expEvaluation.choice(v, c);
			}
		}

		c.erase();
		cout << endl;
	}
}

void Interface::read(string pythonFile) //stores the program from the Python file into the vector
{
	if (pythonFile == "read()" || pythonFile == "read")
	{
		cout << "Error! There was no file in the 'read' command" << endl;
		cout << "Try again and type read(<filename>.py)" << endl;
		return; //exits out of the function if pythonFile equals 'read()' or 'read'
	}


	clear(); //if there is code from the previous file still in there, this clears it out

	//since the file is passed as a parameter, this pulls the file name out of the read(<filename>.py)
	string par = pythonFile.substr(0, pythonFile.size() - 1);
	string pythonFileName = par.substr(5, par.size());

	//pulls the .py file extension
	string py = pythonFile.substr(pythonFile.length() - 4, 3); //subtring(first position, string length)

	if (py != ".py") //checks if there was a .py file extension
	{
		while (py != ".py") //While there is no .py extension file
		{
			cout << "Error: Python file not found! Try again, type read(<filename>.py), and make sure you include .py file type" << endl;
			cout << ">>> ";
			getline(cin, pythonFile);

			par = pythonFile.substr(0, pythonFile.size() - 1); //pulls the file name out of the read(<filename>.py)
			pythonFileName = par.substr(5, par.size());

			py = pythonFileName.substr(pythonFileName.size() - 3, 3); //retrieves the '.py' extension for the loop check
		}
	}


	ifstream fin;
	fin.open(pythonFileName); //Opens the file
	cout << endl << "Looking for " << pythonFileName << endl;

	if (!fin) //Gives error message if the file does not open
	{
		cout << "Error opening file! Either the file name is incorrect or the file you requested is not appearing" << endl;

	}
	else
	{
		cout << "File found!!" << endl;
	}

	string line;

	while (fin)
	{
		getline(fin, line);
		programCode.push_back(line); //using push_back to put the lines into the vector
	}

}

void Interface::help(string command) //brings up the help utility
{
	if (command == "help" || command == "help()") //if no parameter was given
	{
		string commandHelp;
		bool check = true;
		cout << "Welcome to the help utility!" << endl;
		cout << "* To exit and return to the interpreter, type 'exit'" << endl;
		cout << "* To get a list of commands, type 'commands'" << endl;
		cout << "* To get a description of any command, just type the command at the help> prompt" << endl;
		while (check)
		{
			cout << "help> ";
			cin >> commandHelp;

			if (commandHelp == "commands")
			{
				cout << endl;
				cout << "Below is a list of commands. Enter any command at the prompt to get more help" << endl;
				cout << "*clear\t\t\t*help" << endl;
				cout << "*read\t\t\t*show" << endl;
				cout << "*quit" << endl << endl;
				cout << "NOTE: All commands can be also entered as functions" << endl;
				cout << "*clear()\t\t\t*help()" << endl;
				cout << "*read()\t\t\t\t*show()" << endl;
				cout << "*show(tokens)\t\t\t*quit()" << endl << endl;

			}

			if (commandHelp == "read" || commandHelp == "read()")
			{
				cout << "The 'read' command reads program lines from the Python file and stores the lines into the program data structure" << endl;
			}
			else if (commandHelp == "show" || commandHelp == "show()")
			{
				cout << "The 'show' command shows the lines of the program that are stored in the program data structure" << endl;
			}
			else if (commandHelp == "clear" || commandHelp == "clear()")
			{
				cout << "The 'clear' command clears out or deletes any lines that are stored in the program data structure" << endl;
			}
			else if (commandHelp == "quit" || commandHelp == "quit()")
			{
				cout << "The 'quit' command exits the command line interpreter" << endl;
			}
			else if (commandHelp == "help" || commandHelp == "help()")
			{
				cout << "The 'help' command opens the help utility and provides more information of the commands and functions" << endl;
			}
			else if (commandHelp == "show(tokens)")
			{
				cout << "The 'show(tokens)' command displays the tokens of the programCode and its respective category type" << endl;
			}
			else if (commandHelp == "exit")
			{
				cin.clear();
				check = false;
			}
		}
	}
	else if (command.substr(0, 4) == "help") //if parameter was given in the help() command
	{
		string p = command.substr(0, command.size() - 1); //Pulls the parameter out from the full string
		string commandName = p.substr(5, p.size());

		if (commandName == "commands")
		{
			cout << endl;
			cout << "Below is a list of commands. Enter any command at the prompt to get more help" << endl;
			cout << "*clear\t\t\t*help" << endl;
			cout << "*read\t\t\t*show" << endl;
			cout << "*quit" << endl << endl;
			cout << "NOTE: All commands can be also entered as functions" << endl;
			cout << "*clear()\t\t\t*help()" << endl;
			cout << "*read()\t\t\t\t*show()" << endl;
			cout << "*show(tokens)\t\t\t*quit()" << endl << endl;

		}

		if (commandName == "read" || commandName == "read()")
		{
			cout << "The 'read' command reads program lines from the Python file and stores the lines into the program data structure" << endl;
		}
		else if (commandName == "show" || commandName == "show()")
		{
			cout << "The 'show' command shows the lines of the program that are stored in the program data structure" << endl;
		}
		else if (commandName == "clear" || commandName == "clear()")
		{
			cout << "The 'clear' command clears out or deletes any lines that are stored in the program data structure" << endl;
		}
		else if (commandName == "quit" || commandName == "quit()")
		{
			cout << "The 'quit' command exits the command line interpreter" << endl;
		}
		else if (commandName == "show(tokens)")
		{
			cout << "The 'show(tokens)' command displays the tokens of the programCode and its respective category type" << endl;
		}
		else if (commandName == "help" || commandName == "help()")
		{
			cout << "The 'help' command opens the help utility and provides more information of the commands and functions" << endl;
		}
	}
}

void Interface::show() //prints the program from the vector
{
	for (int x = 0; x != programCode.size(); x++)
	{
		cout << "[" << x << "]  " << programCode[x] << endl;
	}
}

void Interface::clear() //clears the program from the vector
{
	programCode.clear();
}

bool Interface::quit()  //exits the interpreter interface
{
	cout << endl << "Thank you for using the PySub Interpreter 5.0!!" << endl;
	return false; //returns false, thus ending the loop and the interpreter interface
}