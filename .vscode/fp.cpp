#include<iostream>
using namespace std;
class A
{
    public:
    string name;
    int age;
    A(string name, int age) : name(name) , age(age)
    {}
    void display()
    {
        cout<<"Your name is: "<<name<<" and age is:"<<age;
    }
};
class B : public A{
    public:
    B(string name, int age) : A(name, age) {}
};
int main()
{
    B obj("Harsha",21);
    B obj1("Jaaji",21);
    obj.display();
    cout<<endl;
    obj1.display();
}