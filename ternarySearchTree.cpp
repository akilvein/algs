#include <iostream>
#include <cstring>
#include <string>
#include <cassert>

using namespace std;

class TST {
    struct Node {
        char c;

        Node *left = nullptr;
        Node *mid = nullptr;
        Node *right = nullptr;

        bool set = false;

#if 1
        ~Node() {
            delete left;
            delete mid;
            delete right;
        }
#endif
    };

    int mSize = 0;
    Node *mRoot = nullptr;

    Node *get(Node *n, const char *key, int keyLen) {
        if (!keyLen || !n) {
            return nullptr;
        }

        char c = *key;

        if (c < n->c) {
            return get(n->left, key, keyLen);
        } else if (c > n->c) {
            return get(n->right, key, keyLen);
        } else if (keyLen > 1) {
            return get(n->mid, key + 1, keyLen - 1);
        } else {
            return n;
        }
    }

    Node *put(Node *n, const char *key, int keyLen) {
        if (!keyLen) {
            return nullptr;
        }

        char c = *key;

        if (!n) {
            n = new Node();
            n->c = c;
        }

        if (c < n->c) {
            n->left = put(n->left, key, keyLen);
        } else if (c > n->c) {
            n->right = put(n->right, key, keyLen);
        } else if (keyLen > 1) {
            n->mid = put(n->mid, key + 1, keyLen - 1);
        } else {
            n->set = true;
        }

        return n;
    }

public:
    ~TST() {
        delete mRoot;
    }

    int size() {
        return mSize;
    }

    bool contains(const char *key) {
        Node *n = get(mRoot, key, (int)strlen(key));
        if (n && n->set) {
            return true;
        }

        return false;
    }

    void put(const char *key) {

        if (!contains(key)) {
            mSize++;
        }

        mRoot = put(mRoot, key, (int)strlen(key));
    }

};

int main()
{
    cout << "Hello World!" << endl;

    TST tst;

    tst.put("");
    tst.put("abra");
    tst.put("abraca");
    tst.put("abrama");

    assert(tst.contains("abra"));
    assert(tst.contains("abraca"));
    assert(tst.contains("abrama"));

    assert(!tst.contains("abr"));
    assert(!tst.contains("abrac"));
    assert(!tst.contains("abramart"));


    return 0;
}
