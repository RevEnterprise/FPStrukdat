#include <iostream>
#include <vector>
#include <string>

using namespace std;

size_t total_memory_used = 0;

template<typename K, typename V>
class HashTable {
private:
    struct Entry {
        K key;
        V value;
        bool isOccupied;

        Entry() : isOccupied(false) {
            total_memory_used += sizeof(Entry);
        }
    };

    vector<Entry> table;
    size_t capacity;
    size_t size;

    size_t hash(K key) {
        return std::hash<K>()(key) % capacity;
    }

public:
    HashTable(size_t cap) : capacity(cap), size(0) {
        table.resize(capacity);
        total_memory_used += sizeof(Entry) * capacity; // approximate
    }

    void insert(K key, V value) {
        size_t idx = hash(key);
        size_t start = idx;
        while (table[idx].isOccupied && table[idx].key != key) {
            idx = (idx + 1) % capacity;
            if (idx == start) {
                cout << "Hash table full\n";
                return;
            }
        }

        if (!table[idx].isOccupied) {
            table[idx].isOccupied = true;
            table[idx].key = key;
            table[idx].value = value;
            size++;
            total_memory_used += sizeof(K) + sizeof(V);  // estimate per item
        } else {
            table[idx].value = value;  // overwrite existing
        }
    }

    bool search(K key, V& out) {
        size_t idx = hash(key);
        size_t start = idx;
        while (table[idx].isOccupied) {
            if (table[idx].key == key) {
                out = table[idx].value;
                return true;
            }
            idx = (idx + 1) % capacity;
            if (idx == start) break;
        }
        return false;
    }

    size_t memoryUsage() const {
        return total_memory_used;
    }
};

int main() {
    HashTable<string, int> ht(10000);

    for (int i = 0; i < 5000; ++i) {
        ht.insert("key" + to_string(i), i);
    }

    cout << "Estimated memory used: "
         << ht.memoryUsage() / (1024.0 * 1024.0)
         << " MB" << endl;

    int val;
    if (ht.search("key1234", val)) {
        cout << "Found key1234: " << val << endl;
    } else {
        cout << "Not found\n";
    }

    return 0;
}
