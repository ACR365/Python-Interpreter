#ifndef INTERFACE_H //Checks to see if the header is not already defined
#define INTERFACE_H //Making sure not to confuse the compiler
#include <vector>
#include <string>

using namespace std;

//read() => stores the program into the vector
//show() => prints the program

typedef vector<string> programType;

class Interface
{
	public:
	void startInterface(); //starts the interface

	void read(string); //stores the program from the Python file into the vector

	void show(); //prints the program from the vector

	void clear(); //clears the program from the vector

	bool quit(); //exits the interpreter interface

	void help(string); //brings up the help utility

private:
	programType programCode;
	friend class LexAnalyzer;

};
#endif // !INTERFACE_H

