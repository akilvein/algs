#include <iostream>
#include <algorithm>
#include <vector>


using namespace std;

template <typename T, typename R>
class Decompressor {
    struct query {
        pair<int, int> range;
        size_t number;
        R result;
    };

    vector<query> queries;

    struct Mo {
        int blockSize;

        bool operator()(const query &q1, const query &q2) const {
            if (q1.range.first / blockSize != q2.range.first / blockSize) {
                return q1.range.first / blockSize < q2.range.first / blockSize;
            }

            return q1.range.second < q2.range.second;
        }

        Mo(int bSize) : blockSize(bSize) {}
    };

    struct Naive {
        bool operator()(const query &q1, const query &q2) const {
            return q1.range < q2.range;
        }
    };

    struct Restore {
        bool operator()(const query &q1, const query &q2) const {
            return q1.number < q2.number;
        }
    };

    /// add your logic here!
    int addItem(int r, const int &item) const {
        return r + item;
    }

    /// add your logic here!
    int removeItem(int r, const int &item) const {
        return r - item;
    }

public:
    const R &getAnswer(int i) const {
        return queries[i].result;
    }

    Decompressor(const vector<T> &data, const vector<pair<int, int> > &q) : queries(q.size()) {
        for (size_t i = 0; i < q.size(); i++) {
            queries[i] = {q[i], i, R()};
        }

        sort(queries.begin(), queries.end(), Mo(sqrt(data.size())));
        //sort(queries.begin(), queries.end(), Naive());


        size_t begin = 0;
        size_t end = 0;
        R result = R();
        // do the job
        for (size_t i = 0; i < queries.size(); i++) {
            size_t qBegin = queries[i].range.first;
            size_t qEnd = queries[i].range.second;

            while (begin < qBegin) {
                result = removeItem(result, data[begin]);
                begin++;
            }

            while (begin > qBegin) {
                begin--;
                result = addItem(result, data[begin]);
            }

            while (end < qEnd) {
                result = addItem(result, data[end]);
                end++;
            }

            while (end > qEnd) {
                end--;
                result = removeItem(result, data[end]);
            }

            queries[i].result = result;
        }

        sort(queries.begin(), queries.end(), Restore());
    }
};

int main(int argc, char *argv[])
{
    cout << "Hello World!" << endl;

    vector<int> data = {0, 2, 5, 1, 7, 9, 0, 4, 2, 8};
    vector<pair<int, int> > queries = {
        {1, 2},
        {0, 9},
        {8, 9},
        {1, 7},
        {4, 6}
    };

    Decompressor<int, int> d(data, queries);

    for (int i = 0; i < (int)queries.size(); i++) {
        cout << "[" << queries[i].first << "-" << queries[i].second << ") - " << d.getAnswer(i) << endl;
    }

    return 0;
}
