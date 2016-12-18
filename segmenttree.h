#include <iostream>
#include <algorithm>
#include <vector>
#include <limits>

using namespace std;


class SegmentTree {

public:

    struct Node {
        int min;

        int pending = 0;
        int from;
        int to;

        inline int size() {
            return to - from + 1;
        }
    };


private:
    vector<Node> mHeap;
    vector<int> mArray;

    static inline int prevPow2(int i) {
#if 0
        int p = 1 << (31 - __builtin_clz(i));
        return (p == i ? p / 2 : p);
#else
        i--;
        i |= (i >> 1);
        i |= (i >> 2);
        i |= (i >> 4);
        i |= (i >> 8);
        i |= (i >> 16);
        return i - ((unsigned)i >> 1);
#endif
    }

    inline bool contains(int outerFrom, int outerTo, int innerFrom, int innerTo) {
        return innerFrom >= outerFrom && innerTo <= outerTo;
    }

    inline bool intersects(int from1, int to1, int from2, int to2) {
        return (from1 <= from2 && to1 >= from2) || (from1 >= from2 && from1 <= to2);
    }

    inline void change(Node &n, int value) {
        /**
          update target values according to your logic here!
          */
        n.pending = value;
        n.min = value;
        mArray[n.from] = value;
    }

    void propagate(int v) {
        Node &n = mHeap[v];

        if (n.pending) {
            if (n.size() != 1) {
                change(mHeap[2 * v], n.pending);
                change(mHeap[2 * v + 1], n.pending);
            }
            n.pending = 0;
        }
    }

    void update(int v, int from, int to, int value) {
        Node &n = mHeap[v];

        if (contains(from, to, n.from, n.to)) { // node is inside query segment
            change(n, value);
        }

        if (n.size() == 1) {
            return;
        }

        if (intersects(from, to, n.from, n.to)) {
            propagate(v);

            update(2 * v, from, to, value);
            update(2 * v + 1, from, to, value);

            n.min = min(mHeap[2 * v].min, mHeap[2 * v + 1].min);
        }
    }

    int RMQ(int v, int from, int to) {
        Node &n = mHeap[v];
#if 0
        if (n.pending) { // I will propagate down for any request. but it is nor necessary for RMQ
            propagate(v);
        }
#else
        if (n.pending && contains(n.from, n.to, from, to)) {
            return n.pending;
        }
#endif
        if (contains(from, to, n.from, n.to)) { // Node segment is inside of queried segment
            return mHeap[v].min;
        }

        if (intersects(from, to, n.from, n.to)) {
            propagate(v);
            int leftMin = RMQ(2 * v, from, to);
            int rightMin = RMQ(2 * v + 1, from, to);

            return min(leftMin, rightMin);
        }

        // queried segment does not intersect Node segment
        return numeric_limits<int>::max();
    }

    void build(int v, int from, int size) {
        Node &n = mHeap[v];
        n.from = from;
        n.to = from + size - 1;

        if (n.size() == 1) { // leaf initialization
            n.min = mArray[from];
        } else { // init node with childs
            int lSize = prevPow2(size);
            build(2 * v, from, lSize);
            build(2 * v + 1, from + lSize, size - lSize);

            n.min = min(mHeap[2 * v].min, mHeap[2 * v + 1].min);
        }
    }

public:

    SegmentTree(const vector<int> &values) : mHeap(values.size() * 2), mArray(values) {
        mArray = values;
        build(1, 0, mArray.size());
    }

    int RMQ(int from, int to) {
        return RMQ(1, from, to);
    }

    int size() {
        return mArray.size();
    }

    void update(int from, int to, int value) {
        update(1, from, to, value);
    }

    void print() {
        for (int i = 1; i < (int)mHeap.size(); i++) {
            Node &n = mHeap[i];
            cout << i << " :\t[" << n.from << "-" << n.to << "] \tmin: " << n.min << "; \tpending: " << n.pending << endl;
            if (n.size() == 1) {
                cout << "    leaf value: " << mArray[mHeap[i].from] << endl;
            }
        }
    }


};
