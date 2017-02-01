#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <cassert>


using namespace std;


template <typename KeyType, typename PriorityType>
struct Treap {

    KeyType key;
    PriorityType priority;

    Treap *pLeft;
    Treap *pRight;
    Treap *pParent;

    size_t size;

    Treap(const KeyType &k, const PriorityType &p, Treap *left = nullptr, Treap *right = nullptr, Treap *parent = nullptr) :
        key(k),
        priority(p),
        pLeft(left),
        pRight(right),
        pParent(parent),
        size(1) {
        if (left) {
            size += left->size;
        }
        if (right) {
            size += right->size;
        }
    }

    ~Treap() {
        delete pLeft;
        delete pRight;
    }

    Treap *getElementByIndex(size_t i) {
        assert(i < size);

        size_t leftSize = pLeft ? pLeft->size : 0;

        if (i == leftSize) {
            return this;
        }

        if (i < leftSize) {
            assert(pLeft);
            return pLeft->getElementByIndex(i);
        }

        assert(pRight);
        return pRight->getElementByIndex(i - leftSize - 1);
    }

    size_t recursiveCalcSize() {
        size = 1;

        if (pLeft) {
            size += pLeft->recursiveCalcSize();
        }

        if (pRight) {
            size += pRight->recursiveCalcSize();
        }

        return size;
    }

    static Treap *merge(Treap *left, Treap *right) {
        if (left == nullptr) {
            return right;
        }

        if (right == nullptr) {
            return left;
        }

        if (left->priority > right->priority) {
            Treap *newRight = merge(left->pRight, right);
            return new Treap(left->key, left->priority, left->pLeft, newRight);
        } else {
            Treap *newLeft = merge(left, right->pLeft);
            return new Treap(right->key, right->priority, newLeft, right->pRight);
        }

    }


    pair<Treap *, Treap *> split(const KeyType &k) {
        Treap *newLeft = nullptr;
        Treap *newRight = nullptr;

        if (k >= key) {
            pair<Treap *, Treap *> rightParts(nullptr, nullptr);

            if (pRight) {
                rightParts = pRight->split(k);
            }

            newLeft = new Treap(key, priority, pLeft, rightParts.first);
            newRight = rightParts.second;
        } else {
            pair<Treap *, Treap *> leftParts(nullptr, nullptr);
            if (pLeft) {
                leftParts = pLeft->split(k);
            }

            newLeft = leftParts.first;
            newRight = new Treap(key, priority, leftParts.second, pRight);
        }

        return make_pair(newLeft, newRight);
    }

    pair<Treap *, Treap *> splitByIndex(size_t i) {
        Treap *newLeft = nullptr;
        Treap *newRight = nullptr;

        size_t leftSize = pLeft ? pLeft->size : 0;

        if (leftSize < i) {

        } else {

        }

        return make_pair(newLeft, newRight);
    }


    Treap *add(const KeyType &k, const PriorityType &p) {
        Treap *med = new Treap(k, p);

        auto parts = split(k);
        return merge(merge(parts.first, med), parts.second);
    }

    static Treap *build(const vector<KeyType> &keys, const vector<PriorityType> &priorities) {
        Treap *last = new Treap(keys[0], priorities[0]);

        for (size_t i = 1; i < keys.size(); i++) {
            const KeyType &k = keys[i];
            const PriorityType &p = priorities[i];

            if (p < last->priority) {
                last->pRight = new Treap(k, p, nullptr, nullptr, last);
                last = last->pRight;
            } else {
                Treap *current = last;

                while (current->pParent && current->priority <= p) {
                    current = current->pParent;
                }

                if (current->priority <= p) {
                    last = new Treap(k, p, current, nullptr, nullptr);
                } else {
                    last = new Treap(k, p, current->pRight, nullptr, current);
                    current->pRight = last;
                }

            }
        }

        while (last->pParent) {
            last = last->pParent;
        }

        last->recursiveCalcSize();

        return last;
    }


    void print(int offset = 0) {
        cout << string(offset * 2, ' ');
        cout << size << " : " << key << " - " << priority << endl;

        if (pLeft) {
            pLeft->print(offset + 1);
        } else {
            cout << string(offset * 2 + 2, ' ') << "-----" << endl;
        }

        if (pRight) {
            pRight->print(offset + 1);
        } else {
            cout << string(offset * 2 + 2, ' ') << "-----" << endl;
        }

    }

};



int main() {
    Treap<int, double> *t = new Treap<int, double>(0, 0);

    t = t->add(1, 0.1);
    t = t->add(2, 0.9);
    t = t->add(3, 0.7);
    t = t->add(4, 0.3);
    t = t->add(5, 0.2);
    t = t->add(6, 0.8);

    //t->print();
    //cout << "=============" << endl;

    vector<int> keys = {0, 1, 2, 3, 4, 5, 6};
    vector<double> prior = {0.0, 0.1, 0.9, 0.7, 0.3, 0.2, 0.8};

    Treap<int, double> *t2 = Treap<int, double>::build(keys, prior);

    t2->print();

    cout << t2->getElementByIndex(5)->key << endl;

    cout << "Hello World!" << endl;
    return 0;
}

