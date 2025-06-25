#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <chrono>


#include "Part/Bplus.h"

using namespace std;

int main() {
    BPlusTree<string> tree(3);

    cout << "Pilih file:\n";
    cout << "1. Data/nama_100_orang.txt\n";
    cout << "2. Data/nama_500_orang.txt\n";
    cout << "3. Data/nama_1000_orang.txt\n";
    cout << "Masukkan pilihan (1/2/3): ";

    int pilihan;
    cin >> pilihan;


    string path;
    if (pilihan == 1)
        path = "Data/nama_100_orang.txt";
    else if (pilihan == 2)
        path = "Data/nama_500_orang.txt";
    else if (pilihan == 3)
        path = "Data/nama_1000_orang.txt";
    else {
        cout << "Pilihan tidak valid." << endl;
        return 1;
    }
    // bagian awal clock_t
    clock_t time_req;
    auto start_bpt = chrono::high_resolution_clock::now();

    ifstream f(path);
    if (!f.is_open()) {
        cout << "Gagal membuka file: " << path << endl;
        return 1;
    }
    size_t totalStringBytes = 0;


    string s;
    while (getline(f, s)) {
        tree.insert(s);
        totalStringBytes += sizeof(string) + s.capacity();
    }
    auto end_bpt = chrono::high_resolution_clock::now();
    auto dur_bpt = chrono::duration_cast<chrono::microseconds>(end_bpt - start_bpt).count();

    time_req = clock() - time_req;
    //bagian akhir clock

    cout << "Waktu insert B+ Tree: " << dur_bpt << " µs\n";

	cout << "Waktu yang dibutuhkan untuk memasukkan (clock_t) :" << (float)time_req/CLOCKS_PER_SEC << " seconds" << endl;

    cout << "\nEstimasi kasar memori untuk data: " << totalStringBytes << " byte\n";


    cout << "\nB+ Tree setelah semua nama dimasukkan:\n";
    tree.printTree();

    tree = BPlusTree<string>(3);

    cout << "\nB+ Tree setelah semua nama dimasukkan:\n";
    tree.printTree();

    if (tree.root == nullptr) {
        cout << "Tree kosong." << endl;
    }


    return 0;
}

