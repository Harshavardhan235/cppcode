
using namespace std;

void checkPrime(int num) { // define function before main
    if (num <= 1) {
        cout << num << " is not a prime number." << endl;
        return;
    }
    for (int i = 2; i * i <= num; ++i) {
        if (num % i == 0) {
            cout << num << " is not a prime number." << endl;
            return;
        }
    }
    cout << num << " is a prime number." << endl;
}

int main() {
    int number;
    cout << "Enter a number: ";
    cin >> number;
    checkPrime(number);
    return 0;
}
