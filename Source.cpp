#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

vector<pair<double, double>> positions = { {62.0, 58.4}, { 57.5, 56.0 }, { 51.7, 56.0 },
											{ 67.9, 19.6 }, { 57.7, 42.1 }, { 54.2, 29.1 },
											{ 46.0, 45.1 }, { 34.7, 45.1 }, { 45.7, 25.1 },
											{ 34.7, 26.4 }, { 28.4, 31.7 }, { 33.4, 60.5 },
											{ 22.9, 32.7 }, { 21.5, 45.8 }, { 15.3, 37.8 },
											{ 15.1, 49.6 }, { 9.1, 52.8 }, { 9.1, 40.3 },
											{ 2.7, 56.8 }, { 2.7, 33.1 } };


vector<vector<double>> c(20, vector<double>(20));
double inf = 1000000000;
double dp[1100000][20] = { 0 };
int prevs[1100000][20] = { 0 };
bool map[20];

double recursion(int mask, int last, int size) {
	if (size == 2) {
		return dp[mask][last];
	}
	if (dp[mask][last] < inf) return dp[mask][last];
	double sol = inf + 1;
	int tmpPrev = -1;
	for (int i = 0; i < 20; i++) {
		if (i != last) {
			int bit = 1 << i;
			bit = mask & bit;
			if (bit > 0) {
				int maskCpy = mask ^ (1 << last);
				if (dp[maskCpy][i] < inf) {
					double tmp = dp[maskCpy][i] + c[i][last];
					if (sol > tmp) {
						sol = tmp;
						tmpPrev = i;
					}
				}
				else {
					double tmp = recursion(maskCpy, i, size - 1) + c[i][last];
					if (sol > tmp) {
						sol = tmp;
						tmpPrev = i;
					}
				}				
			}
		}
	}
	prevs[mask][last] = tmpPrev;
	return dp[mask][last] = sol;	
}



int main() {
	for (int i = 0; i < 1100000; i++) {
		for (int j = 0; j < 20; j++) {
			prevs[i][j] = -1;
			dp[i][j] = inf;
		}
	}

	//Calculating values of costs between holes
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			if (i != j) {
				c[i][j] = abs(positions[i].first - positions[j].first) + abs(positions[i].second - positions[j].second);
			}
			else {
				c[i][j] = 0.0;
			}
		}
	}
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			int bitset = (1 << i) | (1 << j);
			if (i != j) {
				dp[bitset][i] = c[i][j];
				dp[bitset][j] = c[i][j];
				prevs[bitset][i] = j;
				prevs[bitset][j] = i;
			}
			else {
				dp[bitset][i] = inf;
				dp[bitset][j] = inf;
			}
		}
	}
	/*for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			int bitset = (1 << i) | (1 << j);
			if (!(dp[bitset][i] == dp[bitset][j] && dp[bitset][j] == c[i][j] && c[i][j] == c[j][i])) cout << "FALSE";
		}
	}*/
	int mask = (1 << 20) - 1;
	for (int i = 0; i < 20; i++) {
		dp[mask][i] = recursion(mask, i, 20);
	}

	/*for (int i = 0; i < 1100000; i++) {
		for (int j = 0; j < 20; j++) {
			if (prevs[i][j] != -1 && i > 1000000) {
				cout << prevs[i][j] << " " << i << " " << j << endl;
				break;
			}
		}
	}*/

	double sol = inf;
	for (int i = 0; i < 20; i++) {
		sol = min(sol, dp[mask][i]);
		//cout << dp[mask][i] << endl;
	}
	cout << "Minimum cost is: " << sol << endl;
	int pos = -1;
	sol = inf;
	vector<int> order;

	int curr = -1;
	for (int i = 0; i < 20; i++) {
		if (sol > dp[mask][i]) {
			sol = dp[mask][i];
			curr = i;
		}
	}
	int next = -1;
	for (int i = 0; i < 19; i++) {
		next = prevs[mask][curr];
		mask = mask ^ (1 << curr);
		order.push_back(curr);
		map[curr] = true;
		curr = next;
	}
	cout << "Order: " ;
	for (int i = 0; i < (int) order.size(); i++) {
		cout << order[i] + 1 << " - ";
	}
	for (int i = 0; i < 20; i++) {
		if (map[i] == false) {
			cout << i + 1 << endl;
			break;
		}
	}
	/*int arr[] = { 4, 6, 9, 10, 11, 13, 15, 18, 20, 19, 17, 16, 14, 8, 7, 5, 1, 2, 3, 12};
	double sum = 0;
	for (int i = 0; i < 20 - 1; i++) {
		sum += c[arr[i] - 1][arr[i + 1] - 1];
	}
	cout << sum << endl;*/
	return 0;
}