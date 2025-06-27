#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <chrono>


#include "Part/Bplus.h"
using namespace std;


void ChildSetting(int& child);  // Function prototype
void DataFiles(string& path); // set new path
void Update(int child, BPlusTree<string>& tree, string path); // for updating
void CompareSearch(BPlusTree<string>& tree, string path);
void sampledata(BPlusTree<string>& tree, string path, string& first, string& middle, string& last, string& best, string& worst);
void datasearch(BPlusTree<string>& tree, string value);
void CompareInsertDelete(BPlusTree<string>& tree);
string getRightmostKey(BPlusTree<string>& tree);

int main() {
    int child = 3;
    int CekData = 0;
    int pilihan;
    string path = "Data/nama_100_orang.txt";
    BPlusTree<string> tree(child);
    do {
        cout << "\nMenu:\n";
        cout << "1. Setting\n";
        cout << "2. DataFiles\n";
        cout << "3. Update\n";
        cout << "4. CompareSearch\n";
        cout << "5. Print\n";
        cout << "6. CompareInsertDelete\n";
        cout << "7. Exist\n";
        cout << "Chosee menu: ";
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1: ChildSetting(child); break;
            case 2: DataFiles(path); break;
            case 3: Update(child, tree, path); break;
            case 4: CompareSearch(tree, path); break;
            case 5: {
                if (tree.root == nullptr || tree.root->keys.empty()) {
                    cout << "Tree Empty." << endl;
                    break;
                } else {
                    cout << "\nB+ Tree setelah semua nama dimasukkan:\n";
                    tree.printTree();
                }
                break;
            }
            case 6: CompareInsertDelete(tree); break;
            case 7: printf("Existing the Program...\n"); break;
            default: printf("Option Not valid.\n");
        }
    } while (pilihan != 7);


    return 0;
}

void CompareInsertDelete(BPlusTree<string>& tree) {
    cout << "\n---------- Compare Insert & Delete ----------" << endl;
    cout << "1. Compare Insert\n";
    cout << "2. Compare Delete\n";
    cout << "Choose option (1/2): ";
    string pilihan;
    cin >> pilihan;
    cin.ignore(); // buat jaga jaga

    string input;

    if (pilihan == "1") {
        cout << "Input the data you want to insert: ";
        getline(cin, input);

        if (tree.search(input)) {
            cout << input << " already exists.\n";
        } else {
            // chrono start
            auto start_bpt = chrono::high_resolution_clock::now();

            tree.insert(input);

            // chrono end
            auto end_bpt = chrono::high_resolution_clock::now();
            auto dur_bpt = chrono::duration_cast<chrono::microseconds>(end_bpt - start_bpt).count();
            cout << "Data successfully inserted.\n";
            cout << "Time needed insert data in B+ tree (Chrono) :" << dur_bpt << " µs\n";
            cout << "Time needed insert data in B+ Tree (clock_t) :" << (float)time_req/CLOCKS_PER_SEC << " seconds" << endl;
        }

    } else if (pilihan == "2") {
        cout << "Input the data you want to remove: ";
        getline(cin, input);

        if (!tree.search(input)) {
            cout << input << " does not exist.\n";
        } else {
            // chrono start
            auto start_bpt = chrono::high_resolution_clock::now();
            tree.remove(input);
            // chrono end
            auto end_bpt = chrono::high_resolution_clock::now();
            auto dur_bpt = chrono::duration_cast<chrono::microseconds>(end_bpt - start_bpt).count();
            cout << "Data successfully removed.\n";
            cout << "Time needed remove data in B+ tree (Chrono) :" << dur_bpt << " µs\n";
            cout << "Time needed remove data in B+ Tree (clock_t) :" << (float)time_req/CLOCKS_PER_SEC << " seconds" << endl;
        }

    } else {
        cout << "Invalid option. Please enter 1 or 2.\n";
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
        cout << "\nEnter the new data file to be used:\n";
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



void Update(int child, BPlusTree<string>& tree, string path){
    string inputC;

    cout << "\n---------- Update Center ----------" << endl;
    cout << "Current data used: " << path << endl;
    cout << "Do you wish to change the data file? (Y/other): ";
    cin >> inputC;

    if (inputC == "Y" || inputC == "y") {
        cout << "\n-- Updating in Progress --\n";

        tree = BPlusTree<string>(child);
        // chrono start
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

        // chrono end
        auto end_bpt = chrono::high_resolution_clock::now();
        auto dur_bpt = chrono::duration_cast<chrono::microseconds>(end_bpt - start_bpt).count();

        f.clear();
        f.seekg(0, ios::beg);
        size_t totalStringBytes = 0;

        while (getline(f, s)) {
            totalStringBytes += sizeof(string) + s.capacity();
        }

        size_t treeMemory = tree.estimateMemory();
        cout << "Estimated B+ Tree Structure Memory: " << treeMemory << " byte\n";
        cout << "Time needed B+ tree (Chrono) :" << dur_bpt << " µs\n";
        cout << "Time needed B+ Tree (clock_t) :" << (float)time_req/CLOCKS_PER_SEC << " seconds" << endl;
        cout << "\nRough Memory Estimate for the Data: " << totalStringBytes << " byte\n";
        cout << "Successfully Update!!\n";

    } else {
        cout << "Returning..." << endl;
    }
}


void CompareSearch(BPlusTree<string>& tree, string path){
    string first, middle, last, best, worst;


    string inputC;

    cout << "\n---------- Compare Time Search Center ----------\n";
    cout << "Current data used: " << path << endl;
    if (tree.root == nullptr || tree.root->keys.empty()) {
        cout << "The Tree is empty. Please update the tree!" << endl;
        return;
    }
    sampledata(tree, path, first, middle, last, best, worst);
    cout << "\nEnter the option to be used:\n";
    cout << "1. Predetermined Data.\n";
    cout << "2. Input Data.txt\n";
    cout << "Choose option (1/2): ";
    cin >> inputC;

    if (inputC == "1") {
        datasearch(tree, first);
        datasearch(tree, middle);
        datasearch(tree, last);
        datasearch(tree, best);
        datasearch(tree, worst);
    } else if (inputC == "2") {
        cout << "Input the data: \n";
        string dayacari;
        cin.ignore(); // jaga jaga
        getline(cin, dayacari);
        datasearch(tree, dayacari);
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

        cout << "Time needed to find " << value << " in B+ Tree (Chrono) :" << dur_bpt << " µs\n";
        cout << "Time needed to find " << value << " in B+ Tree (clock_t) :" << (float)time_req/CLOCKS_PER_SEC << " seconds" << endl;
}


void sampledata(BPlusTree<string>& tree, string path, string& first, string& middle, string& last, string& best, string& worst){
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
    cout << "Best Case(root)   : " << best << endl;
    cout << "Worst Case(did not exist)   : " << worst << endl;

    allLines.clear();
    allLines.shrink_to_fit();  // opsional, tapi membantu jika kamu ingin benar-benar lepas memori heap

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

