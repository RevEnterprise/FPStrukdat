#ifndef HASHMAP_H
#define HASHMAP_H

#include <string>
#include <vector>
#include <list>

using namespace std;

struct HashNode {
    string key;
    string value;
};

class HashMap {
private:
    static const int TABLE_SIZE = 100;
    vector<list<HashNode>> table;

    unsigned int u32(const string& str);
    int hashFunction(const string& key);
    size_t estimateMemory() const;

public:
    HashMap();


    void insert(const string& value);
    void create();
    bool search(const string& value);
    void remove(const string& value);
    void update(const string& oldValue, const string& newValue);
    void display();

    friend void loadFromFile(HashMap&, const string&);
    friend void searchWithPreciseTime(HashMap&, const string&);
    friend int totalData(const HashMap&);
    friend void analyzeCases(HashMap&);
    friend size_t estimateMemory(const HashMap&);

};

#include "HashMap.cpp"
#endif

