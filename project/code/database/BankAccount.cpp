#include "BankAccount.h"

#include <string>
#include <cstring>

using std::string;

BankAccount::BankAccount(int accno, string fname,
   string lname, double bal)
{
   setAccountNumber(accno);
   setFirstName(fname);
   setLastName(lname);
   setBalance(bal);
}

void BankAccount::setAccountNumber(int accno)
{
   accountNumber = accno;
}

void BankAccount::setLastName(string lname)
{
   const char* ln = lname.data();
   int len = lname.size();
   len = (len < MAX_SIZE ? len : MAX_SIZE - 1);
   strncpy(lastName, ln, len);
   lastName[len] = '\0';
}

void BankAccount::setFirstName(string fname)
{
   const char* fn = fname.data();
   int len = fname.size();
   len = (len < MAX_SIZE ? len : MAX_SIZE - 1);
   strncpy(firstName, fn, len);
   firstName[len] = '\0';
}

void BankAccount::setBalance(double bal)
{
   balance = bal;
}

int BankAccount::getAccountNumber() const
{
   return accountNumber;
}

string BankAccount::getFirstName() const
{
   return firstName;
}

string BankAccount::getLastName() const
{
   return lastName;
}

double BankAccount::getBalance() const
{
   return balance;
}

BankAccount::~BankAccount()
{
}