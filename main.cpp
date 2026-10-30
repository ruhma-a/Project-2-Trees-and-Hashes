#include <iostream>
#include <vector>
#include <list>
#include <unordered_set>
#include <chrono>
#include <iomanip>

#include "Item.hpp"
#include "Compare.hpp"
#include "ItemGenerator.hpp"
#include "Inventory.hpp"
#include "HashInventory.hpp"
#include "TreeInventory.hpp"

using namespace std;
using namespace std::chrono;

// Timing contains()
template <typename InventoryType>
double timeContains(int n) {
    InventoryType inv;
    ItemGenerator gen(42);

    for (int i = 0; i < n; ++i) {
        inv.pickup(gen.randomItem());
    }

    vector<string> contained;
    for (int i = 0; i < 100; ++i) {
        contained.push_back(gen.randomUsedName());
    }

    vector<string> missing;
    for (int i = 0; i < 100; ++i) {
        missing.push_back(gen.randomItem().name_); 
    }

    double totalTimeMs = 0.0;

    for (const string& name : contained) {
        auto start = high_resolution_clock::now();
        inv.contains(name);
        auto stop = high_resolution_clock::now();
        totalTimeMs += duration<double, std::milli>(stop - start).count();
    }

    for (const string& name : missing) {
        auto start = high_resolution_clock::now();
        inv.contains(name);
        auto stop = high_resolution_clock::now();
        totalTimeMs += duration<double, std::milli>(stop - start).count();
    }

    return totalTimeMs / 200.0;
}

// Timing query() CompareItemName
template <typename InventoryType>
double timeQueryName(int n) {
    InventoryType inv;
    ItemGenerator gen(42); 

    for (int i = 0; i < n; ++i) {
        inv.pickup(gen.randomItem());
    }

    double totalTimeMs = 0.0;

    for (int i = 0; i < 10; ++i) {
        Item start(gen.randomUsedName());
        Item end(gen.randomUsedName());

        if (CompareItemName::lessThan(end, start)) {
            swap(start, end);
        }

        auto startTime = high_resolution_clock::now();
        inv.query(start, end);
        auto stopTime = high_resolution_clock::now();
        
        totalTimeMs += duration<double, std::milli>(stopTime - startTime).count();
    }

    return totalTimeMs / 10.0;
}

// Timing query() CompareItemWeight
template <typename InventoryType>
double timeQueryWeight(int n) {
    InventoryType inv;
    ItemGenerator gen(42); 

    for (int i = 0; i < n; ++i) {
        inv.pickup(gen.randomItem());
    }

    double totalTimeMs = 0.0;

    for (int i = 0; i < 10; ++i) {
        float minWeight = gen.randomFloat(ItemGenerator::MIN_WEIGHT, ItemGenerator::MAX_WEIGHT);
        Item start("start", minWeight, ItemType::None);
        Item end("end", minWeight + 0.1f, ItemType::None);

        auto startTime = high_resolution_clock::now();
        inv.query(start, end);
        auto stopTime = high_resolution_clock::now();
        
        totalTimeMs += duration<double, std::milli>(stopTime - startTime).count();
    }

    return totalTimeMs / 10.0;
}

int main() {
    vector<int> n_values = {1000, 2000, 4000, 8000};

    cout << fixed << setprecision(6);
    
    cout << "=== Part A: Average contains() Execution Time (ms) ===" << endl;
    cout << "N\tVector\t\tList\t\tHash\t\tTree\n";
    for (int n : n_values) {
        double tVec  = timeContains<Inventory<CompareItemName, vector<Item>>>(n);
        double tList = timeContains<Inventory<CompareItemName, list<Item>>>(n);
        double tHash = timeContains<Inventory<CompareItemName, unordered_set<Item>>>(n);
        double tTree = timeContains<Inventory<CompareItemName, Tree>>(n);
        
        cout << n << "\t" << tVec << "\t" << tList << "\t" << tHash << "\t" << tTree << "\n";
    }
    
    cout << "\n=== Part B: Average query() Name Execution Time (ms) ===" << endl;
    cout << "N\tVector\t\tList\t\tHash\t\tTree\n";
    for (int n : n_values) {
        double tVec  = timeQueryName<Inventory<CompareItemName, vector<Item>>>(n);
        double tList = timeQueryName<Inventory<CompareItemName, list<Item>>>(n);
        double tHash = timeQueryName<Inventory<CompareItemName, unordered_set<Item>>>(n);
        double tTree = timeQueryName<Inventory<CompareItemName, Tree>>(n);
        
        cout << n << "\t" << tVec << "\t" << tList << "\t" << tHash << "\t" << tTree << "\n";
    }

    cout << "\n=== Part B: Average query() Weight Execution Time (ms) ===" << endl;
    cout << "N\tVector\t\tList\t\tHash\t\tTree\n";
    for (int n : n_values) {
        double tVec  = timeQueryWeight<Inventory<CompareItemWeight, vector<Item>>>(n);
        double tList = timeQueryWeight<Inventory<CompareItemWeight, list<Item>>>(n);
        double tHash = timeQueryWeight<Inventory<CompareItemWeight, unordered_set<Item>>>(n);
        double tTree = timeQueryWeight<Inventory<CompareItemWeight, Tree>>(n);
        
        cout << n << "\t" << tVec << "\t" << tList << "\t" << tHash << "\t" << tTree << "\n";
    }

    return 0;
}