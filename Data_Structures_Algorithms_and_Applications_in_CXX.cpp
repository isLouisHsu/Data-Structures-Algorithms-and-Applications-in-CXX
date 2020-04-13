#include <iostream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <numeric>
#include <vector>
#include <string>
#include <unordered_map>
#include <stack>
#include <queue>
#include "sort.h"

using namespace std;

int main()
{
    int nums[10] = {50, 30, 0, 100, 11, 2, 123, 543, 187, 49};
    vector<int> numsv(nums, nums + 10);

    shellSort(numsv);

    int num = 5;
    num = ~num + 1;

    string input;
    getline(cin, input);
    stringstream ss(input);

    return 0;
}