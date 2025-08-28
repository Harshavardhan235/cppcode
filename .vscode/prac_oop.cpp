#include <iostream>
using namespace std;
class BankAcccount {
    private: 
    int balance = 0;
    public:
    void deposit(int amount) {
        if(amount > 0) balance = balance + amount;
    }
    void displayBalance() {
        cout << "Balance: " << balance << endl;
    }
    void withdraw(int amount) {
        if(amount > 0 && amount <= balance) {
            balance = balance - amount;
        } else {
            cout << "Insufficient funds or invalid amount." << endl;
        }
    }
};
int main() {
    BankAcccount bal;
    bal.deposit(10000);
    bal.displayBalance();
    bal.withdraw(5000);
    bal.displayBalance();
}
