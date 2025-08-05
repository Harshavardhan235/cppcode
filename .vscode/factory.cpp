#include <iostream>
using namespace std;

class Animal {
public:
    virtual void speak() {
        cout << "Some animal sound.\n";
    }
};

class Dog : public Animal {
public:
    void speak() override {
        cout << "Woof!\n";
    }
};

class Cat : public Animal {
public:
    void speak() override {
        cout << "Meow!\n";
    }
};

// Factory function to create animals
Animal* createAnimal(string type) {
    if (type == "dog") {
        return new Dog();
    } else if (type == "cat") {
        return new Cat();
    } else {
        return new Animal();
    }
}

int main() {
    Animal* a1 = createAnimal("dog");
    a1->speak();  // Woof!

    Animal* a2 = createAnimal("cat");
    a2->speak();  // Meow!

    Animal* a3 = createAnimal("cow");
    a3->speak();  // Some animal sound.

    delete a1;
    delete a2;
    delete a3;

    return 0;
}
