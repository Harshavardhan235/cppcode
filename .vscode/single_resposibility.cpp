#include<iostream>
using namespace std;
class employee
{
    private:
    string name;
    public:
    employee(string name) : name(name) { };
    string getname()
    {
        return name;
    }
};
class empdisplay{
  public:
    void display(employee &emp)
    {
        cout<<"Name is "<<emp.getname();
    }
};
int main()
{
    employee e("Harsha");
    empdisplay e1;
    e1.display(e);
}