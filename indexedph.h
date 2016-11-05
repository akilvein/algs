#ifndef INDEXEDPQ_H
#define INDEXEDPQ_H

#include <vector>
#include <cassert>


// Sedgewick-based priority queue for graph tasks
template<typename PriorityKey>
class indexedPQ {
    std::vector<int> mHeap;
    std::vector<int> mInverse;
    std::vector<PriorityKey> mKeys;
    int mSize = 0;

    inline void exch(int i, int j) {
        int swap = mHeap[i];
        mHeap[i] = mHeap[j];
        mHeap[j] = swap;
        mInverse[mHeap[i]] = i;
        mInverse[mHeap[j]] = j;
    }

    inline bool greater(int i, int j) {
        return mKeys[mHeap[i]] > mKeys[mHeap[j]];
    }

    inline void swim(int k) {
        while (k > 0 && greater((k - 1) / 2, k)) {
            exch(k, (k - 1) / 2);
            k = ((k - 1) / 2);
        }
    }

    inline void sink(int k) {
        while (2 * k + 1 < mSize) {
            int j = 2 * k + 1;
            if (j < mSize - 1 && greater(j, j+1))
                j++;
            if (!greater(k, j))
                break;
            exch(k, j);
            k = j;
        }
    }

public:
    indexedPQ(int n) : mHeap(n), mInverse(n, -1), mKeys(n) {}

    bool notEmpty() {
        return mSize;
    }

    bool contains(int i) {
        assert(i >= 0);
        assert(i < (int)mKeys.size());

        return mInverse[i] != -1;
    }

    void push(int i, PriorityKey key) {
        assert(i >= 0);
        assert(i < (int)mKeys.size());
        assert(!contains(i));

        mInverse[i] = mSize;
        mHeap[mSize] = i;
        mKeys[i] = key;
        swim(mSize);
        mSize++;
    }

    int pop() {
        assert(mSize);

        int min = mHeap[0];
        exch(0, --mSize);
        sink(0);
        assert(min == mHeap[mSize]);
        mInverse[min] = -1;
        return min;
    }

    void decrease(int i, PriorityKey key) {
        assert(i >= 0);
        assert(i < (int)mKeys.size());
        assert(contains(i));
        assert(key < mKeys[i]);

        mKeys[i] = key;
        swim(mInverse[i]);
    }

};


#endif // INDEXEDPQ_H
