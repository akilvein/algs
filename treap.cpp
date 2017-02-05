#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <cassert>

#include "treap.h"

using namespace std;


void test0() {
    Treap<int, double> t1;
    t1.add(6, 0.8);
    //t1.print();
    t1.add(0, 0.0);
    //t1.print();
    t1.add(1, 0.1);
    //t1.print();
    t1.add(4, 0.3);
    //t1.print();
    t1.add(5, 0.2);
    //t1.print();
    t1.add(2, 0.9);
    //t1.print();
    t1.add(3, 0.7);
    //t1.print();

    vector<int> keys = {0, 1, 2, 3, 4, 5, 6};
    vector<double> prior = {0.0, 0.1, 0.9, 0.7, 0.3, 0.2, 0.8};

    Treap<int, double> t2(keys, prior);


    auto t1content = t1.elements();
    auto t2content = t2.elements();
    /*
    for (auto i : t1content) {
        cout << "(" << i.first << ":" << i.second << "); ";
    }
    cout << endl;

    for (auto i : t2content) {
        cout << "(" << i.first << ":" << i.second << "); ";
    }
    cout << endl;*/


    assert(t1content == t2content);

    cout << "creation ok" << endl;
}

void test1() {
    Treap<int, double> t1;
    t1.add(0, 0);
    t1.add(1, 0.1);
    t1.add(2, 0.9);
    t1.add(3, 0.7);
    t1.add(4, 0.3);
    t1.add(5, 0.2);
    t1.add(6, 0.8);

    t1.add(2, 0.2);
    t1.add(2, 0.2);

    vector<int> keys = {0, 1, 2, 2, 2, 3, 4, 5, 6};
    vector<double> prior = {0.0, 0.1, 0.2, 0.2, 0.9, 0.7, 0.3, 0.2, 0.8};

    Treap<int, double> t2(keys, prior);


    auto t1content = t1.elements();
    auto t2content = t2.elements();


    for (auto i : t1content) {
        cout << "(" << i.first << ":" << i.second << "); ";
    }
    cout << endl;

    for (auto i : t2content) {
        cout << "(" << i.first << ":" << i.second << "); ";
    }
    cout << endl;

    t1.print();
    t2.print();

    assert(t1content == t2content);

    cout << "creation with duplicates ok" << endl;
}

/*
void test2() {

    vector<int> keys = {0, 1, 2, 3, 4, 5, 6};
    vector<double> prior = {0.0, 0.1, 0.9, 0.7, 0.3, 0.2, 0.8};

    Treap<int, double> *t1 = Treap<int, double>::build(keys, prior);
    t1 = t1->remove(2);

    auto t1content = t1->elements();


    Treap<int, double> *t2 = new Treap<int, double>(0, 0);
    t2->add(1, 0.1);
    t2->add(3, 0.7);
    t2->add(4, 0.3);
    t2->add(5, 0.2);
    t2->add(6, 0.8);

    auto t2content = t2->elements();


    assert(t1content == t2content);

    cout << "remove ok" << endl;
}


void test3() {
    vector<int> keys = {0, 1, 2, 2, 2, 3, 4, 5, 6};
    vector<double> prior = {0.0, 0.1, 0.2, 0.2, 0.9, 0.7, 0.3, 0.2, 0.8};
    Treap<int, double> *t = Treap<int, double>::build(keys, prior);

    Treap<int, double>::Parts parts = t->split(2);

    auto leftContent = parts.left->elements();
    auto rightContent = parts.right->elements();


    assert(leftContent.size() + rightContent.size() == keys.size());

    parts.left->printFlat();
    parts.right->printFlat();

    for (auto i : leftContent) {
        assert(i.first < 2);
    }

    cout << "split ok" << endl;
}
*/

int main() {

    test0();
    test1();
    //test2();
    //test3();

    cout << "Hello World!" << endl;
    return 0;
}
