#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <chrono>
#include <functional>
#include <iomanip>
#include <climits>
#include "Part/HashMap.h"


using namespace std;


void loadFromFile(HashMap& map, const string& path) {
    ifstream file(path);
    if (!file.is_open()) {
        cout << "Gagal membuka file!\n";
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

    cout << count << " data berhasil dimasukkan dari " << path << " dalam " << fixed << setprecision(4) << duration << " mikrodetik.\n";
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
    if (found)
        cout << "Data ditemukan. Waktu rata-rata pencarian: " << fixed << setprecision(4) << duration / repeat << " mikrodetik\n";
    else
        cout << "Data tidak ditemukan. Waktu rata-rata pencarian: " << fixed << setprecision(4) << duration / repeat << " mikrodetik\n";
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

    cout << "\n=== Analisis Best/Worst Case===\n";
    cout << "Best Case: Cari '" << bestCaseValue << "' di bucket tersempit (" << minSize << " elemen)\n";
    cout << "Worst Case: Cari '" << worstCaseValue << "' di bucket terpadat (" << maxSize << " elemen)\n";
    cout << "Average Case: Rata-rata elemen per bucket: " << fixed << setprecision(2) << (float)totalData(map) / 100 << endl;

    cout << "\nPengujian Waktu:\n";
    cout << "\nBest case: ";
    searchWithPreciseTime(map, bestCaseValue);
    cout << "Worst case: ";
    searchWithPreciseTime(map, worstCaseValue);
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

int main() {
    HashMap map;
    int pilihan;
    string value, newValue;

    do {
        cout << "\n===== Menu HashMap =====\n";
        cout << "1. Buat Table\n";
        cout << "2. Load Data nama_100_orang.txt\n";
        cout << "3. Load Data nama_500_orang.txt\n";
        cout << "4. Load Data nama_1000_orang.txt\n";
        cout << "5. Tambah Data Manual\n";
        cout << "6. Cari Data\n";
        cout << "7. Update Data\n";
        cout << "8. Hapus Data\n";
        cout << "9. Tampilkan Semua Data\n";
        cout << "10. Exit\n";
        cout << "11. Analisis Best/Worst Case\n";
        cout << "12. Analisis Memory\n";
        cout << "Pilih: ";
        cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
        case 1:
            map.create();
            break;
        case 2:
            loadFromFile(map, "Data/nama_100_orang.txt");
            break;
        case 3:
            loadFromFile(map, "Data/nama_500_orang.txt");
            break;
        case 4:
            loadFromFile(map, "Data/nama_1000_orang.txt");
            break;
        case 5:
            cout << "Masukkan data: ";
            getline(cin, value);
            {
                auto start = chrono::high_resolution_clock::now();
                map.insert(value);
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::duration<double, micro>>(end - start).count();
                cout << "Waktu penambahan data: " << fixed << setprecision(4) << duration << " mikrodetik\n";
            }
            break;
        case 6:
            cout << "Masukkan data yang ingin dicari: ";
            getline(cin, value);
            searchWithPreciseTime(map, value);
            break;
        case 7:
            cout << "Masukkan data lama yang ingin diupdate: ";
            getline(cin, value);
            cout << "Masukkan data baru: ";
            getline(cin, newValue);
            map.update(value, newValue);
            break;
        case 8:
            cout << "Masukkan data yang ingin dihapus: ";
            getline(cin, value);
            {
                auto start = chrono::high_resolution_clock::now();
                map.remove(value);
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::duration<double, micro>>(end - start).count();
                cout << "Waktu penghapusan data: " << fixed << setprecision(4) << duration << " mikrodetik\n";
            }
            break;
        case 9:
            map.display();
            break;
        case 10:
            cout << "Keluar program...\n";
            break;
        case 11:
            analyzeCases(map);
            break;
        case 12:
            cout << "Total estimasi memori HashMap: " << estimateMemory(map) << " byte\n";
            break;
        default:
            cout << "Pilihan tidak valid!\n";
        }
    } while (pilihan != 10);

    return 0;
}

