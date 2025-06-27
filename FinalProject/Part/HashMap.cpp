#include "HashMap.h"
#include <iostream>

using namespace std;

unsigned int HashMap::u32(const string& str) {
    unsigned int hash = 0;
    for (char c : str) {
        hash = hash * 31 + c;
    }
    return hash;
}

int HashMap::hashFunction(const string& key) {
    return u32(key) % TABLE_SIZE;
}

HashMap::HashMap() {
    table.resize(TABLE_SIZE);
}

void HashMap::insert(const string& value) {
    string key = to_string(u32(value));
    int idx = hashFunction(key);

    for (auto& node : table[idx]) {
        if (node.key == key) {
            cout << "Data Already Exist!\n";
            return;
        }
    }
    table[idx].push_back({ key, value });
}

void HashMap::create() {
    table.clear();
    table.resize(TABLE_SIZE);
}

bool HashMap::search(const string& value) {
    string key = to_string(u32(value));
    int idx = hashFunction(key);

    for (const auto& node : table[idx]) {
        if (node.key == key && node.value == value) {
            return true;
        }
    }
    return false;
}

void HashMap::remove(const string& value) {
    string key = to_string(u32(value));
    int idx = hashFunction(key);

    for (auto it = table[idx].begin(); it != table[idx].end(); ++it) {
        if (it->key == key && it->value == value) {
            table[idx].erase(it);
            cout << "Data has been removed.\n";
            return;
        }
    }
    cout << "Data Not Found.\n";
}

void HashMap::update(const string& oldValue, const string& newValue) {
    string key = to_string(u32(oldValue));
    int idx = hashFunction(key);

    for (auto& node : table[idx]) {
        if (node.key == key && node.value == oldValue) {
            node.value = newValue;
            node.key = to_string(u32(newValue));
            cout << "Data has been updated to : " << newValue << endl;
            return;
        }
    }
    cout << "Data Not Found.\n";
}

void HashMap::display() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        for (const auto& node : table[i]) {
            cout << "[" << node.key << "] " << node.value << endl;
        }
    }
}

