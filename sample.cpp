#include<iostream>
using namespace std;
class Add
{
    public:
    int a,b;
    Add(int a,int b)
    {
        cout<<a+b;
    }
};
int main()
{
    Add a(3,2);
}