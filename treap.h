#ifndef TREAP_H
#define TREAP_H

#include <iostream>
#include <vector>
#include <cassert>
#include <memory>

namespace std {

class TreapTester;

template <typename KeyType, typename PriorityType>
class Treap {
    friend class TreapTester;

    struct Node {
        KeyType key;
        PriorityType priority;

        unique_ptr<Node> left;
        unique_ptr<Node> right;
        size_t size = 1;

        Node(KeyType k, PriorityType p) : key(k), priority(p) {
        }

        static size_t getSize(const unique_ptr<Node> &n) {
            return n ? n->size : 0;
        }

        void recalc() {
            assert(this);
            size = 1 + getSize(left) + getSize(right);
        }

        static void split(unique_ptr<Node> tree, KeyType key, unique_ptr<Node> &left, unique_ptr<Node> &right) {
            if (!tree) {
                left = nullptr;
                right = nullptr;
            } else if (key < tree->key) {
                split(move(tree->left), key, left, tree->left);
                assert(tree);
                right = move(tree);
                right->recalc();
                assert(left != right);
            } else {
                split(move(tree->right), key, tree->right, right);
                assert(tree);
                left = move(tree);
                left->recalc();
                assert(left != right);
            }
        }

        static void splitRight(unique_ptr<Node> tree, KeyType key, unique_ptr<Node> &left, unique_ptr<Node> &right) {
            if (!tree) {
                left = nullptr;
                right = nullptr;
            } else if (key <= tree->key) {
                splitRight(move(tree->left), key, left, tree->left);
                assert(tree);
                right = move(tree);
                right->recalc();
                assert(left != right);
            } else {
                splitRight(move(tree->right), key, tree->right, right);
                assert(tree);
                left = move(tree);
                left->recalc();
                assert(left != right);
            }
        }

        static void insert(unique_ptr<Node> &tree, unique_ptr<Node> &item) {
            if (!tree) {
                tree = move(item);
            } else if (item->priority > tree->priority) {
                split(move(tree), item->key, item->left, item->right);
                tree = move(item);
            } else {
                insert(item->key <= tree->key ? tree->left : tree->right, item);
            }

            tree->recalc();
        }

        static void merge(unique_ptr<Node> &tree, unique_ptr<Node> left, unique_ptr<Node> right) {
            if (!left) {
                tree = move(right);
            } else if (!right) {
                tree = move(left);
            } else if (left->priority > right->priority) {
                assert(left->key <= right->key);
                merge(left->right, move(left->right), move(right));
                tree = move(left);
                assert(tree);
                tree->recalc();
            } else {
                assert(left->key <= right->key);
                merge(right->left, move(left), move(right->left));
                tree = move(right);
                assert(tree);
                tree->recalc();
            }
        }

        static unique_ptr<Node> remove(unique_ptr<Node> &tree, KeyType key) {
            if (!tree) {
                return nullptr;
            }

            if (tree->key == key) {
                auto removedNode = move(tree);
                merge(tree, move(removedNode->left), move(removedNode->right));
                return removedNode;
            }

            auto result = remove(key < tree->key ? tree->left : tree->right, key);
            if (tree) {
                tree->recalc();
            }
            return result;
        }

        static unique_ptr<Node> removeRange(unique_ptr<Node> &tree, KeyType key) {
            if (!tree) {
                return nullptr;
            }

            unique_ptr<Node> l;
            unique_ptr<Node> r;
            unique_ptr<Node> m;

            split(move(tree), key, l, r);
            splitRight(move(l), key, l, m);

            merge(tree, move(l), move(r));

            return m;
        }

        Node *getByIndex(size_t i) {
            assert(i < size);

            size_t leftSize = getSize(left);

            if (i < leftSize) {
                return left->getByIndex(i);
            }

            if (i == leftSize) {
                return this;
            }

            assert(right);
            return right->getByIndex(i - 1 - leftSize);
        }

        void printFlat() {
            if (left) {
                left->printFlat();
            }

            cout << "(" << key << ":" << priority << ") ";

            if (right) {
                right->printFlat();
            }
        }

        void print(int offset = 0) {
            cout << string(offset * 4, ' ');
            cout << "(" << key << ":" << priority << ") - " << size << endl;
            if (left) {
                left->print(offset + 1);
            } else {
                cout << string(offset * 4 + 4, ' ') << "null" << endl;
            }
            if (right) {
                right->print(offset + 1);
            } else {
                cout << string(offset * 4 + 4, ' ') << "null" << endl;
            }
        }
    };

    unique_ptr<Node> mpRoot;

public:

    void insert(KeyType k, PriorityType p) {
        auto i = unique_ptr<Node>(new Node(k, p));
        Node::insert(mpRoot, i);
    }

    bool remove(KeyType key) {
        return Node::remove(mpRoot, key).get();
    }

    bool removeRange(KeyType key) {
        return Node::removeRange(mpRoot, key).get();
    }

    size_t size() const {
        return Node::getSize(mpRoot);
    }

    bool isEmpty() {
        return Node::getSize(mpRoot) == 0;
    }

    KeyType operator[](size_t i) const {
        assert(i < size());
        assert(mpRoot);
        Node *n = mpRoot->getByIndex(i);

        return n->key;
    }
};


class TreapTester {

    template <typename KeyType, typename PriorityType>
    static void printFlat(const Treap<KeyType, PriorityType> &t) {
        if (t.mpRoot) {
            t.mpRoot->printFlat();
            cout << endl;
        }
    }

    template <typename KeyType, typename PriorityType>
    static void print(const Treap<KeyType, PriorityType> &t) {
        cout << "================" << endl;
        if (t.mpRoot) {
            t.mpRoot->print();
        }
        cout << "================" << endl;
    }

    template <typename KeyType, typename PriorityType>
    static void getKeys(const unique_ptr<typename Treap<KeyType, PriorityType>::Node> &n, vector<KeyType> &out) {
        if (!n) {
            return;
        }

        getKeys<KeyType, PriorityType>(n->left, out);

        out.push_back(n->key);

        getKeys<KeyType, PriorityType>(n->right, out);
    }

    template <typename KeyType, typename PriorityType>
    static vector<KeyType> getKeys(const Treap<KeyType, PriorityType> &t) {
        vector<KeyType> out;
        getKeys<KeyType, PriorityType>(t.mpRoot, out);
        return out;
    }

public:
    static void t1() {
        Treap<int, int> t;
        assert(t.isEmpty());
        assert(t.size() == 0);

        t.insert(1, 1);
        assert(t.size() == 1);
        t.insert(9, 0);
        assert(t.size() == 2);
        t.insert(5, 4);
        assert(t.size() == 3);
        t.insert(8, 0);
        assert(t.size() == 4);
        t.insert(4, 9);
        assert(t.size() == 5);

        //print(t);
        //printFlat(t);

        assert(t[0] == 1);
        assert(t[1] == 4);
        assert(t[2] == 5);
        assert(t[3] == 8);
        assert(t[4] == 9);

        assert(!t.remove(0));
        assert(t.size() == 5);

        t.remove(1);
        assert(t[0] == 4);
        assert(t[1] == 5);
        assert(t[2] == 8);
        assert(t[3] == 9);

        t.remove(8);
        assert(t.size() == 3);
        assert(t[0] == 4);
        assert(t[1] == 5);
        assert(t[2] == 9);

        cout << "t1 passed" << endl;
    }

    static double median(const Treap<int, int> &t) {
        size_t size = t.size();
        if (size % 2 == 1) {
            return t[size / 2];
        } else {
            double result = 0;
            result += (double(t[size / 2 - 1]) / 2);
            result += (double(t[size / 2]) / 2);
            return result;
        }
        assert(false);
        return 0;
    }

    static void t2() {
        srand(time(0));

        Treap<int, int> t;

        t.insert(1, rand());
        assert(median(t) == 1);

        t.insert(2, rand());
        assert(median(t) == 1.5);

        t.insert(1, rand());
        assert(median(t) == 1);

        t.remove(1);
        assert(median(t) == 1.5);

        t.remove(2);
        assert(median(t) == 1);

        t.remove(1);
        assert(t.isEmpty());

        cout << "t2 passed" << endl;
    }

    static void t3() {
        Treap<int, int> t;

        t.insert(1, rand());
        t.insert(2, rand());
        t.insert(3, rand());
        t.insert(4, rand());
        t.insert(5, rand());
        t.insert(6, rand());
        t.insert(7, rand());
        t.insert(8, rand());

        t.insert(3, rand());
        t.insert(3, rand());
        t.insert(3, rand());
        t.insert(3, rand());

        assert(t.size() == 12);

        unique_ptr<Treap<int, int>::Node> l;
        unique_ptr<Treap<int, int>::Node> r;

        Treap<int, int>::Node::split(move(t.mpRoot), 3, l, r);

        vector<int> lKeys;
        getKeys<int, int>(l, lKeys);
        assert(lKeys == vector<int>({1, 2, 3, 3, 3, 3, 3}));

        vector<int> rKeys;
        getKeys<int, int>(r, rKeys);
        assert(rKeys == vector<int>({4, 5, 6, 7, 8}));

        cout << "t3 passed" << endl;
    }

    static void t4() {
        Treap<int, int> t;

        t.insert(1, rand());
        t.insert(2, rand());
        t.insert(3, rand());
        t.insert(4, rand());
        t.insert(5, rand());
        t.insert(6, rand());
        t.insert(7, rand());
        t.insert(8, rand());

        t.insert(3, rand());
        t.insert(3, rand());
        t.insert(3, rand());
        t.insert(3, rand());

        assert(t.size() == 12);

        unique_ptr<Treap<int, int>::Node> l;
        unique_ptr<Treap<int, int>::Node> r;

        Treap<int, int>::Node::splitRight(move(t.mpRoot), 3, l, r);

        vector<int> lKeys;
        getKeys<int, int>(l, lKeys);

        assert(lKeys == vector<int>({1, 2}));

        vector<int> rKeys;
        getKeys<int, int>(r, rKeys);
        assert(rKeys == vector<int>({3, 3, 3, 3, 3, 4, 5, 6, 7, 8}));

        cout << "t4 passed" << endl;
    }

    static void t5() {
        Treap<int, int> t;

        t.insert(3, rand());
        t.insert(4, rand());
        t.insert(5, rand());
        t.insert(1, rand());
        t.insert(2, rand());
        t.insert(6, rand());
        t.insert(3, rand());
        t.insert(3, rand());
        t.insert(8, rand());
        t.insert(7, rand());
        t.insert(3, rand());
        t.insert(3, rand());

        assert(t.size() == 12);

        t.removeRange(3);
        vector<int> keys = getKeys(t);
        assert(keys.size() == 7);

        assert(keys == vector<int>({1, 2, 4, 5, 6, 7, 8}));

        cout << "t5 passed" << endl;
    }


    static void main() {
        t1();
        t2();
        t3();
        t4();
        t5();

        cout << "cool!" << endl;
    }
};

} // namespace std

#endif // TREAP_H

