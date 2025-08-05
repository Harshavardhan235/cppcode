#include <iostream>
 
class Bird {
public:
    virtual void speak() {
        std::cout << "Bird is making a sound\n";
    }
};
 
class Parrot : public Bird {
public:
    void speak() override {
        std::cout << "Parrot is speaking\n";
    }
};
 
class Penguin : public Bird {
public:
    void speak() override {
        std::cout << "Penguin is chirping\n";
    }
};
 
void letBirdSpeak(Bird* bird) {
    bird->speak();
}
 
int main() {
    Parrot p;
    Penguin peng;
 
    letBirdSpeak(&p);
    letBirdSpeak(&peng);
 
    return 0;
}