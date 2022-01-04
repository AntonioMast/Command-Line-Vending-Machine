// vending.h - Antonio Mastroianni
// This program emulates the functionality of a vending machine in order to give insight into how an operating system's command line interpreter works

#ifndef SODA_H
#define SODA_H

#define NOMINMAX
#include <iostream>
#include <string>
#include <windows.h>
#include <iomanip>
#include <sstream>

//creates a VendingMachine class to allow easy communication between functions relating to the object.
class VendingMachine
{
private:

	//private variables
	std::string password = "password";
	const int PRICE = 145;
	const static int AMOUNTOFCOINS = 3;
	const static int AMOUNTOFBILLS = 2;
	const static int SIZEOFCOLATYPES = 5;
	int coins[AMOUNTOFCOINS] = { 0,0,0 };
	int bills[AMOUNTOFBILLS] = { 0,0 };
	int cola[SIZEOFCOLATYPES] = { 0,0,0,0,0 };
	std::string colaTypes[SIZEOFCOLATYPES] = { "COKE", "PEPSI", "SPRITE", "FANTA", "FAYGO" };
	int cups = 0;
	bool runProgram = true;
	bool serviceMode = true;
	int coinsBuffer[AMOUNTOFCOINS] = { 0,0,0 }; //holds the amount of coins deposited into the buffer
	int billsBuffer[AMOUNTOFBILLS] = { 0,0 }; //holds the amount of bills deposited into the buffer

public:

	//function prototypes
	void coinCmd(std::string);
	void billCmd(std::string);
	void colaCmd(std::string, std::string);
	void returnCmd();
	void statusCmd();
	void unlockCmd(std::string);
	void lockCmd(std::string);
	void helpCmd();
	void dispenseChange();
	void dispenseCola(int, std::string, std::string);
	void adjustItemCountCmd(std::string, std::string, std::string, std::string);
	int convertBufferToValue();
	int convertStringCoinToInt(std::string);
	int convertStringBillToInt(std::string);
	int convertStringColaToInt(std::string);
	bool checkIfChangeAvailable(double);
	bool checkForValidCola(std::string);
	bool checkForValidChange(std::string s);
	bool checkForValidBill(std::string s);
	bool checkForAvailableCola(std::string);
	int convertStringToNumber(std::string);
	void addRemoveDenomination(std::string, std::string, std::string, std::string);
	void parseCommand(std::string);
	void parseOption(std::string, std::string);
	void parseSwitch(std::string, std::string, std::string);
	void goToCmd(std::string, std::string, std::string, std::string);
	bool getProgramRunningStatus();
	bool getModeStatus();
	std::string toUpper(std::string);
};


//************************
//Handles the coin command
//************************
void VendingMachine::coinCmd(std::string option)
{
	//gets the index of the coins array that corresponds to the string given
	int index = convertStringCoinToInt(option);

	//deposits the coin into the buffer if it is a valid denomination
	if (index >= 0 && index < AMOUNTOFCOINS) {
		coinsBuffer[index] += 1;
		std::cout << "Added 1 " << option << "-denomination COIN!\n";
	}

	//outputs an error if the coin is not a valid denomination
	else {
		std::cout << option << " is not a valid parameter for the COIN command" << '\n';
	}
}

//************************
//Handles the bill command
//************************
void VendingMachine::billCmd(std::string option)
{
	//gets the index of the bills array that corresponds to the string given
	int index = convertStringBillToInt(option);

	//deposits the bill into the buffer if it is a valid denomination
	if (index >= 0 && index < AMOUNTOFBILLS) {
		billsBuffer[index] += 1;
		std::cout << "Added 1 " << option << "-denomination BILL!\n";
	}

	//outputs an error if the bill is not a valid denomination
	else {
		std::cout << option << " is not a valid parameter for the BILL command" << '\n';
	}
}

//************************
//Handles the cola command
//************************
void VendingMachine::colaCmd(std::string option, std::string switchParameter)
{
	//used to calculate whether or not enough money was inserted
	int costOfCola = PRICE;
	int value = convertBufferToValue();


	///////////////////////////////////////////////////////////////////////////////
	//the following statements check if all requirements are met to dispense cola//
	///////////////////////////////////////////////////////////////////////////////

	//checks if a valid cola was entered
	if (checkForValidCola(option)) {

		//checks if enough money was inserted
		if (value >= costOfCola) {

			//checks if a cup is available
			if (cups > 0) {

				//checks if change can be made
				if (checkIfChangeAvailable(value)) {

					//checks if the cola requested is available
					if (checkForAvailableCola(option)) {

						//dispenses the cola if all requirements are met
						dispenseCola(convertStringColaToInt(option), option, switchParameter);
					}

	//////////////////////////////////////////////////////////////////////////////
	//displays an appropriate error message if any of the previous checks failed//
	//////////////////////////////////////////////////////////////////////////////

					//displays that the requested cola is not available
					else {
						std::cout << option << " cola is not avaiable" << '\n';
					}
				}

				//displays that the machine is unable to make enough change
				else {
					std::cout << "Insufficient change avaiable! Returning your money...\n";
					returnCmd();
				}
			}

			//displays that there are not enough cups available
			else {
				std::cout << "Insufficient cups avaiable! Returning your money...\n";
				returnCmd();
			}
		}

		//displays that not enough money was entered and shows the current amount entered
		else {
			std::cout << "Insufficient funds! Enter more money\n";
			double fullAmount = (coinsBuffer[0] * 0.05) + (coinsBuffer[1] * 0.10) + (coinsBuffer[2] * 0.25) + (billsBuffer[0] * 1) + (billsBuffer[1] * 5);
			const int width = 30;

			std::cout << std::setprecision(2) << std::fixed;
			std::cout << std::setw(width - 1) << std::left << "  Amount Deposited" << "$" << fullAmount << '\n';
		}
	}

	//displays that the requested cola is not valid
	else {
		std::cout << option << " is not a valid cola type" << '\n';
	}
}

//**************************
//Handles the return command
//**************************
void VendingMachine::returnCmd()
{

	bool returnedMoney = false;

	//"returns" the held coins
	for (int i = 0; i < AMOUNTOFCOINS; i++) {
		if (coinsBuffer[i] > 0) { returnedMoney = true; }
		coinsBuffer[i] = 0;
	}

	//"returns" the held bills
	for (int i = 0; i < AMOUNTOFBILLS; i++) {
		if (billsBuffer[i] > 0) { returnedMoney = true; }
		billsBuffer[i] = 0;
	}
	
	//displays whether or not any money was returned
	if (returnedMoney) 
		{ std::cout << "Returned all money\n"; }
	else 
		{ std::cout << "No money to return!\n"; }
	
}

//**************************
//Handles the status command
//**************************
void VendingMachine::statusCmd()
{
	//calculates the amount currently deposited into the machine
	double fullAmount = (coinsBuffer[0] * 0.05) + (coinsBuffer[1] * 0.10) + (coinsBuffer[2] * 0.25) + (billsBuffer[0] * 1) + (billsBuffer[1] * 5);
	
	//used to assign the width of each label
	const int width = 28;

	//displays the status in a formated table
	std::cout << std::setprecision(2) << std::fixed;
	std::cout << std::setw(width) << std::left << "   Amount Deposited" << " = $" << fullAmount << '\n';
	std::cout << std::setw(width) << std::left << "   Total One Dollar Bills" << " =  " << bills[0] << '\n';
	std::cout << std::setw(width) << std::left << "   Total Five Dollar Bills" << " =  " << bills[1] << '\n';
	std::cout << std::setw(width) << std::left << "   Total Nickels" << " =  " << coins[0] << '\n';
	std::cout << std::setw(width) << std::left << "   Total Dimes" << " =  " << coins[1] << '\n';
	std::cout << std::setw(width) << std::left << "   Total Quarters" << " =  " << coins[2] << '\n';
	std::cout << std::setw(width) << std::left << "   Total Coke" << " =  " << cola[0] << '\n';
	std::cout << std::setw(width) << std::left << "   Total Pepsi" << " =  " << cola[1] << '\n';
	std::cout << std::setw(width) << std::left << "   Total Sprite" << " =  " << cola[2] << '\n';
	std::cout << std::setw(width) << std::left << "   Total Fanta" << " =  " << cola[3] << '\n';
	std::cout << std::setw(width) << std::left << "   Total Faygo" << " =  " << cola[4] << '\n';
	std::cout << std::setw(width) << std::left << "   Total Cups" << " =  " << cups << '\n';
}

//**************************
//Handles the unlock command
//**************************
void VendingMachine::unlockCmd(std::string option)
{
	//unlocks the machine if the correct password is given
	if (option == password) { 
		serviceMode = true;
		std::cout << "Unlocked!" << '\n';
	}

	//displays an error message if the incorrect password is given
	else { std::cout << "Invalid password, try again" << '\n'; }
}

//************************
//Handles the lock command
//************************
void VendingMachine::lockCmd(std::string option)
{
	//locks the machine if the correct password is given
	if (option == password) { 
		serviceMode = false;
		std::cout << "Locked" << '\n';
	}

	//displays an error message if the incorrect password is given
	else { std::cout << "Invalid password, try again" << '\n'; }
}

//************************
//Handles the help command
//************************
void VendingMachine::helpCmd()
{

	//displays all commands in serivce mode
	if (serviceMode == true) {
		std::cout << "Commands in Service Mode are:" << '\n';
		std::cout << "Coin <value> where value is 5 10 25 nickel dime quarter" << '\n';
		std::cout << "Bill <value> where value is 1 5" << '\n';
		std::cout << "Cola <value> where value is coke pepsi sprite fanta faygo  [-no_ice]" << '\n';
		std::cout << "Return" << '\n';
		std::cout << "Status" << '\n';
		std::cout << "Exit" << '\n';
		std::cout << "Add [COLA|CUPS] brand <quantity>" << '\n';
		std::cout << "Add|Remove [Coins|Bills] <denomination> <quantity>" << '\n';
		std::cout << "Lock [password]" << '\n';
	}

	//displays all commands in normal mode
	else {
		std::cout << "Commands in Normal Mode are:" << '\n';
		std::cout << "Coin <value> where value is 5 10 25 nickel dime quarter" << '\n';
		std::cout << "Bill <value> where value is 1 5" << '\n';
		std::cout << "Cola <value> where value is coke pepsi sprite fanta faygo  [-no_ice]" << '\n';
		std::cout << "Return" << '\n';
		std::cout << "Status" << '\n';
		std::cout << "Exit" << '\n';
		std::cout << "Unlock [password]" << '\n';
	}
}

//*********************************************************
//Calculates and dispenses the appropriate amount of change
//*********************************************************
void VendingMachine::dispenseChange()
{
	int amountPaid = convertBufferToValue(); //amount of money deposited
	int desiredChange = amountPaid - PRICE; //amount of money that the change needs to equal
	int changeMade = 0; //the amount of change that is made

	//dispenses $5 while the change needed is more or equal to that
	while (desiredChange >= (changeMade + 500) && bills[1] > 0) {
		bills[0] -= 1;
		changeMade += 500;
	}
	//dispenses $1 while the change needed is more or equal to that
	while (desiredChange >= (changeMade + 100) && bills[0] > 0) {
		bills[0] -= 1;
		changeMade += 100;
	}
	//dispenses $0.25 while the change needed is more or equal to that
	while (desiredChange >= (changeMade + 025) && coins[2] > 0) {
		coins[2] -= 1;
		changeMade += 25;
	}
	//dispenses $0.10 while the change needed is more or equal to that
	while (desiredChange >= (changeMade + 010) && coins[1] > 0) {
		coins[1] -= 1;
		changeMade += 10;
	}
	//dispenses $0.05 while the change needed is more or equal to that
	while (desiredChange >= (changeMade + 005) && coins[0] > 0) {
		coins[0] -= 1;
		changeMade += 5;
	}

	//adds the deposited buffer change into the actual machine
	for (int i = 0; i < AMOUNTOFCOINS; i++) {
		coins[i] += coinsBuffer[i];
		coinsBuffer[i] = 0;
	}
	for (int i = 0; i < AMOUNTOFBILLS; i++) {
		bills[i] += billsBuffer[i];
		billsBuffer[i] = 0;
	}
}

//******************************
//Dispenses the appropriate cola
//******************************
void VendingMachine::dispenseCola(int colaIndex, std::string colaString, std::string switchParameter)
{
	//checks if the request cola is in stock
	if (cola[colaIndex] > 0) {

		//Dispenses cola with ice if there is not a switch parameter
		if (switchParameter == "") {
			cola[colaIndex] -= 1;
			cups -= 1;
			std::cout << "Dispensing " << colaString << " with ice" << '\n';
			dispenseChange();
		}

		//Dispenses cola without ice if there is a valid "-no_ice" switch parameter
		else if (switchParameter == "-NO_ICE") {
			cola[colaIndex] -= 1;
			cups -= 1;
			std::cout << "Dispensing " << colaString << " without ice" << '\n';
			dispenseChange();
		}

		//displays an error message if the switch parameter is invalid
		else {
			std::cout << switchParameter << " is not a valid parameter for the COLA command" << '\n';
		}
	}

	//displays an error message if the requested cola is not in stock
	else {
		std::cout << "Something went wrong. Returning your money..." << '\n';
		returnCmd();
	}
}

//*************************************************
//Adjusts the amount of an item held in the machine
//*************************************************
void VendingMachine::adjustItemCountCmd(std::string command, std::string option, std::string switchParameter, std::string switchParameter2)
{
	int quantity;
	int colaTypeChosen;

	//adds an item
	if (command == "ADD") {

		//adds a cola
		if (option == "COLA") {
			bool validity = false;

			//checks if the string entered is a valid cola type
			for (int i = 0; i < SIZEOFCOLATYPES; i++) {
				if (switchParameter == colaTypes[i]) { //checks if the string matches any in the array
					validity = true;
					colaTypeChosen = i;
				}
			}

			//proceeds if the string entered was valid
			if (validity) {
				bool quantityOnlyDigits = (switchParameter2.find_first_not_of("0123456789") == std::string::npos); //checks for values that aren't numeric

				//checks if a valid quantity was requested
				if (quantityOnlyDigits) {
					quantity = convertStringToNumber(switchParameter2);
					cola[colaTypeChosen] += quantity;
					std::cout << "Added " << quantity << " " << switchParameter << "!\n";
				}

				//displays an error message if an invalid quantity was requested
				else { std::cout << switchParameter2 << " is not a valid quantity" << '\n'; }
			}
			//displays an error message if an invalid cola was requested
			else { std::cout << switchParameter << " is not a valid cola" << '\n'; }
		}

		//adds cups
		else if (option == "CUPS") {

			//checks if the command contains too many parameters
			if (switchParameter2 == "") {

				bool quantityOnlyDigits = (switchParameter.find_first_not_of("0123456789") == std::string::npos); //checks for values that aren't numeric

				//adds the amount of cups requested if the quantity is valid
				if (quantityOnlyDigits) {
					quantity = convertStringToNumber(switchParameter);
					cups += quantity;
					std::cout << "Added " << quantity << " " << option << "!\n";
				}

				//displays an error message if an invalid quantity was requested
				else { std::cout << switchParameter << " is not a valid quantity" << '\n'; }
			}

			//displays an error message if the command contained too many parameters
			else { std::cout << switchParameter2 << " is not a valid parameter for the " << command << " command" << '\n'; }
		}

		//displays an error message if the command contained an invalid parameters
		else { std::cout << "The " << option << " option is not valid for the " << command << " command" << '\n'; }
	}

	//displays an error message if the user attempts to remove an item
	else {
		std::cout << "The " << option << " option is not valid for the " << command << " command" << '\n';
	}
}

//*********************************************
//Converts the buffer change to a numeric value
//*********************************************
int VendingMachine::convertBufferToValue()
{
	//holds the actual numeric value of the change
	int value = 0;

	//temporary variables
	int numOfBills, numOfFiveBills, numOfNickels, numOfDimes, numOfQuarters;
	numOfBills = billsBuffer[0];
	numOfFiveBills = billsBuffer[1];
	numOfNickels = coinsBuffer[0];
	numOfDimes = coinsBuffer[1];
	numOfQuarters = coinsBuffer[2];

	//checks value of all inserted bills
	while (numOfFiveBills > 0) {
		numOfFiveBills -= 1;
		value += 500;
	}
	while (numOfBills > 0) {
		numOfBills -= 1;
		value += 100;
	}

	//checks value of all inserted coins
	while (numOfQuarters > 0) {
		numOfQuarters -= 1;
		value += 25;
	}
	while (numOfDimes > 0) {
		numOfDimes -= 1;
		value += 10;
	}
	while (numOfNickels > 0) {
		numOfNickels -= 1;
		value += 5;
	}

	return value;
}


//*************************************************
//Converts a coin's name to its corresponding index 
//*************************************************
int VendingMachine::convertStringCoinToInt(std::string s)
{
	//returns -1 if the string is invalid
	int value = -1;

	if (s == "NICKEL" || s == "5") { value = 0; }
	else if (s == "DIME" || s == "10") { value = 1; }
	else if (s == "QUARTER"|| s == "25") { value = 2; }


	return value;
}

//*************************************************
//Converts a bill's name to its corresponding index
//*************************************************
int VendingMachine::convertStringBillToInt(std::string s)
{
	//returns -1 if the string is invalid
	int value = -1;

	if (s == "1") { value = 0; }
	else if (s == "5") { value = 1; }

	return value;
}

//*************************************************
//Converts a cola's name to its corresponding index
//*************************************************
int VendingMachine::convertStringColaToInt(std::string s)
{
	//returns -1 if the string is invalid
	int value = -1;

	for (int i = 0; i < SIZEOFCOLATYPES; i++) {
		if (s == colaTypes[i]) { //checks if the string matches any in the array
			value = i;
		}
	}

	return value;
}

//******************************************************************************
//Checks if enough change is held in order to dispense change from a transaction
//******************************************************************************
bool VendingMachine::checkIfChangeAvailable(double oneOrFiveDollarBill)
{

	bool avaiableChange = false; //holds whether or not change can be made
	int amountPaid = convertBufferToValue(); //the amount deposited into the machine
	int desiredChange = amountPaid - PRICE; //the amount change should equal to

	//holds the actual numeric value of the change
	int changeMade = 0;

	//temporary variables
	int fiveBills = bills[1];
	int oneBills = bills[0];
	int quarterCoins = coins[2];
	int dimeCoins = coins[1];
	int nickelCoins = coins[0];

	//if possible, adds $5 to the temporary value while change needed is more or equal to that
	while (desiredChange >= (changeMade + 500) && fiveBills > 0) {
		fiveBills -= 1;
		changeMade += 500;
	}
	//if possible, adds $1 to the temporary value while change needed is more or equal to that
	while (desiredChange >= (changeMade + 100) && oneBills > 0) {
		oneBills -= 1;
		changeMade += 100;
	}
	//if possible, adds $0.25 to the temporary value while change needed is more or equal to that
	while (desiredChange >= (changeMade + 025) && quarterCoins > 0) {
		quarterCoins -= 1;
		changeMade += 25;
	}
	//if possible, adds $0.10 to the temporary value while change needed is more or equal to that
	while (desiredChange >= (changeMade + 010) && dimeCoins > 0) {
		dimeCoins -= 1;
		changeMade += 10;
	}
	//if possible, adds $0.05 to the temporary value while change needed is more or equal to that
	while (desiredChange >= (changeMade + 005) && nickelCoins > 0) {
		nickelCoins -= 1;
		changeMade += 5;
	}

	if (changeMade == desiredChange) 
		{ avaiableChange = true; }

	return avaiableChange;
}

//************************************************
//Checks if the cola type entered is a valid brand
//************************************************
bool VendingMachine::checkForValidCola(std::string s)
{
	bool validity = false;
	s = toUpper(s);

	//checks the given string against each possible value
	for (int i = 0; i < SIZEOFCOLATYPES; i++) {
		if (s == colaTypes[i]) {
			validity = true;
		}
	}

	return validity;
}

//**********************************************
//Checks if the change entered is a valid amount
//**********************************************
bool VendingMachine::checkForValidChange(std::string s)
{
	bool validity = false;
	const int SIZEOFCHANGETYPES = AMOUNTOFCOINS * 2;
	std::string changeTypes[] = { "5", "10", "25", "NICKEL", "DIME", "QUARTER"};
	s = toUpper(s);

	//checks the given string against each possible value
	for (int i = 0; i < SIZEOFCHANGETYPES; i++) {
		if (s == changeTypes[i]) {
			validity = true;
		}
	}

	return validity;
}

//********************************************
//Checks if the bill entered is a valid amount
//********************************************
bool VendingMachine::checkForValidBill(std::string s)
{
	bool validity = false;
	std::string billTypes[] = { "1", "5" };
	s = toUpper(s);

	//checks the given string against each possible value
	for (int i = 0; i < AMOUNTOFBILLS; i++) {
		if (s == billTypes[i]) {
			validity = true;
		}
	}

	return validity;
}

//********************************************************
//Checks if the cola type entered is available to dispense
//********************************************************
bool VendingMachine::checkForAvailableCola(std::string s)
{
	bool tmp = false;
	s = toUpper(s);
	int colaAvailable = cola[convertStringColaToInt(s)];

	//returns true if the number of cola is greater than 0
	if (colaAvailable > 0)
		{ tmp = true; }

	return tmp;
}

//****************************************
//Converts a string to its numerical value
//****************************************
int VendingMachine::convertStringToNumber(std::string s)
{
	//uses a stringstream for the conversion
	std::stringstream ss(s);

	//converts the string to the int value
	int tmp = 0;
	ss >> tmp;

	//returns the value
	return tmp;
}

//**************************************************************************************************************
//Adds or removes currency of a certain denomination. For example, it may add a single $5 bill to the total held
//**************************************************************************************************************
void VendingMachine::addRemoveDenomination(std::string addOrRemove, std::string type, std::string denomination, std::string quantity)
{
	int tmpQuantity;
	int index;

	//attempts to add an item
	if (addOrRemove == "ADD") {

		//attempts to add a coin
		if (type == "COINS") {
			index = convertStringCoinToInt(denomination);

			//checks if valid denomination is entered
			if (index >= 0 && index < AMOUNTOFCOINS) {
				bool quantityOnlyDigits = (quantity.find_first_not_of("0123456789") == std::string::npos); //checks for values that aren't numeric

				//adds the item if a valid quantity was requested
				if (quantityOnlyDigits) {
					tmpQuantity = convertStringToNumber(quantity);
					coins[index] += tmpQuantity;
					std::cout << "Added " << tmpQuantity << " " << denomination << "-denomination " << type << "!\n";
				}

				//displays an error message if an invalid quantity was requested
				else { std::cout << quantity << " is not a valid quantity" << '\n'; }
			}

			//displays an error message if an invalid denomination was requested
			else { std::cout << denomination << " is not a valid denomination" << '\n'; }
		}

		//attempts to add a bill
		else {
			index = convertStringBillToInt(denomination);

			//checks if valid denomination is entered
			if (index >= 0 && index < AMOUNTOFBILLS) {
				bool quantityOnlyDigits = (quantity.find_first_not_of("0123456789") == std::string::npos); //checks for values that aren't numeric

				//adds the item if a valid quantity was requested
				if (quantityOnlyDigits) {
					tmpQuantity = convertStringToNumber(quantity);
					bills[index] += tmpQuantity;
					std::cout << "Added " << tmpQuantity << " " << denomination << "-denomination " << type << "!\n";
				}

				//displays an error message if an invalid quantity was requested
				else { std::cout << quantity << " is not a valid quantity" << '\n'; }
			}

			//displays an error message if an invalid denomination was requested
			else { std::cout << denomination << " is not a valid denomination" << '\n'; }
		}
	}

	//attempts to remove an item
	else {

		//attempts to remove a coin
		if (type == "COINS") {
			index = convertStringCoinToInt(denomination);

			//checks if valid denomination is entered
			if (index >= 0 && index < AMOUNTOFCOINS) {
				bool quantityOnlyDigits = (quantity.find_first_not_of("0123456789") == std::string::npos); //checks for values that aren't numeric

				//checks if a valid quantity was requested
				if (quantityOnlyDigits) {
					tmpQuantity = convertStringToNumber(quantity);

					//sets the amount of items requested equal to the amount available if more than that was requested
					if (tmpQuantity > coins[index]) 
						{ tmpQuantity = coins[index]; }
					coins[index] -= tmpQuantity;
					std::cout << "Removed " << tmpQuantity << " " << denomination << "-denomination " << type << ", leaving " << coins[index] << " std::left!\n";
				}

				//displays an error message if an invalid quantity was requested
				else { std::cout << quantity << " is not a valid quantity" << '\n'; }
			}

			//displays an error message if an invalid denomination was requested
			else { std::cout << denomination << " is not a valid denomination" << '\n'; }
		}

		//attempts to remove a bill
		else {
			index = convertStringBillToInt(denomination);

			//checks if valid denomination is entered
			if (index >= 0 && index < AMOUNTOFBILLS) {
				bool quantityOnlyDigits = (quantity.find_first_not_of("0123456789") == std::string::npos); //checks for values that aren't numeric

				//checks if a valid quantity was requested
				if (quantityOnlyDigits) {
					tmpQuantity = convertStringToNumber(quantity);

					//sets the amount of items requested equal to the amount available if more than that was requested
					if (tmpQuantity > bills[index]) { tmpQuantity = bills[index]; }
					bills[index] -= tmpQuantity;
					std::cout << "Removed " << tmpQuantity << " " << denomination << "-denomination " << type << ", leaving " << bills[index] << " std::left!\n";
				}

				//displays an error message if an invalid quantity was requested
				else { std::cout << quantity << " is not a valid quantity" << '\n'; }
			}

			//displays an error message if an invalid denomination was requested
			else { std::cout << denomination << " is not a valid denomination" << '\n'; }
		}
	}
}

//********************************************************
//Parses the command out of the string entered by the user
//********************************************************
void VendingMachine::parseCommand(std::string linestring)
{
	//keeps track of whether or not the command entered is valid
	bool commandFound = false;

	//the amount of possible commands
	const int NUMOFCOMMANDS = 11;

	//list of possible commands
	std::string validCommands[NUMOFCOMMANDS] = { "UNLOCK", "HELP", "COIN", "BILL", "COLA", "RETURN", "STATUS", "EXIT", "ADD", "REMOVE", "LOCK" };

	std::string command;
	std::istringstream iss;
	
	//adds the user's input into the stream buffer
	iss.str(linestring); 
	iss >> command;
	command = toUpper(command);

	//checks whether the command is valid
	for (int i = 0; i < NUMOFCOMMANDS; i++) {
		if (command == validCommands[i]){
			if (serviceMode == false && i < 8)
				commandFound = true;

			else if (serviceMode == true && i > 0)
				commandFound = true;
		}
	}

	//displays an error if the command is invalid
	if (commandFound == false) {
		std::cout << "Invalid command. Type HELP for a list of valid commands" << '\n';
	}

	//parses the option if the command is valid
	else {
		parseOption(command, linestring);
	}
}

//*******************************************************
//Parses the option out of the string entered by the user
//*******************************************************
void VendingMachine::parseOption(std::string command, std::string linestring)
{
	std::string tmpString, option = "";
	std::istringstream iss;

	//adds the user's input back into the stream buffer
	iss.str(linestring);

	//removes the command from the buffer
	iss >> tmpString;

	//gets the command
	if (iss) {
		iss >> option;
	}

	//toUpper's the option unless the command was lock or unlock
	if (command != "LOCK" && command != "UNLOCK") {
		option = toUpper(option);
	}

	//parses the switch
	parseSwitch(option, command, linestring);
}

//*******************************************************
//Parses the switch out of the string entered by the user
//*******************************************************
void VendingMachine::parseSwitch(std::string option, std::string command, std::string linestring)
{
	const int NUMOFSWITCHES = 2;
	std::string switchParameter[NUMOFSWITCHES] = { "", "" };
	std::istringstream iss;

	//used to check if the command has too many parameters
	std::string tmpSwitch;

	//adds the user's input back into the stream buffer
	iss.str(linestring);

	//removes the command and option from the buffer
	for (int i = 0; i < 2; i++) {
		iss >> tmpSwitch;
	}

	//gets the next two parameters
	for (int i = 0; i < 2; i++) {
		if (iss) {
			iss >> switchParameter[i];
		}
	}

	//checks and gets if there are any last aditional parameters
	tmpSwitch = "";
	if (iss) {
		iss >> tmpSwitch;
	}

	switchParameter[0] = toUpper(switchParameter[0]);
	switchParameter[1] = toUpper(switchParameter[1]);

	//parses the full command if there are not too many parameters
	if (tmpSwitch != "") 
		{ std::cout << "Too many parameters!" << '\n'; }
	else
		{ goToCmd(command, option, switchParameter[0], switchParameter[1]); }
	
}

//**************************************************************************************************
//Takes the individually parsed input and parses the full command to call the corresponding function
//**************************************************************************************************
void VendingMachine::goToCmd(std::string command, std::string option, std::string switchParameter, std::string switchParameter2)
{
	//runs the requested exit command if the correct amount of parameters were given
	if (command == "EXIT") {
		if (option == "") { runProgram = false; }
		else { std::cout << "The " << option << " option is not valid for the " << command << " command" << '\n'; }
	}

	//runs the requested help command if the correct amount of parameters were given
	else if (command == "HELP") {
		if (option == "") { helpCmd(); }
		else { std::cout << "The " << option << " option is not valid for the " << command << " command" << '\n'; }
	}

	//runs the requested return command if the correct amount of parameters were given
	else if (command == "RETURN") {
		if (option == "") { returnCmd(); }
		else { std::cout << "The " << option << " option is not valid for the " << command << " command" << '\n'; }
	}

	//runs the requested lock command if the correct amount of parameters were given
	else if (command == "LOCK" && option != "") {
		if (switchParameter == "") { lockCmd(option); }
		else { std::cout << "A switch parameter is not valid for the " << command << " command" << '\n'; }
	}

	//runs the requested unlock command if the correct amount of parameters were given
	else if (command == "UNLOCK" && option != "") {
		if (switchParameter == "") { unlockCmd(option); }
		else { std::cout << "A switch parameter is not valid for the " << command << " command" << '\n'; }
	}

	//runs the requested status command if the correct amount of parameters were given
	else if (command == "STATUS") {
		if (option == "") { statusCmd(); }
		else { std::cout << "The " << option << " option is not valid for the " << command << " command" << '\n'; }
	}

	//runs the requested coin command if the correct amount of parameters were given
	else if (command == "COIN" && option != "") {
		if (switchParameter == "") { coinCmd(option); }
		else { std::cout << "A switch parameter is not valid for the " << command << " command" << '\n'; }
	}

	//runs the requested bill command if the correct amount of parameters were given
	else if (command == "BILL" && option != "") {
		if (switchParameter == "") { billCmd(option); }
		else { std::cout << "A switch parameter is not valid for the " << command << " command" << '\n'; }
	}

	//runs the requested cola command if the correct amount of parameters were given
	else if (command == "COLA" && option != "") {
		if (switchParameter2 == "") { colaCmd(option, switchParameter); }
		else { std::cout << "The " << command << " command has too many parameters" << '\n'; }
	}

	//runs the requested add or remove command if the correct amount of parameters were given
	else if (command == "ADD" && option != "" || command == "REMOVE" && option != "") {

		//checks if an appropriate number of parameters are given if the user want to add/remove cups
		if (option == "CUPS" && command == "ADD") {
			if (switchParameter != "") {
				if (switchParameter2 == "") {
					adjustItemCountCmd(command, option, switchParameter, switchParameter2);
				}

				//displays an error message that the command has too many parameters
				else { std::cout << "The " << command << " command has too many parameters" << '\n'; }
			}

			//displays an error message that the command has too few parameters
			else { std::cout << "The " << command << " command has too few parameters" << '\n'; }
		}

		//checks if an appropriate number of parameters are given if the user want to add/remove cola
		else if (option == "COLA" && command == "ADD") {
			if (switchParameter2 != "") {
				adjustItemCountCmd(command, option, switchParameter, switchParameter2);
			}

			//displays an error message that the command has too few parameters
			else { std::cout << "The " << command << " command has too few parameters" << '\n'; }
		}

		//checks if an appropriate number of parameters are given if the user want to add/remove coins
		else if (option == "COINS") {
			if (switchParameter2 != "") {
				addRemoveDenomination(command, option, switchParameter, switchParameter2);
			}

			//displays an error message that the command has too few parameters
			else { std::cout << "The " << command << " command has too few parameters" << '\n'; }
		}

		//checks if an appropriate number of parameters are given if the user want to add/remove bills
		else if (option == "BILLS") {
			if (switchParameter2 != "") {
				addRemoveDenomination(command, option, switchParameter, switchParameter2);
			}

			//displays an error message that the command has too few parameters
			else { std::cout << "The " << command << " command has too few parameters " << '\n'; }
		}

		//displays an error message that the option is invalid
		else { std::cout << "The " << option << " option is not valid for the " << command << " command" << '\n'; }
	}

	//displays an error message that the requested command had too few parameters
	else { std::cout << "The " << command << " command has too few parameters " << '\n'; }
}

//**********************************************************************
//Returns the value of runProgram to determine if the program is exiting
//**********************************************************************
bool VendingMachine::getProgramRunningStatus() {
	return runProgram;
}


//*************************************************************************
//Returns the value of serviceMode to determine what mode the program is in
//*************************************************************************
bool VendingMachine::getModeStatus() {
	return serviceMode;
}

//********************************************
//Converts a string to soley upper case values
//********************************************
std::string VendingMachine::toUpper(std::string s)
{
	//loops through the string to convert each character to uppercase
	for (auto& c : s)
		{ c = toupper(c); } 

	return s;
}

#endif