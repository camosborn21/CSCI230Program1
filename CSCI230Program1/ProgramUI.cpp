#include "stdafx.h"
#include "ProgramUI.h"
#include <iostream>
#include <fstream>>
#include <vector>
#include <string>
#include <cstring>
#include <conio.h>
using namespace std;

void ProgramUI::loadProgramFromFile()
{
	//[9/13/2017 17:36] Cameron Osborn: Get a valid filename
	char fileName[MaxLengthFileName];
	cout << "Name of file to upload program from (ex file.txt): ";
	cin >> fileName;

	//[9/15/2017 23:09] Cameron Osborn: Repeat line in until a filename is entered.
	while (strlen(fileName) == 0)
	{
		cin >> fileName;
	}

	//[9/13/2017 22:58] Cameron Osborn: flush the input buffer to fix first loop skipping glitch. Discovered code on Stackoverflow (https://stackoverflow.com/questions/10553597/cin-and-getline-skipping-input). Reused throughout every time cin appears outside of getline function.
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	//[9/13/2017 17:36] Cameron Osborn: Attempt to load file
	ifstream inFile;
	inFile.open(fileName);
	if (inFile.fail())
	{
		cout << endl << "Cannot open this file!" << endl;
		return;
	}

	//[9/13/2017 17:41] Cameron Osborn: consider extract method if a linesBuffer vector is added.
	lines.clear();

	//[9/14/2017 22:26] Cameron Osborn: Assign current file
	currentFile = fileName;
	fileLoaded = true;

	//[9/13/2017 17:31] Cameron Osborn: If the file is loaded successfully then push all non-blank lines to the lines vector
	string inputBuffer;
	while (!inFile.eof())
	{
		//[9/13/2017 17:30] Cameron Osborn: Load next line to input buffer
		getline(inFile, inputBuffer);

		//[9/13/2017 17:20] Cameron Osborn: Skips blank lines if not end of field
		while (!inFile.eof() && inputBuffer.size() == 0)
		{
			getline(inFile, inputBuffer);
		}

		//[9/13/2017 17:28] Cameron Osborn: push lines to buffer (original template has a print lines block here, too but I refactored that to the displayProgram routine.)
		if (!inFile.eof())
		{
			lines.push_back(inputBuffer);
		}
	}
	//[9/13/2017 18:58] Cameron Osborn: On program load, display program.
	displayProgram();
	inFile.close();
}


void ProgramUI::displayProgram()
{
	//[9/13/2017 18:54] Cameron Osborn: Line number counter.
	int lineCounter = 1;

	//[9/13/2017 21:12] Cameron Osborn: iterate through all lines and display them in console
	for (vector<string>::iterator iterator = lines.begin(); iterator < lines.end(); ++iterator, ++lineCounter)
	{
		cout << "Line["
			<< lineCounter
			<< "] read:"
			<< *iterator << endl;
	}
}

void ProgramUI::displayChanges()
{
	//[9/15/2017 23:06] Cameron Osborn: Line number counter.
	int lineCounter = 1;

	//[9/15/2017 23:06] Cameron Osborn: Iterate through all lines and display them in console
	for (vector<string>::iterator iterator = changeBuffer.begin(); iterator < changeBuffer.end(); ++iterator, ++lineCounter)
	{
		cout << "Line["
			<< lineCounter
			<< "] read:"
			<< *iterator << endl;
	}
}

void ProgramUI::processChanges(string changeMessage)
{
	//[9/13/2017 21:47] Cameron Osborn: Review Changes.
	cout << changeMessage << endl << HorizontalRule << endl << "Changes read as follows:" << endl;
	displayChanges();

	//[9/13/2017 23:11] Cameron Osborn: User input: Accept or reject changes
	cout << endl << HorizontalRule << endl << "Press 'Y' to accept these changes or 'N' to roll back" << endl << "Your choice is: ";
	char keyPressed = _getch();
	cout << keyPressed;

	//[9/13/2017 23:11] Cameron Osborn: proceed based on user decision for accept/reject changes.
	switch (keyPressed)
	{
	case 'Y': case 'y': //[9/13/2017 23:11] Cameron Osborn: changes accepted. Update lines vector to match changeBuffer vector
		lines = changeBuffer;
		cout << endl << "Changes accepted. " << changeMessage;
		break;

	default: //[9/13/2017 23:12] Cameron Osborn: Changes weren't accepted. Offer the user the option to reprint/review the most recent version of the program before the rejected changes were implemented. That way if they're looking up for ongoing inline edits they don't have to think around the changes they've just rejected.
		cout << endl << "Changes rejected. Program rolled back to previous version." << endl << endl << HorizontalRule << endl << "Would you like to reprint the current program? (Press Y to reprint the program without changes)" << endl << "Your choice is: ";
		char secondKey = _getch();
		cout << secondKey;
		switch (secondKey)
		{
		case 'Y': case 'y':
			cout << endl;
			displayProgram();
			cout << endl << HorizontalRule;
			break;
		default:
			break;
		}
	}
}


void ProgramUI::appendLines()
{

	//[9/13/2017 21:34] Cameron Osborn: Display action
	cout << "Keep reading and appending lines of statements you enter to the end of the program until a single period is entered as a line." << endl;

	//[9/13/2017 21:44] Cameron Osborn: load the change buffer with the existing statements.
	changeBuffer = lines;

	//[9/13/2017 21:45] Cameron Osborn: register new statements to be appended (USER INPUT) and count number of added lines.
	string s;
	int addedLines = 0;
	while (s != ".")
	{
		cout << "[" << changeBuffer.size() + 1 << "]: ";
		getline(cin, s);
		if (s != "." && s != "")
		{
			++addedLines;
			changeBuffer.push_back(s);
		}
	}

	//[9/13/2017 21:45] Cameron Osborn: signal process cancellation if no new lines were added | Else format the user notification for change review.
	string msg;
	bool offerAcceptChanges = true;
	switch (addedLines)
	{
	case 0: //[9/13/2017 23:10] Cameron Osborn: UI isn't dirty; cancel process
		cout << "No lines added. Append process cancelled.";
		offerAcceptChanges = false;
		break;

	case 1:
		msg = "1 line appended to the end of the program.";
		break;

	default:
		msg = std::to_string(addedLines) + " lines appended to the end of the program.";
	}

	//[9/13/2017 23:09] Cameron Osborn: Call accept changes routine if the UI is dirty.
	if (offerAcceptChanges)
	{
		processChanges(msg);
	}
}

//[9/14/2017 01:08] Cameron Osborn: determine if an input line number is within the range of the application.
//[9/14/2017 01:14] Cameron Osborn: added boolean check for include zero to account for insertion after "Line 0" a.k.a. as line 1. Otherwise line checks should be bound 1 to changebuffer.size
bool ProgramUI::isLineNumberValid(int checkNumber, bool includeZero)
{
	if (includeZero == true) {
		if (checkNumber < 0 || checkNumber > changeBuffer.size())
		{
			return false;
		}
		return true;
	}
	if (checkNumber < 1 || checkNumber> changeBuffer.size())
	{
		return false;
	}
	return true;
}

void ProgramUI::insertLines()
{
	int lineNumber;
	changeBuffer = lines;
	//[9/14/2017 00:16] Cameron Osborn: Get Line Number (if using cin clear buffer)
	cout << "After which line (tell us the line #) do you want to insert a new line? ";
	cin >> lineNumber;

	//[9/14/2017 01:37] Cameron Osborn: verify that the input from cin was numeric
	if (!cin)
	{
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	//[9/14/2017 00:16] Cameron Osborn: Validate Line Number
	while (!isLineNumberValid(lineNumber, true))
	{
		cout << "Invalid line number. Please enter a number between 0 and " << changeBuffer.size() << endl << "After which line do you want to insert a new line? ";
		cin >> lineNumber;
		if (!cin)
		{
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}

	//[9/15/2017 22:42] Cameron Osborn: Clear cin Buffer
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	int lineTracker = lineNumber;
	//[9/13/2017 23:31] Cameron Osborn: Instructions to user
	cout << endl << "We'll keep reading and inserting lines of statements you enter until a single period is entered as a line." << endl;

	//[9/14/2017 00:29] Cameron Osborn: Register new statements to be inserted. After each valid insert, increment line number, increment added lines, and push new line to change buffer.
	++lineTracker;
	int addedLines = 0;
	string s;
	while (s != ".")
	{
		cout << "[" << lineTracker << "]: ";
		getline(cin, s);
		if (s != "." && s != "")
		{
			vector<string>::iterator itIns = changeBuffer.begin() + (lineTracker - 1);
			changeBuffer.insert(itIns, s);
			++addedLines;
			++lineTracker;
		}
	}

	//[9/14/2017 00:32] Cameron Osborn: Signal process cancellation if no new lines were inserted | Else format the user notification for change review
	string msg;
	bool offerAcceptChanges = true;
	switch (addedLines)
	{
	case 0: //[9/14/2017 00:34] Cameron Osborn: UI isn't dirty; cancel process
		cout << "No lines inserted. Insert process cancelled.";
		offerAcceptChanges = false;
		break;

	case 1:
		msg = "1 line inserted after line " + std::to_string(lineNumber) + ".";
		break;

	default:
		msg = std::to_string(addedLines) + " lines inserted after line " + std::to_string(lineNumber) + ".";
	}

	//[9/14/2017 00:49] Cameron Osborn: Call accept changes routine if the UI is dirty.
	if (offerAcceptChanges)
	{
		processChanges(msg);
	}
}

void ProgramUI::deleteLines()
{
	changeBuffer = lines;

	//[9/14/2017 01:22] Cameron Osborn: User Instruction
	cout << "What lines do you want to delete?";
	int firstLine;
	int lastLine;

	//[9/15/2017 22:13] Cameron Osborn: Get first line
	cout << endl << "From line #: ";
	cin >> firstLine;

	//[9/14/2017 01:37] Cameron Osborn: verify that the input from cin was numeric
	//[9/15/2017 22:41] Cameron Osborn: Consider Refactoring to "ValidateIsNumeric()"
	if (!cin)
	{
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	//[9/15/2017 22:13] Cameron Osborn: Validate line
	while (!isLineNumberValid(firstLine, false))
	{
		cout << endl << "Invalid line number. Please enter a number between 1 and " << changeBuffer.size() << endl << "Start deletion with line #: ";
		cin >> firstLine;
		//[9/14/2017 01:37] Cameron Osborn: verify that the input from cin was numeric
		if (!cin)
		{
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}

	//[9/15/2017 22:14] Cameron Osborn: Get Last line
	cout << "To line #: ";
	cin >> lastLine;

	//[9/14/2017 01:37] Cameron Osborn: verify that the input from cin was numeric
	if (!cin)
	{
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	//[9/16/2017 21:31] Cameron Osborn: validate line
	while (!isLineNumberValid(lastLine, false) || lastLine < firstLine)
	{
		cout << "Invalid line number. Please enter a number between" << firstLine << " and " << changeBuffer.size() << endl << "Finish deletion with line #: ";
		cin >> lastLine;
		//[9/14/2017 01:37] Cameron Osborn: verify that the input from cin was numeric
		if (!cin)
		{
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}

	//Clear cin buffer
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	//[9/14/2017 03:05] Cameron Osborn: Propegate change in buffer.
	changeBuffer.erase(changeBuffer.begin() + firstLine - 1, changeBuffer.begin() + lastLine);


	//generate change message
	string msg;
	if (lastLine == firstLine)
	{
		msg = "Line " + std::to_string(firstLine) + " deleted.";
	}
	else
	{
		msg = "Lines " + std::to_string(firstLine) + " to " + std::to_string(lastLine) + " deleted.";
	}

	//call accept changes
	processChanges(msg);


}

void ProgramUI::replaceOneLine()
{
	changeBuffer = lines;
	//[9/14/2017 11:46] Cameron Osborn: Get Line
	int changeLine;
	cout << endl << "What line # would you like to replace? ";

	cin >> changeLine;
	if (!cin)
	{
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	//[9/14/2017 11:46] Cameron Osborn: Validate line number
	while (!isLineNumberValid(changeLine, false))
	{
		cout << endl << "Invalid line number. Please enter a number between 1 and " << changeBuffer.size() << endl << "What line # would you like to replace? ";
		cin >> changeLine;
		if (!cin)
		{
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}

	//[9/14/2017 12:56] Cameron Osborn: Clear cin buffer
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	//[9/14/2017 11:47] Cameron Osborn: Get new line of code
	string formerLine = changeBuffer[changeLine];
	cout << endl << HorizontalRule << "You've selected line " << changeLine << ". It currently reads: " << endl;
	cout << "[" << changeLine << "]: " << formerLine << endl << endl << "What would you like to change this line to say? (Enter a single period to cancel)";

	bool offerAcceptChanges = true;
	string s;
	while (s != "") {
		cout << endl << "[" << changeLine << "]: ";
		getline(cin, s);
		if (s != ".")
		{
			changeBuffer[changeLine] = s;
		}
		else {
			cout << "No lines changed. Replace process cancelled.";
			offerAcceptChanges = false;
		}
	}

	if (offerAcceptChanges) {
		//[9/14/2017 12:03] Cameron Osborn: Generate change message	
		string msg = "Line " + std::to_string(changeLine) + " has been changed. It used to read: \n[" + std::to_string(changeLine) + "]: " + formerLine + "\n\nIt now reads: \n[" + std::to_string(changeLine) + "]: " + s;

		//[9/14/2017 12:05] Cameron Osborn: Call Accept Changes
		processChanges(msg);
	}
}

void ProgramUI::saveProgramIntoFile()
{
	//[9/16/2017 21:32] Cameron Osborn: If lines vector has no content then a new file cannot be saved.
	if (!checkStatementsInBuffer())
	{
		return;
	}

	//[9/16/2017 21:26] Cameron Osborn: Get a file name. Add future validation
	char fileName[MaxLengthFileName];
	cout << "Save as File Name: ";
	cin >> fileName;
	while(strlen(fileName) == 0)
	{
		cin >> fileName;
	}

	//[9/16/2017 21:37] Cameron Osborn: Create, or open, output file
	ofstream outFile;
	outFile.open(fileName);
	if (outFile.fail())
	{
		cout << endl << "Cannot create this file!" << endl;
		return;
	}

	//[9/16/2017 21:39] Cameron Osborn: Write all lines to output file.
	for (vector<string>::iterator writer = lines.begin(); writer < lines.end(); ++writer)
	{
		outFile << *writer << endl;
	}
	outFile.close();
}


void ProgramUI::quickSave()
{
	//[9/16/2017 21:40] Cameron Osborn: If a file isn't currently loaded then nothing can be quick saved
	if (!checkFileLoaded())
	{
		return;
	}

	//[9/16/2017 21:40] Cameron Osborn: Get output file open for writing
	ofstream outFile;
	outFile.open(currentFile);
	if (outFile.fail())
	{
		cout << endl << "Cannot open this file to save!" << endl;
		return;
	}

	//[9/16/2017 21:44] Cameron Osborn: Clear previous file content and write statements from lines vector to file.
	outFile.clear();
	for(vector<string>::iterator writer = lines.begin(); writer < lines.end(); ++writer)
	{
		outFile << *writer << endl;
	}
	outFile.close();
}


void ProgramUI::endOfService(const string service)
{
	//[9/16/2017 21:45] Cameron Osborn: Writes an output message for each action completion.
	cout << endl << service << " done." << endl << "Press any key to continue.";
	_getch();
	cout << endl
		<< HorizontalRule
		<< endl
		<< endl;
}

bool ProgramUI::checkStatementsInBuffer()
{
	//[9/16/2017 23:29] Cameron Osborn: If there are statements in the primary buffer this check returns true; otherwise false with output to user.
	if(lines.size() > 0)
	{
		return true;
	}
	cout << "There are no statements in the program. To perform this action the program must have at least one statement.";
	return false;
}


bool ProgramUI::checkFileLoaded()
{
	//[9/16/2017 23:33] Cameron Osborn: If there is a file loaded then this check returns true; otherwise false with output to user.
	if (!fileLoaded)
	{
		cout << "A file must be loaded to perform this action. No file is currently loaded. Load a file then attempt this action again.";
		return false;
	}
	return true;
}


//[9/13/2017 16:09] Cameron Osborn: This menu() function is the primary user interface
void ProgramUI::startInterface()
{
	bool inMenu = true; //[9/13/2017 16:12] Cameron Osborn: program exit boolean. When false on loop repeat the program will exit.
	char keyPressed; //[9/13/2017 16:13] Cameron Osborn: Stores which menu selection the user picks.

	while (inMenu)
	{
		cout << endl << endl;
		cout << HorizontalRule << endl;
		cout << "**  MENU:(press a character to select an option)  **" << endl;
		cout << HorizontalRule << endl;
		cout << "Q. [QUIT]         Quit." << endl;
		cout << "L. [LOAD]         Read in a program (lines of statements) from a file" << endl;
		cout << "S. [SAVE]         Save lines of statement in original file." << endl;
		cout << "N. [SAVE AS NEW]  Save lines of statement as a new file." << endl;
		cout << endl;
		cout << "D. [DISPLAY]      Display the source code as lines of statements" << endl;
		cout << endl;
		cout << "A. [APPEND]       Append new lines to the end of the program" << endl;
		cout << "I. [INSERT]       Insert new lines before an existing line" << endl;
		cout << "X. [DELETE]       Delete a range of existing lines" << endl;
		cout << "R. [REPLACE]      Replace the contents of an existing line" << endl;
		cout << endl;
		cout << "P. [PARSE]        Parse and indent the code" << endl;
		cout << "E. [EXECUTE]      Execute (run) the program" << endl;
		cout << "T. [TOGGLE]       Toggle the execution debug mode" << endl;
		cout << HorizontalRule << endl << endl;
		cout << "Your choice is: ";

		keyPressed = _getch();
		cout << keyPressed << endl;

		switch (keyPressed)
		{
		case 'Q': case 'q':
			cout << "[QUIT]:" << endl;
			inMenu = false;
			break;

		case 'L': case 'l':
			cout << "LOAD PROGRAM:" << endl;
			loadProgramFromFile();
			endOfService("[LOAD PROGRAM]");
			break;

		case 'D': case 'd':
			cout << "[DISPLAY PROGRAM]:" << endl;
			displayProgram();
			endOfService("[DISPLAY PROGRAM]");
			break;

		case 'A': case 'a':
			cout << "[APPEND]:" << endl;
			appendLines();
			endOfService("[APPEND LINES]");
			break;

		case 'I': case 'i':
			cout << "[INSERT LINES]:" << endl;
			insertLines();
			endOfService("[INSERT LINES]");
			break;

		case 'X': case 'x':
			cout << "[DELETE LINES]:" << endl;
			deleteLines();
			endOfService("[DELETE LINES]");
			break;

		case 'R': case 'r':
			cout << "[REPLACE]:" << endl;
			replaceOneLine();
			endOfService("[REPLACE ONE LINE]");
			break;

		case 'S': case 's':
			cout << "[SAVE]:" << endl;
			quickSave();
			endOfService("[SAVE CODE INTO ORIGINAL FILE]");
			break;

		case 'N': case 'n':
			cout << "[SAVE AS NEW]:" << endl;
			saveProgramIntoFile();
			endOfService("[SAVE CODE INTO A NEW FILE]");
			break;

		case 'P': case 'p':
			cout << "[PARSE AND INDENT SOURCE CODE]:" << endl;
			endOfService("[PARSE AND INDENT SOURCE CODE]");
			break;

		case 'E': case 'e':
			cout << "[EXECUTE TO RUN PROGRAM]:" << endl;
			endOfService("[EXECUTE TO RUN PROGRAM]");
			break;

		case 'T': case 't':
			cout << "[TOGGLE the debug mode]:" << endl;
			break;

		default:
			cout << "[?? UNKNOWN COMMAND]:" << keyPressed << endl;
		} //[9/13/2017 16:48] Cameron Osborn: end of switch


	} //[9/13/2017 16:48] Cameron Osborn: end of while loop
}