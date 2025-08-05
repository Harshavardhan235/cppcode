#include <iostream>
 
class IPrinter {
public:
    virtual void print() = 0;
};
 
class IScanner {
public:
    virtual void scan() = 0;
};
 
class Printer : public IPrinter {
public:
    void print() override {
        std::cout << "Printing document...\n";
    }
};
 
class AllInOne : public IPrinter, public IScanner {
public:
    void print() override {
        std::cout << "All-In-One: Printing document...\n";
    }
    void scan() override {
        std::cout << "All-In-One: Scanning document...\n";
    }
};
 
int main() {
    Printer p;
    AllInOne aio;
 
    p.print();
    aio.print();
    aio.scan();
 
    return 0;
}