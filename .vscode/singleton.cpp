#include <iostream>
using namespace std;

class Singleton {
private:
    // Private constructor
    Singleton() {
        cout << "Singleton object created.\n";
    }

    // Static instance pointer
    static Singleton* instance;

public:
    // Static method to get the same instance every time
    static Singleton* getInstance() {
        if (instance == nullptr) {
            instance = new Singleton();
        }
        return instance;
    }

    void sayHello() {
        cout << "Hello from Singleton!\n";
    }
};

// Initialize the static variable
Singleton* Singleton::instance = nullptr;

int main() {
    Singleton* obj1 = Singleton::getInstance();
    Singleton* obj2 = Singleton::getInstance();

    obj1->sayHello();

    // Both objects are the same
    if (obj1 == obj2) {
        cout << "Both are the same instance.\n";
    }

    return 0;
}
