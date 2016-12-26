#include <cstring>
#include <vector>
#include <cstdlib>
#include <ctime>

// 3-way string quicksort a[lo..hi] starting at dth character
void sort(vector<const char *> &a, int lo, int hi, int d) {

    // cutoff to insertion sort for small subarrays ( < 15 )
    if (hi <= lo + 30) {
        for (int i = lo; i <= hi; i++) {
            for (int j = i; j > lo && strcmp(a[j] + d, a[j-1] + d) < 0; j--) {
                swap(a[j], a[j-1]);
            }
        }
        return;
    }

    // run 3-way sort
    int lt = lo;
    int gt = hi;
    int v = a[lo][d];
    int i = lo + 1;
    while (i <= gt) {
        int t = a[i][d];
        if (t < v) {
            swap(a[lt++], a[i++]);
        } else if (t > v) {
            swap(a[i], a[gt--]);
        } else {
            i++;
        }
    }

    // a[lo..lt-1] < v = a[lt..gt] < a[gt+1..hi].
    sort(a, lo, lt-1, d);
    if (v >= 0) {
        sort(a, lt, gt, d+1);
    }

    sort(a, gt+1, hi, d);
}

void sort(vector<const char *> &a) {
    // shuffle input data
    srand(std::time(0));
    int n = a.size();
    for (int i = 0; i < n; i++) {
        int r = i + rand() % (n - i);   // between i and n-1
        swap(a[i], a[r]);
    }
    
    // sort
    sort(a, 0, (int)a.size() - 1, 0);
}
