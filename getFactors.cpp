#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> getSieve(int n) {
    vector<int> sieve(n);

    for (int i = 0; i < n; i++) {
        sieve[i] = i;
    }

    for (int i = 2; i * i < n; i++) {
        if (sieve[i] == i) {
            for (int j = i * i; j < n; j+=i) {
                if (sieve[j] == j) {
                    sieve[j] = i;
                }
            }
        }
    }

    return sieve;
}

vector<vector<int> > getFactors(int n) {
    vector<int> sieve = getSieve(n);

    vector<vector<int> > factors(n);
    factors[1].push_back(1);

    for (int i = 2; i < n; i++) {

        int seed = i;
        int power = 0;
        while (seed % sieve[i] == 0) {
            seed /= sieve[i];
            power++;
        }

        for (int j = 0; j < (int)factors[seed].size(); j++) {
            int factor = factors[seed][j];
            for (int p = 0; p <= power; p++) {
                factors[i].push_back(factor);
                factor *= sieve[i];
            }
        }

        sort(factors[i].begin(), factors[i].end());
    }

    return factors;
}


int main()
{
    auto factorsTable = getFactors(100);

    for (int i = 0; i < factorsTable.size(); i++) {
        cout << i << ":  ";
        for (auto f : factorsTable[i]) {
            cout << f << " ";
        }
        cout << endl;
    }

    return 0;
}
