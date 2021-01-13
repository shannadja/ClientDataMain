//Implementation
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>

#include "ClientData.h" 

using namespace std;

// default ClientData constructor
ClientData::ClientData(string fullNameValue, int accountNumberValue, double currentBalanceValue, double creditLimitValue)
{
	setCustomerName(fullNameValue);
	setAccountNumber(accountNumberValue);
	setCurrentBalance(currentBalanceValue);
	setCreditLimit(creditLimitValue);
}

// get & set accountNumber value
int ClientData::getAccountNumber() const 
{ 
	return accountNumber; 
}
void ClientData::setAccountNumber( int accountNumberValue ) 
{ 
	accountNumber = accountNumberValue; 
}
	
// get customerName value
string ClientData::getCustomerName() const 
{ 
	return customerName; 
}

// set customerName value
void ClientData::setCustomerName( string fullNameString )
{
	// copy at most 50 characters from string to customerName
	const char *fullNameValue = fullNameString.data();
	int length = fullNameString.size();
	length = ( length < 50 ? length : 49 );
	std::strncpy( customerName, fullNameValue, length );
	customerName[length] = '\0'; 
} 

// get & set currentBalance value
double ClientData::getCurrentBalance() const 
{
	return currentBalance;
}
 
void ClientData::setCurrentBalance( double balanceValue ) 
{
	currentBalance = balanceValue;
}

// get & set creditLimit value
double ClientData::getCreditLimit() const \
{ 
	return creditLimit;
}

void ClientData::setCreditLimit( double creditLimitValue ) 
{
	creditLimit = creditLimitValue;
}

void ClientData::printRecords(const char *fileName)
{
	ifstream inCreditSystem("ARXEIO.dat", ios::in);
	
	ClientData client;
	
	if(!inCreditSystem)
	{
		cerr << "File could not be opened!" << endl;
		exit(1);
	}
	
	cout << left << setw(51) << "NAME" << setw(12) << "ACCOUNT #" << setw(10) << "BALANCE" << setw(15) << "CREDIT LIMIT" << endl;
	
	inCreditSystem.read(reinterpret_cast<char*>(&client), sizeof(ClientData));
	
	while (inCreditSystem && !inCreditSystem.eof())
	{
		if (client.getAccountNumber() != 0) 
			outputLine(cout, client);
		inCreditSystem.read(reinterpret_cast<char*>(&client), sizeof(ClientData));
	}
}

void ClientData::outputLine(ostream &output, const ClientData &record)
{
	output << left << setw(51) << record.getCustomerName()
	<< setw(12) << record.getAccountNumber()
	<< setw(7) << setprecision(2) << right << fixed
	<< showpoint << record.getCurrentBalance()
	<< setw(15) << setprecision(2) << right << fixed
	<< showpoint << record.getCreditLimit() << endl;
}

bool ClientData::file_exist(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

void ClientData::begin()
{
	int num;
	cout << "\nENTER ACCOUNT NUMBER: ";
	cin >> num;
	if (num > 0)
		searchFile(num);
	else
	{
		printRecords("ARXEIO.dat");
		exit(1);
	}
}

void ClientData::searchFile(int num)
{
	bool error = false;
	double purchase;
	char fullName[50];
	
	fstream inCreditSystem("ARXEIO.dat", ios::in | ios::out | ios::binary);
	ClientData client;
	
	if(!inCreditSystem)
	{
		cerr << "File could not be opened!" << endl;
		exit(1);
	}

	inCreditSystem.seekp((num - 1) * sizeof(ClientData));
	inCreditSystem.read(reinterpret_cast<char*>(&client), sizeof(ClientData));
	
	do
	{
		cout << "ENTER AMOUNT OF PURCHASE: ";
		cin >> purchase;
		if (purchase < 0)
		{
			cout << "AMOUNT OF PURCHASE MUST BE >0." << endl;
			cout << "ENTER AMOUNT OF PURCHASE: ";
			cin >> purchase;
		}
	} while (purchase < 0);	
	
	if (num == client.getAccountNumber())
	{
		// Epalitheusi onomatos
		cout << "ENTER CUSTOMER NAME: ";
		cin.ignore();
		cin.getline(fullName, 50);
		string nameCheck(fullName);
		
		if (client.getCustomerName().compare(nameCheck))
			error = true;
		
		if (error)
		{
			cout << "\n RE-ENTER DATA ---ACCOUNT NUMBER AND CUSTOMER NAME DO NOT MATCH ." << endl;
			begin();
		}
		else
		{
			transaction(num, purchase, client.getCreditLimit());
			begin();
		}
	} 
	else
	{
		newRecord(num, purchase);
		begin();
	}	
}

void ClientData::newRecord(int num, double purchase)
{
	char ch;
	char nameArray[50];
	double credit;
	
	fstream inCreditSystem("ARXEIO.dat", ios::in | ios::out | ios::binary);
	ClientData client;
	
	if(!inCreditSystem)
	{
		cerr << "File could not be opened!" << endl;
		exit(1);
	}
	
	do
	{
		cout << "IS THIS A NEW ACCOUNT NUMBER? (Y OR N): ";
		cin >> ch;
		if (ch != 'Y' && ch != 'N') 
		{
			cout << "\nWRONG CHARACTER ENTERED!" << endl;
			cout << "IS THIS A NEW ACCOUNT NUMBER? (Y OR N): ";
			cin >> ch;
		}
	} while (ch != 'Y' && ch != 'N');
	
	if (ch == 'Y')
	{
		client.setAccountNumber(num);
		
		cout << "ENTER CUSTOMER NAME: ";
		cin.ignore();
		cin.getline(nameArray, 50);
		string name(nameArray);
		client.setCustomerName(name);
		
		do
		{
			cout << "ENTER CREDIT LIMIT: ";
			cin >> credit;
			if (credit <= 0)
			{
				cout << "\nCREDIT LIMIT MUST BE >0." << endl;
				cin >> credit;
			}
		} while (credit <= 0);
		
		client.setCreditLimit(credit);
		
		inCreditSystem.seekp((num - 1) * sizeof(ClientData));
		inCreditSystem.write(reinterpret_cast<const char*>(&client), sizeof(ClientData));
		inCreditSystem.read(reinterpret_cast<char*>(&client), sizeof(ClientData));
		
		transaction(num, purchase, credit);
	}
	else
	{ 
		cout << "\nINVALID ACCOUNT NUMBER -- RE-ENTER DATA.\n" << endl;
	}
}

void ClientData::transaction(int num, double purchase, double credit)
{
	double over, balPurch;

	fstream inCreditSystem("ARXEIO.dat", ios::in | ios::out | ios::binary);
	ClientData client, blankClient;
	
	if(!inCreditSystem)
	{
		cerr << "File could not be opened!" << endl;
		exit(1);
	}
	
	inCreditSystem.seekp((num - 1) * sizeof(ClientData));
	inCreditSystem.read(reinterpret_cast<char*>(&client), sizeof(ClientData));
	
	client.setCreditLimit(credit);
	
	balPurch = client.getCurrentBalance() + purchase;

	if (balPurch <= client.getCreditLimit())
	{
		client.setCurrentBalance(client.getCurrentBalance() + purchase);
			
		inCreditSystem.seekp((num - 1) * sizeof(ClientData));
		inCreditSystem.write(reinterpret_cast<const char*>(&client), sizeof(ClientData));
			
		cout << "\nDEAR " << client.getCustomerName() << "," << endl;
		cout << "THANK YOU FOR THE BUSINESS.\n" << endl;
	}
	else
	{
		over = (client.getCurrentBalance() + purchase) - client.getCreditLimit();
		cout << "\nDEAR " << client.getCustomerName() << "," << endl;
		cout << "WE CANNOT AUTHORIZE THIS PURCHASE AT THIS TIME AS IT" << endl;
		cout << "WOULD PUT YOU E" << over << " OVER YOUR CREDIT LIMIT.\n" << endl;

		inCreditSystem.read(reinterpret_cast<char*>(&client), sizeof(ClientData));
	}	
}
