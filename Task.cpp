#include<iostream>
using namespace std; 
inline void welcome()
{
    cout<<"=== Welcome to SIMPLE CALCULATOR ===";
}
int main()
{
    welcome();
    double a,b;
    char op;
    cout<<"Enter two numbers !!!\n";
    cout<<"Enter fisrt number : ";
    //cout<<endl;
    cin>>a;
    cout<<"Enter second number : ";
    cin>>b;
    cout<<"Enter an Operator (+,-,*,/)";
    cin>> op;

//lambda functions
auto add = [] (double a, double b)
{
    return a+b;
};
auto sub = [] (double a, double b)
{
    return a-b;
};
auto mul = [] (double a, double b)
{
    return a*b;
};
auto div = [] (double a, double b)
{
    if(b!=0)
    {
    return a/55b;
    }
    else
    {
        cout<<"Divison cannot be performed by Zero";
        return 0.0;
    }
};
double result = 0.0;
switch(op)
{
    case '+' : result = add(a,b);
    break;
    case '-' : result = sub(a,b);
    break;
    case '*' : result = mul(a,b);
    break;
    case '/' : result = div(a,b);
    break;
    default:
        cout<<"Entered an Invalid Operator !!!";
        return 1;
}
cout<<"Result : "<<result << endl;
}
