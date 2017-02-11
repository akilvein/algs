#ifndef TREAP_H
#define TREAP_H

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <cassert>

namespace std {

class TreapTester;

template <typename KeyType, typename PriorityType>
class Treap {
    friend class TreapTester;

    struct Node {
        KeyType key;
        PriorityType priority;

        Node *left = nullptr;
        Node *right = nullptr;
        size_t size = 1;

        Node(KeyType k, PriorityType p) : key(k), priority(p) {
        }

        static size_t getSize(Node *n) {
            return n ? n->size : 0;
        }

        void recalc() {
            assert(this);
            size = 1 + getSize(left) + getSize(right);
        }

        static void split(Node *tree, KeyType key, Node *&left, Node *&right) {
            if (!tree) {
                left = nullptr;
                right = nullptr;
            } else if (key < tree->key) {
                split(tree->left, key, left, tree->left);
                assert(tree);
                right = tree;
                right->recalc();
                assert(left != right);
            } else {
                split(tree->right, key, tree->right, right);
                assert(tree);
                left = tree;
                left->recalc();
                assert(left != right);
            }
        }

        static void insert(Node *&tree, Node *item) {
            if (!tree) {
                tree = item;
            } else if (item->priority > tree->priority) {
                split(tree, item->key, item->left, item->right);
                tree = item;
            } else {
                insert(item->key < tree->key ? tree->left : tree->right, item);
            }

            tree->recalc();
        }

        static void merge(Node *&tree, Node *left, Node *right) {
            if (!left) {
                tree = right;
            } else if (!right) {
                tree = left;
            } else if (left->priority > right->priority) {
                assert(left->key < right->key);
                merge(left->right, left->right, right);
                tree = left;
                assert(tree);
                tree->recalc();
            } else {
                assert(left->key < right->key);
                merge(right->left, left, right->left);
                tree = right;
                assert(tree);
                tree->recalc();
            }
        }

        static void remove(Node *&tree, KeyType key) {
            if (!tree) {
                return;
            }

            if (tree->key == key) {
                merge(tree, tree->left, tree->right);
            } else {
                remove(key < tree->key ? tree->left : tree->right, key);
                if (tree) {
                    tree->recalc();
                }
            }
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

    Node *mpRoot = nullptr;

public:
    void insert(KeyType k, PriorityType p) {
        Node::insert(mpRoot, new Node(k, p));
    }

    void remove(KeyType key) {
        Node::remove(mpRoot, key);
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

        t.remove(0);
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

    static void main() {
        t1();
        t2();

        cout << "cool!" << endl;
    }
};

} // namespace std

#endif // TREAP_H

