#include <iostream>
#include <vector>
using namespace std;


size_t total_memory_used = 0;

template <typename T> class BPlusTree {
public:
    struct Node {
        bool isLeaf;
        vector<T> keys;
        vector<Node*> children;
        Node* next;

        Node(bool leaf = false)
            : isLeaf(leaf), next(nullptr)
        {
            total_memory_used += sizeof(Node);
        }
    };

    Node* root;
    int t;

    BPlusTree(int degree): root(nullptr), t(degree){}

    void insert(T key) {
        if (root == nullptr) {
            root = new Node(true);
            root->keys.push_back(key);
            total_memory_used += sizeof(T);  // key
        } else {
            if (root->keys.size() == 2 * t - 1) {
                Node* newRoot = new Node();
                newRoot->children.push_back(root);
                total_memory_used += sizeof(Node*);
                splitChild(newRoot, 0, root);
                root = newRoot;
            }
            insertNonFull(root, key);
        }
    }

    void insertNonFull(Node* node, T key) {
        if (node->isLeaf) {
auto it = node->keys.begin();
while (it != node->keys.end() && *it < key) ++it;
node->keys.insert(it, key);
            total_memory_used += sizeof(T);
        } else {
            int i = node->keys.size() - 1;
            while (i >= 0 && key < node->keys[i]) i--;
            i++;
            if (node->children[i]->keys.size() == 2 * t - 1) {
                splitChild(node, i, node->children[i]);
                if (key > node->keys[i]) i++;
            }
            insertNonFull(node->children[i], key);
        }
    }

    void splitChild(Node* parent, int index, Node* child) {
        Node* newChild = new Node(child->isLeaf);
        parent->children.insert(parent->children.begin() + index + 1, newChild);
        total_memory_used += sizeof(Node*);

        parent->keys.insert(parent->keys.begin() + index, child->keys[t - 1]);
        total_memory_used += sizeof(T);

        newChild->keys.assign(child->keys.begin() + t, child->keys.end());
        total_memory_used += (child->keys.size() - t) * sizeof(T);
        child->keys.resize(t - 1);

        if (!child->isLeaf) {
            newChild->children.assign(child->children.begin() + t, child->children.end());
            total_memory_used += (child->children.size() - t) * sizeof(Node*);
            child->children.resize(t);
        }

        if (child->isLeaf) {
            newChild->next = child->next;
            child->next = newChild;
        }
    }
};

int main() {
    BPlusTree<int> tree(3);
    for (int u = 0; u < 100000; u++) {
        tree.insert(u);
    }

    cout << "Estimated memory used: " << total_memory_used / (1024.0 * 1024.0) << " MB" << endl;
}
