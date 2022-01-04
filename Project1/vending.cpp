// soda.cpp - Antonio Mastroianni
// This program emulates the functionality of a vending machine in order to give insight into how an operating system's command line interpreter works

//NOTE: The default password is password

#include "vending.h"

//*************
//Main Function
//*************
int main() {
	//creates a string to hold the user's input
	std::string userInput; 

	//outputs the next string in a red color text
	const HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, 4); 
	std::cout << "Please Enter a command and its parameter(s)\n\n\t(type HELP for list of commands, EXIT to quit)\n";
	//sets the next string to output in a white color text
	SetConsoleTextAttribute(h, 15);

	//creates the "vending machine" object
	VendingMachine mainMachine;

	//continually gets the user's input and operates according to it until the vending machine has an exit status
	while (mainMachine.getProgramRunningStatus()) {

		if (mainMachine.getModeStatus() == true)
			{ std::cout << "\n[SERVICE MODE] >"; }
		else
			{ std::cout << "\n[NORMAL MODE] >"; }

		//switches font color to red to denote the user's input
		SetConsoleTextAttribute(h, 4); 
		std::getline(std::cin, userInput);

		//switches font color to white to denote the console's output
		SetConsoleTextAttribute(h, 15);
		std::cout << '\n';

		//the "vending machine" object handles all operations with the user's input
		mainMachine.parseCommand(userInput);
	}

	return 0;
}