#include <iostream>
#include <fstream>
#include "ClientData.h"

using namespace std;

int main()
{	
	ClientData client;
	
	
	bool checkFile = client.file_exist("ACCT.dat");
	if (checkFile == false) // Checking if the file exists
	{
		ofstream outCreditSystem("ACCT.dat", ios::binary);
		ClientData blankClient;
		
		for (int i = 0; i < 200; i++) // If it doesn't exist, we create 200 empty records
			outCreditSystem.write(reinterpret_cast<const char*>(&blankClient), sizeof(ClientData));
	}
	
	// Then, we open the file
	ifstream inCreditSystem("ACCT.dat", ios::in | ios::out | ios::binary);
	
	if(!inCreditSystem)
	{
		cerr << "File could not be opened!" << endl;
		exit(1);
	}
	
	// Print records
	cout << "\nCURRENT RECORDS\n" << endl;
	client.printRecords("ACCT.dat");
	
	// Start the process
	client.begin();
	
	return 0;
}
