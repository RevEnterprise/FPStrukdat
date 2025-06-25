// Bplus.tpp
#include <iostream>
#include <algorithm>
#include <functional>


using namespace std;


// Implementation of splitChild function
template <typename T>
void BPlusTree<T>::splitChild(Node* parent, int index,
                              Node* child)
{
    Node* newChild = new Node(child->isLeaf);
    parent->children.insert(
        parent->children.begin() + index + 1, newChild);
    parent->keys.insert(parent->keys.begin() + index,
                        child->keys[t - 1]);

    newChild->keys.assign(child->keys.begin() + t,
                          child->keys.end());
    child->keys.resize(t - 1);

    if (!child->isLeaf) {
        newChild->children.assign(child->children.begin()
                                      + t,
                                  child->children.end());
        child->children.resize(t);
    }

    if (child->isLeaf) {
        newChild->next = child->next;
        child->next = newChild;
    }
}

// Implementation of insertNonFull function
template <typename T>
void BPlusTree<T>::insertNonFull(Node* node, T key)
{
    if (node->isLeaf) {
        node->keys.insert(upper_bound(node->keys.begin(),
                                      node->keys.end(),
                                      key),
                          key);
    }
    else {
        int i = node->keys.size() - 1;
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;
        if (node->children[i]->keys.size() == 2 * t - 1) {
            splitChild(node, i, node->children[i]);
            if (key > node->keys[i]) {
                i++;
            }
        }
        insertNonFull(node->children[i], key);
    }
}

// Implementation of remove function
template <typename T>
void BPlusTree<T>::remove(Node* node, T key)
{
    // If node is a leaf
    if (node->isLeaf) {
        auto it = find(node->keys.begin(), node->keys.end(),
                       key);
        if (it != node->keys.end()) {
            node->keys.erase(it);
        }
    }
    else {
        int idx = lower_bound(node->keys.begin(),
                              node->keys.end(), key)
                  - node->keys.begin();
        if (idx < node->keys.size()
            && node->keys[idx] == key) {
            if (node->children[idx]->keys.size() >= t) {
                Node* predNode = node->children[idx];
                while (!predNode->isLeaf) {
                    predNode = predNode->children.back();
                }
                T pred = predNode->keys.back();
                node->keys[idx] = pred;
                remove(node->children[idx], pred);
            }
            else if (node->children[idx + 1]->keys.size()
                     >= t) {
                Node* succNode = node->children[idx + 1];
                while (!succNode->isLeaf) {
                    succNode = succNode->children.front();
                }
                T succ = succNode->keys.front();
                node->keys[idx] = succ;
                remove(node->children[idx + 1], succ);
            }
            else {
                merge(node, idx);
                remove(node->children[idx], key);
            }
        }
        else {
            if (node->children[idx]->keys.size() < t) {
                if (idx > 0
                    && node->children[idx - 1]->keys.size()
                           >= t) {
                    borrowFromPrev(node, idx);
                }
                else if (idx < node->children.size() - 1
                         && node->children[idx + 1]
                                    ->keys.size()
                                >= t) {
                    borrowFromNext(node, idx);
                }
                else {
                    if (idx < node->children.size() - 1) {
                        merge(node, idx);
                    }
                    else {
                        merge(node, idx - 1);
                    }
                }
            }
            remove(node->children[idx], key);
        }
    }
}

// Implementation of borrowFromPrev function
template <typename T>
void BPlusTree<T>::borrowFromPrev(Node* node, int index)
{
    Node* child = node->children[index];
    Node* sibling = node->children[index - 1];

    child->keys.insert(child->keys.begin(),
                       node->keys[index - 1]);
    node->keys[index - 1] = sibling->keys.back();
    sibling->keys.pop_back();

    if (!child->isLeaf) {
        child->children.insert(child->children.begin(),
                               sibling->children.back());
        sibling->children.pop_back();
    }
}

// Implementation of borrowFromNext function
template <typename T>
void BPlusTree<T>::borrowFromNext(Node* node, int index)
{
    Node* child = node->children[index];
    Node* sibling = node->children[index + 1];

    child->keys.push_back(node->keys[index]);
    node->keys[index] = sibling->keys.front();
    sibling->keys.erase(sibling->keys.begin());

    if (!child->isLeaf) {
        child->children.push_back(
            sibling->children.front());
        sibling->children.erase(sibling->children.begin());
    }
}

// Implementation of merge function
template <typename T>
void BPlusTree<T>::merge(Node* node, int index)
{
    Node* child = node->children[index];
    Node* sibling = node->children[index + 1];

    child->keys.push_back(node->keys[index]);
    child->keys.insert(child->keys.end(),
                       sibling->keys.begin(),
                       sibling->keys.end());
    if (!child->isLeaf) {
        child->children.insert(child->children.end(),
                               sibling->children.begin(),
                               sibling->children.end());
    }

    node->keys.erase(node->keys.begin() + index);
    node->children.erase(node->children.begin() + index
                         + 1);

    delete sibling;
}

// Implementation of printTree function
template <typename T>
void BPlusTree<T>::printTree(Node* node, int level)
{
    if (node != nullptr) {
        for (int i = 0; i < level; ++i) {
            cout << "  ";
        }
        for (const T& key : node->keys) {
            cout << key << " ";
        }
        cout << endl;
        for (Node* child : node->children) {
            printTree(child, level + 1);
        }
    }
}

// Implementation of printTree wrapper function
template <typename T> void BPlusTree<T>::printTree()
{
    printTree(root, 0);
}

// Implementation of search function
template <typename T> bool BPlusTree<T>::search(T key)
{
    Node* current = root;
    while (current != nullptr) {
        int i = 0;
        while (i < current->keys.size()
               && key > current->keys[i]) {
            i++;
        }
        if (i < current->keys.size()
            && key == current->keys[i]) {
            return true;
        }
        if (current->isLeaf) {
            return false;
        }
        current = current->children[i];
    }
    return false;
}
// Impelemtasi find function
template <typename T>
pair<typename BPlusTree<T>::Node*, typename vector<T>::iterator> BPlusTree<T>::find(T key)
{
    Node* current = root;
    while (current != nullptr) {
        auto it = lower_bound(current->keys.begin(), current->keys.end(), key);
        if (it != current->keys.end() && *it == key) {
            return {current, it};
        }
        if (current->isLeaf) {
            break;
        }
        int index = it - current->keys.begin();
        current = current->children[index];
    }
    return {nullptr, typename vector<T>::iterator()};
}



// Implementation of range query function
template <typename T>
vector<T> BPlusTree<T>::rangeQuery(T lower, T upper)
{
    vector<T> result;
    Node* current = root;
    while (!current->isLeaf) {
        int i = 0;
        while (i < current->keys.size()
               && lower > current->keys[i]) {
            i++;
        }
        current = current->children[i];
    }
    while (current != nullptr) {
        for (const T& key : current->keys) {
            if (key >= lower && key <= upper) {
                result.push_back(key);
            }
            if (key > upper) {
                return result;
            }
        }
        current = current->next;
    }
    return result;
}

// Implementation of insert function
template <typename T> void BPlusTree<T>::insert(T key)
{
    if (root == nullptr) {
        root = new Node(true);
        root->keys.push_back(key);
    }
    else {
        if (root->keys.size() == 2 * t - 1) {
            Node* newRoot = new Node();
            newRoot->children.push_back(root);
            splitChild(newRoot, 0, root);
            root = newRoot;
        }
        insertNonFull(root, key);
    }
}

// Implementation of remove function
template <typename T> void BPlusTree<T>::remove(T key)
{
    if (root == nullptr) {
        return;
    }
    remove(root, key);
    if (root->keys.empty() && !root->isLeaf) {
        Node* tmp = root;
        root = root->children[0];
        delete tmp;
    }
}

// bisa di pelajari ini
template <typename T> size_t BPlusTree<T>::estimateMemory() {
    size_t total = 0;

    // Fungsi rekursif untuk menjelajahi semua node
    function<void(Node*)> traverse = [&](Node* node) {
        if (!node) return;

        // Estimasi ukuran Node itu sendiri
        total += sizeof(Node);

        // Estimasi keys
        for (const T& key : node->keys) {
            total += sizeof(T);               // ukuran objek T (di sini string)
            total += key.capacity();          // kapasitas memori dalam string
        }

        // Estimasi pointer anak-anak
        total += sizeof(Node*) * node->children.capacity();

        // Traverse ke semua anak
        for (Node* child : node->children) {
            traverse(child);
        }
    };

    traverse(root);
    return total;
}


