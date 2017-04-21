#include <iostream>
#include <algorithm>
#include <vector>


using namespace std;

struct Query {
    int l; // left query-segment bound
    int r; // right query-segment bound (not included)
    int number; // this field is used to restore inintial query order

    // add your fields here
    int result;
};


struct Mo {
    vector<int> votesForId;
    vector<int> idsVorVote;

    void addItem(int item) {
        // add item to current segment
    }

    void removeItem(int item) {
        // remove item from current segment
    }
    
    void saveResult(Query &q) {
        // save result for current segment (current query)
    }
    

    Mo(vector<int> &data, vector<Query> &queries) {
        sort(queries.begin(), queries.end(),
             [=](const Query& q1, const Query& q2)
        {
            if (q1.l / blockSize != q2.l / blockSize) {
                return q1.l / blockSize < q2.l / blockSize;
            }

            return q1.r < q2.r;
        });

        int begin = 0;
        int end = 0;
        // do the job
        for (int i = 0; i < (int)queries.size(); i++) {
            int qBegin = queries[i].l;
            int qEnd = queries[i].r;

            while (end < qEnd) {
                addItem(data[end]);
                end++;
            }
            
            while (begin < qBegin) {
                removeItem(data[begin]);
                begin++;
            }

            while (begin > qBegin) {
                begin--;
                addItem(data[begin]);
            }

            while (end > qEnd) {
                end--;
                removeItem(data[end]);
            }

            saveResult(queries[i]);
        }

        sort(queries.begin(), queries.end(),
             [](const Query &q1, const Query &q2) {
            return q1.number < q2.number;
        });
        
    }
};

int main(int argc, char *argv[])
{
    cout << "Hello World!" << endl;

    vector<int> data = {0, 2, 5, 1, 7, 9, 0, 4, 2, 8};
    vector<Query> queries = {
        {1, 2},
        {0, 9},
        {8, 9},
        {1, 7},
        {4, 6}
    };

    Mo mo(data, queries);

    for (int i = 0; i < (int)queries.size(); i++) {
        cout << "[" << queries[i].first << "-" << queries[i].second << ") - " << queries[i].result << endl;
    }

    return 0;
}
