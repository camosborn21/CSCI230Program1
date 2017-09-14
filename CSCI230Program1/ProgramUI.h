//#pragma once

#ifndef PROGRAMUI_H

#define PROGRAMUI_H

#include <vector>  //[9/13/2017 14:07] Cameron Osborn: include support of vector container
#include <string> //[9/13/2017 14:07] Cameron Osborn: include support of new C++ string class

using namespace std;

class ProgramUI
{
private:
	vector<string> lines;
	vector<string> changeBuffer;
	string HorizontalRule = "****************************************************";
public:
	//[9/13/2017 14:07] Cameron Osborn: used to set the maximum length of file names. 
	// Template dictated 100, consider increasing to allow fully qualified file path.
	static const unsigned int MaxLengthFileName = 100;
	
	
	//[9/13/2017 14:56] Cameron Osborn: not vary familar with C++ header files yet but this seems to be an interface definition similar to C#. The following lines were replicated from the template file. The class name of Interface was exchanged for ProgramUI because I'm too used to interface being a reserved word in other languages.

	//[9/13/2017 14:49] Cameron Osborn: Read in a program (lines of statements) from a file
	void loadProgramFromFile();

	//[9/13/2017 14:59] Cameron Osborn: display the lines of statements
	void displayProgram();

	//[9/13/2017 21:27] Cameron Osborn: display the lines in the change buffer for user approval
	void displayChanges();

	//[9/13/2017 22:12] Cameron Osborn: Present the changes to the user with an option to accept or rollback to the pre-change program.
	void processChanges(string changeMessage);

	//[9/13/2017 14:59] Cameron Osborn: add new lines onto the end of the code
	void appendLines();

	//[9/13/2017 23:52] Cameron Osborn: validate line number input
	bool isLineNumberValid(int checkNumber, bool includeZero);

	//[9/13/2017 14:59] Cameron Osborn: insert new lines after and existing line
	void insertLines();

	//[9/13/2017 14:59] Cameron Osborn: delete existing lines
	void deleteLines();

	//[9/13/2017 15:00] Cameron Osborn: replace the contents of an existing line
	void replaceOneLine();

	//[9/13/2017 15:03] Cameron Osborn: save program to a file
	void saveProgramIntoFile();

	//[9/13/2017 15:04] Cameron Osborn: This function serves as the central place to access all the services provided by the interpreter including the functions listed above
	void startInterface();

	//[9/13/2017 15:08] Cameron Osborn: This function displays function output messages to the user
	void endOfService(const string service);
	

	//ProgramUI();
	//~ProgramUI();
};

#endif
