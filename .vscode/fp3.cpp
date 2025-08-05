#include<iostream>
using namespace std;
class BankAccount
{
    private:
    int accountnumber , balance;
    public:
    /*
    BankAccount()
    {
        balance = 0;
    }*/
    BankAccount(int accountnumber, int balance=0) : accountnumber(accountnumber) , balance(balance) { }
    void deposit(int amount)
    {
        if (amount >= 0)
        {
            balance = balance + amount;
        }
        else
        {
            cout<<"Deposit request denied!";
        }
    }
    void withdraw(int amount)
    {
        if(amount>= 0 && amount <= balance)
        {
            balance = balance - amount;
        }
        else
        {
            cout<<"Withdrawal can't be accepted";
        }
    }
    void showbalance()
    {
        cout<<balance;
    }
};
int main()
{
    BankAccount b(12346);
    b.deposit(500);
    b.deposit(400);
    b.showbalance();
    cout<<endl;
    b.withdraw(200);
    b.showbalance();
}