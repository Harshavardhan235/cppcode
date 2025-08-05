/*  #include<iostream>
using namespace std;
enum Day  {Sunday=0,Monday,Tuesday,Wednesday,Thursday,Friday,Saturday};
int main()
{
    int number;
    cout<<"Enter an number:";
    cin>>number;
    Day today = static_cast<Day>(number);
    switch(today)
    {
       case Sunday:
            cout << "It's Sunday\n";
            break;
        case Monday:
            cout << "It's Monday\n";
            break;
        case Tuesday:
            cout << "It's Tuesday\n";
            break;
        case Wednesday:
            cout << "It's Wednesday\n";
            break;
        case Thursday:
            cout << "It's Thursday\n";
            break;
        case Friday:
            cout << "It's Friday\n";
            break;
        case Saturday:
            cout << "It's Saturday\n";
            break;
    }
}   */

// Accessing the enum variable
/*
#include<iostream>
using namespace std;
enum alert {
    HIGH,
    LOW,
    MEDIUM
};
int main()
{
    alert a = MEDIUM;
    cout<<a;
}*/


// using struct 
/*
#include<iostream>
#include<string>
using namespace std;
struct{
    int number;
    float value;
    string name;
} sample;
int main()
{
    sample.number = 10;
    sample.value = 11.05;
    sample.name = "Harsha";
    cout<<sample.name;
    
}*/


//Function Overloading
/*
#include<iostream>
#define PI 3.1416;
using namespace std;
{
    float area(float radius)
    {
        return PI*radius*radius;
    }
    float area(float length, float width)
    {
        return length + width;
    }
    float area(float base, float height, char Type)
    {
        if(Type == 't' || Type =='T')
        {
            return base*height;
        }
    }
}
int main()
{
    cout<<"Area of the circle:"<<area(5.5);
    cout<<"Area of the Rectangle:"<<area(5.5,6.5);
    cout<<"Area of the traingle:"<<area(5.5,6.5,'t');
}
*/

//Factorial of number
/*
#include<iostream>
using namespace std;
int factorial(int n)
{
if ( n==0 || n==1)
{
    return 1;
}
else
{
    return n * factorial(n -1);
}
}
int main()
{
    int number;
    cout<<"ENter an number:";
    cin>>number;
    
    int result = factorial(number);
    cout<<"Factorial is:"<<result;
}
*/

//sorting using lambda function
/*
#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;
int main()
{
    vector <int> nums = {20,10,30,11};
    sort(nums.begin(),nums.end(),[](int a, int b))
    {
        for(auto i : nums)
        {
            cout<<i<<" ";
        }
    }
}
*/

//using private vaibles and encapsulation
/*
#include<iostream>
using namespace std;
class car
{
    private:
    string carname;
    int carmodel;
    public:
    car(string carname, int carmodel) : carname(carname) , carmodel(carmodel){}
    void display()
    {
        cout<<"Car Company :"<<carname<<endl;
        cout<<"Car Model :"<<carmodel<<endl;
    }
};
int main()
{
    car c("Toyato",1);
    c.display();
} 
*/

