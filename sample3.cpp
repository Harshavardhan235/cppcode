#include<iostream>
#include<vector>
#include<algorithm>
#include<numeric>
#include<cstdlib>
using namespace std;
int main()
{
vector<int> qualityScores = {45, 72, 88, 60, 55, 93, 67, 80};
vector<int> copywala(qualityScores.size());
vector<int> randomwala(10);
copy(qualityScores.begin(),qualityScores.end(),copywala.begin());
cout<<"The Backup list :\n";
for(auto i : copywala)
{
    cout<<i<<" ";
}
fill(qualityScores.begin(),qualityScores.end()-3,50);
cout<<"\nFirst Five elements with default score of 50 : \n";
for(auto i: qualityScores)
{
    cout<<i<<" ";
}
generate(randomwala.begin(),randomwala.end(),[]()
{
    return rand() % 101;
});
cout<<"\nRandom Generated list : \n";
for(auto i : randomwala)
{
    cout<<i<<" ";
}
transform(qualityScores.begin(),qualityScores.end(),qualityScores.begin(),[](int n)
{
    return n+5;
});
cout<<"\nThe Transformed List(add 5) is : \n";
for(auto i : qualityScores)
{
    cout<<i<<" ";
}
replace_if(qualityScores.begin(),qualityScores.end(),[](int n)
{
    return n<60;
},60);
cout<<"\nModified list after using Replace :\n";
for(auto i : qualityScores)
{
    cout<<i<<" ";
}
qualityScores.erase(remove_if(qualityScores.begin(),qualityScores.end(),[](int n)
{
    return n<70;
}
),qualityScores.end());
cout<<"\nmodified list after removing elements : \n";
for(auto i : qualityScores)
{
    cout<<i<<" ";
}
reverse(qualityScores.begin(),qualityScores.end());
cout<<"\nReverse of the list : \n";
for(auto i : qualityScores)
{
    cout<<i<<" ";
}
partition(qualityScores.begin(),qualityScores.end(),[](int n)
{
    return n>=75;
});
cout<<"\nPartioned of the list  : \n";
for(auto i : qualityScores)
{
    cout<<i<<" ";
}
}
