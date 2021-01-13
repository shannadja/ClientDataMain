//Interface
#ifndef CLIENTDATA_H 
	
	#define CLIENTDATA_H
	#include <string>
	
	using namespace std;
	
	class ClientData 
	{
		public:
			// default ClientData constructor
			ClientData( string = "", int = 0, double = 0.00, double = 0.00 ); 
			
			void setAccountNumber( int );
			int getAccountNumber() const;
			
			void setCustomerName( string );
			string getCustomerName() const;
			
			void setCurrentBalance( double );
			double getCurrentBalance() const;
			
			void setCreditLimit( double );
			double getCreditLimit() const;
			
			// Filecheck
			bool file_exist(const char *fileName);
			
			// Functions
			void begin();
			void searchFile(int);
			void newRecord(int, double);
			void printRecords(const char *);
			void transaction(int, double, double);
			void outputLine(ostream&, const ClientData &);
		
		private:
			char customerName[50];
			int accountNumber;
			double currentBalance;
			double creditLimit;
	};
#endif

