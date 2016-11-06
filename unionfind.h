#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <vector>
#include <cassert>

// Sedgewick based fixed-size union-find class
class UnionFind {
    std::vector<int> mParent;
    int mCount;

public:

    UnionFind(int n) : mParent(n), mCount(n) {
        for (size_t i = 0; i < mParent.size(); i++) {
            mParent[i] = i;
        }
    }

    int find(int p) {
        assert(p >= 0);
        assert(p < (int)mParent.size());

        while (p != mParent[p]) {
            mParent[p] = mParent[mParent[p]];
            p = mParent[p];
        }
        return p;
    }

    int size() {
        return mParent.size();
    }

    int count() {
        return mCount;
    }

    bool connected(int p, int q) {
        assert(p >= 0);
        assert(p < (int)mParent.size());
        assert(q >= 0);
        assert(q < (int)mParent.size());

        return find(p) == find(q);
    }

    void unite(int p, int q) {
        assert(p >= 0);
        assert(p < (int)mParent.size());
        assert(q >= 0);
        assert(q < (int)mParent.size());

        int rootP = find(p);
        int rootQ = find(q);
        if (rootP == rootQ) {
            return;
        }

        mParent[rootQ] = rootP;
        mCount--;
    }
};


#endif // UNIONFIND_H
