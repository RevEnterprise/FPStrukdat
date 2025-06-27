#ifndef BPLUS_H
#define BPLUS_H

#include <vector>
#include <string>

using namespace std;

template <typename T> class BPlusTree {
public:
    struct Node {
        bool isLeaf;
        vector<T> keys;
        vector<Node*> children;
        Node* next;

        Node(bool leaf = false)
            : isLeaf(leaf)
            , next(nullptr)
        {
        }
    };

    Node* root;
    int t;

    void splitChild(Node* parent, int index, Node* child);
    void insertNonFull(Node* node, T key);
    void remove(Node* node, T key);
    void borrowFromPrev(Node* node, int index);
    void borrowFromNext(Node* node, int index);
    void merge(Node* node, int index);
    void printTree(Node* node, int level);

public:
    BPlusTree(int degree): root(nullptr), t(degree){}
    pair<Node*, typename vector<T>::iterator> find(T key);


    size_t estimateMemory();

    void insert(T key);
    bool search(T key);
    void remove(T key);
    vector<T> rangeQuery(T lower, T upper);
    void printTree();
};


#include "Bplus.tpp"  // penting! untuk implementasi template
#endif
