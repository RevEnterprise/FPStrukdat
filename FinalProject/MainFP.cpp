#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <chrono>
#include <vector>
#include <list>
#include <functional>
#include <iomanip>
#include <climits>

#include "Part/HashMap.h"
#include "Part/Bplus.h"
using namespace std;

void ChildSetting(int& child);  // Function prototype
void DataFiles(string& path); // set new path
void Update(int child, BPlusTree<string>& tree, string path, HashMap& map); // for updating
void CompareSearch(BPlusTree<string>& tree, string path, HashMap& mapht);
void sampledata(BPlusTree<string>& tree, string path, string& first, string& middle, string& last, string& best, string& worst, HashMap& map);
void datasearch(BPlusTree<string>& tree, string value);
void CompareInsertDelete(BPlusTree<string>& tree, HashMap& map);
string getRightmostKey(BPlusTree<string>& tree);

void loadFromFile(HashMap& map, const string& path);
size_t estimateMemory(const HashMap& map);
void searchWithPreciseTime(HashMap& map, const string& value);
void analyzeCases(HashMap& map);
int totalData(const HashMap& map);


int main() {
    int child = 3;
    int CekData = 0;
    int pilihan;
    cout << "WELCOME TO THE COMPARE PROGRAM\n";
    string path = "Data/nama_100_orang.txt";
    BPlusTree<string> tree(child);
    HashMap map;
    do {
        cout << "\nMenu:\n";
        cout << "1. Setting\n";
        cout << "2. DataFiles\n";
        cout << "3. Data Structure Update\n";
        cout << "4. CompareSearch\n";
        cout << "5. Print\n";
        cout << "6. CompareInsertDelete\n";
        cout << "7. Exist\n";
        cout << "Chosee menu: ";
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1: ChildSetting(child); break;
            case 2: DataFiles(path); break;
            case 3: Update(child, tree, path, map); break;
            case 4: CompareSearch(tree, path, map); break;
            case 5: {
                if (tree.root == nullptr || tree.root->keys.empty()) {
                    cout << "Tree Empty." << endl;
                    break;
                } else {
                    cout << "\nB+ Tree Data:\n";
                    tree.printTree();
                    cout << "\nHashMap Data:\n";
                    map.display();
                }
                break;
            }
            case 6: CompareInsertDelete(tree, map); break;
            case 7: printf("Existing the Program...\n"); break;
            default: printf("Option Not valid.\n");
        }
    } while (pilihan != 7);


    return 0;
}

void CompareInsertDelete(BPlusTree<string>& tree, HashMap& map) {
    cout << "\n---------- Compare Insert & Delete ----------" << endl;
    cout << "1. Compare Insert\n";
    cout << "2. Compare Delete\n";
    cout << "3. Compare Update\n";
    cout << "Choose option (1/2/3): ";
    string pilihan;
    cin >> pilihan;
    cin.ignore(); // buat jaga jaga

    string input;

    if (pilihan == "1") {
        cout << "Input the data you want to insert: ";
        getline(cin, input);

        if (tree.search(input)) {
            cout << input << " does not exist.\n";
        } else {
            // hashmap
            auto start = chrono::high_resolution_clock::now();
            map.insert(input);
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::duration<double, micro>>(end - start).count();
            cout << "Data successfully inserted in hashmap.\n";
            cout << "Time needed insert data in Hashmap (Chrono) :" << duration << " 탎\n";
            // B+
            auto start_bpt = chrono::high_resolution_clock::now();
            tree.insert(input);
            auto end_bpt = chrono::high_resolution_clock::now();
            auto dur_bpt = chrono::duration_cast<chrono::microseconds>(end_bpt - start_bpt).count();
            cout << "Data successfully inserted.\n";
            cout << "Time needed insert data in B+ tree (Chrono) :" << dur_bpt << " 탎\n";
        }

    } else if (pilihan == "2") {
        cout << "Input the data you want to remove: ";
        getline(cin, input);

        if (!map.search(input) && !tree.search(input)) {
            cout << input << " does not exist.\n";
        } else {
            // hashmap
            auto start = chrono::high_resolution_clock::now();
            map.remove(input);
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::duration<double, micro>>(end - start).count();
            cout << "Data successfully removed in hashmap.\n";
            cout << "Time needed remove data in Hashmap (Chrono) :" << duration << " 탎\n";
            // B+
            auto start_bpt = chrono::high_resolution_clock::now();
            tree.remove(input);
            auto end_bpt = chrono::high_resolution_clock::now();
            auto dur_bpt = chrono::duration_cast<chrono::microseconds>(end_bpt - start_bpt).count();
            cout << "Data successfully removed in b+ tree.\n";
            cout << "Time needed remove data in B+ tree (Chrono) :" << dur_bpt << " 탎\n";
        }

    } else if (pilihan == "3") {
        cout << "Input the old data: ";
        getline(cin, input);
        string newinput;
        cout << "Input the new data: ";
        getline(cin, newinput);

        auto start = chrono::high_resolution_clock::now();
        map.update(input, newinput);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::duration<double, micro>>(end - start).count();
        cout << "Time needed to update data in hashmap (Chrono) :" << duration << " 탎\n";

        if (!tree.search(input)) {
            cout << input << " does not exist.\n";
        } else {
            // B+
            auto start_bpt = chrono::high_resolution_clock::now();
            tree.remove(input);
            tree.insert(newinput);
            auto end_bpt = chrono::high_resolution_clock::now();
            auto dur_bpt = chrono::duration_cast<chrono::microseconds>(end_bpt - start_bpt).count();
            cout << "Data successfully updated in b+ tree.\n";
            cout << "Time needed to update data in B+ tree (Chrono) :" << dur_bpt << " 탎\n";
        }

    } else {
        cout << "Invalid option. Please enter 1 or 2 or 3.\n";
    }
}



void ChildSetting(int& child) {
    string inputC;
    cout << "\n---------- BPlus Setting ----------" << endl;
    cout << "Child Maximum: " << child << endl;
    cout << "Do you wish to change the maximum number of child? (Y/other): ";
    cin >> inputC;

    if (inputC == "Y" || inputC == "y") {
        cout << "Enter new maximum number of child: ";
        cin >> child;
        cout << "The maximum number of child has been changed to: " << child << endl;
    } else {
        cout << "The maximum number of child remains: " << child << endl;
    }
}


void DataFiles(string& path) {
    string inputC;

    cout << "\n---------- Data File ----------\n";
    cout << "Current data used: " << path << endl;
    cout << "Do you wish to change the data file? (Y/other): ";
    cin >> inputC;

    if (inputC == "Y" || inputC == "y") {
        cout << "Enter the new data file to be used:\n";
        cout << "1. Data/nama_100_orang.txt\n";
        cout << "2. Data/nama_500_orang.txt\n";
        cout << "3. Data/nama_1000_orang.txt\n";
        cout << "Choose option (1/2/3): ";

        string pilihan;
        cin >> pilihan;

        if (pilihan == "1")
            path = "Data/nama_100_orang.txt";
        else if (pilihan == "2")
            path = "Data/nama_500_orang.txt";
        else if (pilihan == "3")
            path = "Data/nama_1000_orang.txt";
        else {
            cout << "Invalid option. Data file remains: " << path << endl;
            return;
        }

        cout << "The data file has been changed to: " << path << endl;
        cout << "<<!!Remember to update!!>>\n";

    } else {
        cout << "The data file remains: " << path << endl;
    }
}



void Update(int child, BPlusTree<string>& tree, string path, HashMap& map){
    string inputC;

    cout << "\n---------- Update Center ----------" << endl;
    cout << "Current data used: " << path << endl;
    cout << "Do you wish to change the data file? (Y/other): ";
    cin >> inputC;

    if (inputC == "Y" || inputC == "y") {
        cout << "\n-- Updating in Progress --\n";

        tree = BPlusTree<string>(child);
        map.create();

        //HashMap
        loadFromFile(map, path);
        cout << "Estimated HashMap Structure Memory: " << estimateMemory(map) << " byte\n";

        //B+ tree
        auto start_bpt = chrono::high_resolution_clock::now();
        ifstream f(path);
        if (!f.is_open()) {
            cout << "Failen to open file : " << path << endl;
            return;
        }
        string s;
        while (getline(f, s)) {
            tree.insert(s);
        }
        auto end_bpt = chrono::high_resolution_clock::now();
        auto dur_bpt = chrono::duration_cast<chrono::microseconds>(end_bpt - start_bpt).count();


        f.clear();
        f.seekg(0, ios::beg);
        size_t totalStringBytes = 0;

        while (getline(f, s)) {
            totalStringBytes += sizeof(string) + s.capacity();
        }

        size_t treeMemory = tree.estimateMemory();
        cout << "Time needed B+ tree (Chrono) :" << dur_bpt << " 탎\n";
        cout << "Estimated B+ Tree Structure Memory: " << treeMemory << " byte\n";

        cout << "\nRough Memory Estimate for the Data: " << totalStringBytes << " byte\n";
        cout << "Successfully Update!!\n";

    } else {
        cout << "Returning..." << endl;
    }
}


void CompareSearch(BPlusTree<string>& tree, string path, HashMap& map){
    string first, middle, last, best, worst;


    string inputC;

    cout << "\n---------- Compare Time Search Center ----------\n";
    cout << "Current data used: " << path << endl;
    if (tree.root == nullptr || tree.root->keys.empty()) {
        cout << "The Tree is empty. Please update the tree!" << endl;
        return;
    }
    sampledata(tree, path, first, middle, last, best, worst, map);
    cout << "\nEnter the option to be used:\n";
    cout << "1. Predetermined Data.\n";
    cout << "2. Input Data\n";
    cout << "3. Best/Worst Data\n";
    cout << "Choose option (1/2/3): ";
    cin >> inputC;

    if (inputC == "1") {
        cout << "First, middle, last in B+ Tree: \n";
        datasearch(tree, first);
        datasearch(tree, middle);
        datasearch(tree, last);
        cout << "First, middle, last in hashmap: \n";
        searchWithPreciseTime(map, first);
        searchWithPreciseTime(map, middle);
        searchWithPreciseTime(map, last);

    } else if (inputC == "2") {
        cout << "Input the data: \n";
        string dayacari;
        cin.ignore(); // jaga jaga
        getline(cin, dayacari);
        datasearch(tree, dayacari);
        searchWithPreciseTime(map, dayacari);
    } else if (inputC == "3") {
        cout << "Best/Worst Case in B+ Tree: \n";
        datasearch(tree, best);
        datasearch(tree, worst);
        analyzeCases(map);
    } else {
        cout << "Incorrect input - Existing..." << endl;
    }
}


void datasearch(BPlusTree<string>& tree, string value){
        // chrono start
        auto start_bpt = chrono::high_resolution_clock::now();

        if (tree.search(value)) {
            cout << value << " Data Found\n";
        } else {
            cout << value <<  " Data Not Found\n";
        }

        // chrono end
        auto end_bpt = chrono::high_resolution_clock::now();
        auto dur_bpt = chrono::duration_cast<chrono::microseconds>(end_bpt - start_bpt).count();
        // clock end
        cout << "Time needed to find " << value << " in B+ Tree (Chrono) :" << dur_bpt << " 탎\n";
}

void searchWithPreciseTime(HashMap& map, const string& value) {
    const int repeat = 100000;
    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < repeat; ++i) {
        map.search(value);
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::duration<double, micro>>(end - start).count();

    bool found = map.search(value);
    if (found){
        cout << value << " Data Found\n";
        cout << "Time needed to find " << value << " in hashmap :" << duration / repeat << " 탎\n";
    }
    else{
        cout << value <<  " Data Not Found \n";
        cout << "Time needed to find " << value << " in hashmap :" << duration / repeat << " 탎\n";
    }
}



void sampledata(BPlusTree<string>& tree, string path, string& first, string& middle, string& last, string& best, string& worst, HashMap& map){
    vector<string> allLines;
    ifstream f(path);
    if (!f.is_open()) {
        cout << "Failen to open file : " << path << endl;
        return;
    }
    string s;

    while (getline(f, s)) {
        allLines.push_back(s);
    }

    size_t totalLines = allLines.size();
    if (totalLines > 0) {
        first = allLines.front();
        middle = allLines[totalLines / 2];
        last = allLines.back();
    }

    best = tree.root->keys.front();
    worst = getRightmostKey(tree) + "~";
    cout << "Simple Data Preview:\n";
    cout << "First in Data  : " << first << endl;
    cout << "Middle in Data : " << middle << endl;
    cout << "Last in Data  : " << last << endl;

    allLines.clear();
    allLines.shrink_to_fit();

}


//beneran dah ini?? secara logika yang paling dikanan si, tapi emang bervariasi
string getRightmostKey(BPlusTree<string>& tree) {
    auto node = tree.root;
    if (!node) return "";

    while (!node->isLeaf) {
        node = node->children.back();
    }

    if (!node->keys.empty()) {
        return node->keys.back();
    }

    return "";
}


void loadFromFile(HashMap& map, const string& path) {
    ifstream file(path);
    if (!file.is_open()) {
        cout << "Failen to open file : " << path << endl;
        return;
    }

    string line;
    int count = 0;

    auto start = chrono::high_resolution_clock::now();
    while (getline(file, line)) {
        map.insert(line);
        count++;
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::duration<double, micro>>(end - start).count();

    cout << "Time needed HashMap (Chrono) :" << duration << " 탎\n";

}


size_t estimateMemory(const HashMap& map) {
    size_t total = 0;

    total += sizeof(map.table);
    total += map.table.capacity() * sizeof(list<HashNode>);

    for (const auto& bucket : map.table) {
        total += sizeof(bucket);
        for (const auto& node : bucket) {
            total += sizeof(HashNode);
            total += sizeof(char) * (node.key.capacity() + 1);
            total += sizeof(char) * (node.value.capacity() + 1);
        }
    }
    return total;
}



int totalData(const HashMap& map) {
    int total = 0;
    for (const auto& bucket : map.table)
        total += bucket.size();
    return total;
}

void analyzeCases(HashMap& map) {
    string bestCaseValue = "";
    string worstCaseValue = "";
    int minSize = INT_MAX;
    int maxSize = INT_MIN;

    for (int i = 0; i < 100; ++i) {
        int bucketSize = map.table[i].size();
        if (bucketSize == 0) continue;

        if (bucketSize < minSize) {
            minSize = bucketSize;
            bestCaseValue = map.table[i].front().value;
        }
        if (bucketSize > maxSize) {
            maxSize = bucketSize;
            worstCaseValue = map.table[i].back().value;
        }
    }
    cout << "Best/Worst in hashmap\n";
    cout << "Best Case: Searching for '" << bestCaseValue << "' in the bucket with the fewest elements (" << minSize << " elements)\n";
    cout << "Worst Case: Searching for '" << worstCaseValue << "' in the bucket with the most elements (" << maxSize << " elements)\n";
    cout << "Average Case: Average number of elements per bucket: " << fixed << setprecision(2) << (float)totalData(map) / 100 << endl;
    cout << "Best case: ";
    searchWithPreciseTime(map, bestCaseValue);
    cout << "Worst case: ";
    searchWithPreciseTime(map, worstCaseValue);
}
