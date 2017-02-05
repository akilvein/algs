#ifndef TREAP_H
#define TREAP_H

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <cassert>

namespace std {

template <typename KeyType, typename PriorityType>
class Treap {

public:

    struct Node {

        KeyType key;
        PriorityType priority;

        Node *pLeft;
        Node *pRight;
        Node *pParent;

        size_t size = 1;

        Node(const KeyType &k, const PriorityType &p, Node *left = nullptr, Node *right = nullptr, Node *parent = nullptr) :
            key(k),
            priority(p),
            pLeft(left),
            pRight(right),
            pParent(parent) {

            if (left) {
                size += left->size;
                left->pParent = this;
            }

            if (right) {
                size += right->size;
                right->pParent = this;
            }
        }

        void update(const KeyType &k, const PriorityType &p, Node *left, Node *right, Node *parent) {
            key = k;
            priority = p;
            pLeft = left;
            pRight = right;
            pParent = parent;
            size = 1;

            if (pLeft) {
                size += pLeft->size;
                pLeft->pParent = this;
            }

            if (right) {
                size += pRight->size;
                pRight->pParent = this;
            }
        }

        void update() {
            size = 1;
            pParent = nullptr;

            if (pLeft) {
                size += pLeft->size;
                pLeft->pParent = this;
            }

            if (pRight) {
                size += pRight->size;
                pRight->pParent = this;
            }
        }


        Node *getElementByIndex(size_t i) {
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



        static Node *unite(Node *left, Node *right) {
            if (left == nullptr) {
                return right;
            }

            if (right == nullptr) {
                return left;
            }

            if (left->priority < right->priority) {
                swap(left, right);
            }

            auto rightParts = right->split(left->key);
            Node *newLeft = unite(left->pLeft, rightParts.first);
            Node *newRight = unite(left->pRight, rightParts.second);

            return new Node(left->key, left->priority, newLeft, newRight);
        }




        pair<Node *, Node *> splitByIndex(size_t i) {
            Node *newLeft = nullptr;
            Node *newRight = nullptr;

            size_t leftSize = pLeft ? pLeft->size : 0;

            if (leftSize < i) {

            } else {

            }

            return make_pair(newLeft, newRight);
        }

        Node *remove(const KeyType &k) {
            Node *newLeft = nullptr;
            Node *newRight = nullptr;

            if (k >= key) {
                newLeft = pLeft;
                if (pRight) {
                    newRight = pRight->remove(k);
                }
            } else {
                if (pLeft) {
                    newLeft = pLeft->remove(k);
                }
                newRight = pRight;
            }

            return merge(newLeft, newRight);
        }


        Node *add(const KeyType &k, const PriorityType &p) {
            Node *med = new Node(k, p);

            auto parts = split(k);
            return merge(merge(parts.left, med), parts.right);
        }

        Node *findMin() {
            Node *current = this;

            while (current->pLeft) {
                current = current->pLeft;
            }

            return current;
        }

        Node *findMax() {
            Node *current = this;

            while (current->pRight) {
                current = current->pRight;
            }

            return current;
        }

        Node *next() {
            if (pRight) {
                return pRight->findMin();
            }

            Node *current = this;
            while (current->pParent) {
                if (current->pParent->pRight != current) {
                    return current->pParent;
                }

                current = current->pParent;
            }

            return nullptr;
        }


        void print(int offset = 0) {
            cout << string(offset * 2, ' ');
            cout << "(" << key << " - " << priority << ") size: " << size;
            if (pParent) {
                cout << "; parent: " << pParent->key << endl;
            } else {
                cout << "; parent: NULL" << endl;
            }

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


        vector<pair<KeyType, PriorityType> > elements() {
            vector<pair<KeyType, PriorityType> > result;

            Node *i = findMin();
            while (i) {
                result.push_back(make_pair(i->key, i->priority));
                i = i->next();
            }

            return result;
        }

    };

    static void update(Node *&tree) {
        if (tree) {
            tree->update();
        }
    }

    static Node *build(const vector<KeyType> &keys, const vector<PriorityType> &priorities) {
        assert(keys.size() == priorities.size());

        Node *last = new Node(keys[0], priorities[0]);

        for (size_t i = 1; i < keys.size(); i++) {
            assert(keys[i - 1] <= keys[i]);

            const KeyType &k = keys[i];
            const PriorityType &p = priorities[i];

            if (p < last->priority) {
                last->pRight = new Node(k, p, nullptr, nullptr, last);
                last = last->pRight;
            } else {
                Node *current = last;

                while (current->pParent && current->priority <= p) {
                    current = current->pParent;
                }

                if (current->priority <= p) {
                    last = new Node(k, p, current, nullptr, nullptr);
                } else {
                    last = new Node(k, p, current->pRight, nullptr, current);
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

    static void split(Node *&tree, const KeyType &key, Node *&left, Node *&right) {
        if (!tree) {
            left = right = nullptr;
        } else if (key < tree->key) {
            Node *newLeft;
            split(tree->pLeft, key, left, newLeft);
            right = tree;
            right->pLeft = newLeft;
            update(right);
        } else {
            Node *newRight;
            split(tree->pRight, key, newRight, right);
            left = tree;
            left->pRight = newRight;
            update(left);
        }
    }

    void insertLeft(Node *&tree, Node *node) {
        assert(node->size == 1);

        tree->size++;
        if (tree->pLeft) {
            insert(tree->pLeft, node);
        } else {
            tree->pLeft = node;
            node->pParent = tree;
        }
    }

    void insertRight(Node *&tree, Node *node) {
        assert(node->size == 1);

        tree->size++;
        if (tree->pRight) {
            insert(tree->pRight, node);
        } else {
            tree->pRight = node;
            node->pParent = tree;
        }
    }

    void insert(Node *&tree, Node *node) {
        assert(node->size == 1);

        if (node->priority > tree->priority) {
            Node *newLeft;
            Node *newRight;
            node->pParent = tree->pParent;
            split(tree, node->key, newLeft, newRight);
            tree = node;
            tree->update(node->key, node->priority, newLeft, newRight, node->pParent);
        } else if (node->key <= tree->key) {
            insertLeft(tree, node);
        } else {
            insertRight(tree, node);
        }
    }

    Node *pRoot = nullptr;

public:

    Treap() {}

    Treap(const vector<KeyType> &keys, const vector<PriorityType> &priorities) {
        pRoot = build(keys, priorities);
    }

    void add(const KeyType &k, const PriorityType &p) {
        Node *node = new Node(k, p);

        if (pRoot) {
            insert(pRoot, node);
        } else {
            pRoot = node;
        }
    }

    void print() {
        cout << "============" << endl;
        if (pRoot) {
            pRoot->print();
        }
        cout << "============" << endl;
    }

    vector<pair<KeyType, PriorityType> > elements() {
        return pRoot ? pRoot->elements() : vector<pair<KeyType, PriorityType> >();
    }

};

} // namespace std

#endif // TREAP_H

