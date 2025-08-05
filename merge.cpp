#include <iostream>
#include <vector>
#include <algorithm>  // for std::set_union
 
using namespace std;
 
int main() {
    vector<int> v1 = {1, 2, 3, 4};
    vector<int> v2 = {1, 2, 3, 5, 6};
    vector<int> t(v1.size() + v2.size());
 
    auto it = set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), t.begin());
    t.resize(it - t.begin());
 
    for (auto i : t) {
        cout << i << " ";
    }
 
    return 0;
}