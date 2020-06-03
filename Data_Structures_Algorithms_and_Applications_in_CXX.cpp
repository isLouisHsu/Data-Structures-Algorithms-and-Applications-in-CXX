#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <numeric>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <string>
#include <vector>


using namespace std;


//int main()
//{
//    string input;
//    stringstream ss;
//    getline(cin, input);
//
//
//    return 0;
//}

int main()
{
	int n = 0; cin >> n;
	vector<string> S(n);
	for (int i = 0; i < n; i++) {
		cin >> S[i];
	}
	string T; cin >> T;

	int len = T.size();
	vector<int> dp(len + 1);
	for (int i = 1; i < len + 1; i++) {
		dp[i] = dp[i - 1];
		string t = T.substr(0, i);
		for (string s : S) {
			int m = s.size();
			if (m > t.size() || t.substr(t.size() - m, m) != s) 
				continue;
			dp[i] = max(dp[i], dp[i - m] + 1);
		}
	}

	cout << dp[len] << endl;
	return 0;
}