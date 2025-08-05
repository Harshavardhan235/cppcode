#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>  // for generate
using namespace std;

int main() {
    // Initial vector
    vector<int> nums = {1, 2, 3, 4, 5, 6, 6, 7, 8};

    cout << "--- std::for_each ---" << endl;
    for_each(nums.begin(), nums.end(), [](int n) {
        cout << "Number: " << n << endl;
    });

    cout << "\n--- std::find ---" << endl;
    auto it = find(nums.begin(), nums.end(), 6);
    if (it != nums.end()) {
        cout << "Found 6 at position: " << distance(nums.begin(), it) << endl;
    }

    cout << "\n--- std::find_if ---" << endl;
    auto it2 = find_if(nums.begin(), nums.end(), [](int n) {
        return n > 5;
    });
    if (it2 != nums.end()) {
        cout << "First number > 5: " << *it2 << endl;
    }

    cout << "\n--- std::adjacent_find ---" << endl;
    auto it3 = adjacent_find(nums.begin(), nums.end());
    if (it3 != nums.end()) {
        cout << "Found adjacent duplicate: " << *it3 << endl;
    }

    cout << "\n--- std::count ---" << endl;
    int count6 = count(nums.begin(), nums.end(), 6);
    cout << "Number of 6s: " << count6 << endl;

    cout << "\n--- std::count_if ---" << endl;
    int evenCount = count_if(nums.begin(), nums.end(), [](int n) {
        return n % 2 == 0;
    });
    cout << "Even numbers count: " << evenCount << endl;

    cout << "\n--- std::equal ---" << endl;
    vector<int> nums2 = {1, 2, 3, 4, 5, 6, 6, 7, 8};
    bool same = equal(nums.begin(), nums.end(), nums2.begin());
    cout << (same ? "Equal" : "Not Equal") << endl;

    cout << "\n--- std::copy ---" << endl;
    vector<int> copyVec(nums.size());
    copy(nums.begin(), nums.end(), copyVec.begin());
    for (int n : copyVec) cout << n << " ";

    cout << "\n\n--- std::fill ---" << endl;
    fill(copyVec.begin(), copyVec.end(), 0);
    for (int n : copyVec) cout << n << " ";

    cout << "\n\n--- std::generate ---" << endl;
    int i = 1;
    generate(copyVec.begin(), copyVec.end(), [&i]() { return i++; });
    for (int n : copyVec) cout << n << " ";

    cout << "\n\n--- std::transform (multiply by 2) ---" << endl;
    transform(nums.begin(), nums.end(), nums.begin(), [](int n) {
        return n * 2;
    });
    for (int n : nums) cout << n << " ";

    cout << "\n\n--- std::replace (replace 12 with 99) ---" << endl;
    replace(nums.begin(), nums.end(), 12, 99);  // Replace 12 with 99
    for (int n : nums) cout << n << " ";

    cout << "\n\n--- std::remove_if (remove > 10) ---" << endl;
    nums.erase(remove_if(nums.begin(), nums.end(), [](int n) {
        return n > 10;
    }), nums.end());
    for (int n : nums) cout << n << " ";

    cout << "\n\n--- std::reverse ---" << endl;
    reverse(nums.begin(), nums.end());
    for (int n : nums) cout << n << " ";

    cout << "\n\n--- std::partition (evens first) ---" << endl;
    partition(nums.begin(), nums.end(), [](int n) {
        return n % 2 == 0;
    });
    for (int n : nums) cout << n << " ";

    cout << endl;
    return 0;
}
