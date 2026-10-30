# Distributed Inventory System: Hash vs. Tree Performance Benchmarking

A high-performance C++ inventory management library and empirical benchmarking suite comparing **Hash-Based (`std::unordered_set`)** and **Tree-Based (`ItemAVL`)** data structures against standard sequence containers (`std::vector`, `std::list`).

---

## 📌 Project Overview

This system manages game/item inventories containing unique items. It provides flexible query capabilities based on customizable properties such as **Item Name**, **Weight**, and **Type** using template specialization and custom comparator wrappers.

The repository evaluates trade-offs between $O(1)$ constant-time point lookups in hash tables and $O(\log N + K)$ range-pruned queries in balanced AVL search trees across dataset sizes of $N \in \{1000, 2000, 4000, 8000\}$ items.

> 📄 **Analysis Report:** A complete analysis write-up detailing the architectural trade-offs, theoretical time complexities, and experimental discussion is available in [`Project2_Analysis.pdf`]([https://www.google.com/search?q=./Project2_Report.pdf](https://github.com/ruhma-a/Project-2-Trees-and-Hashes/blob/main/Project%202%20Analysis.pdf)).

---

## 🛠️ Key Components & Architecture

* **`Item` & Custom Hash:** Specialized `std::hash<Item>` template operating on item name strings for $O(1)$ hashing.


* **`CompareItemName`, `CompareItemWeight`, `CompareItemType`:** Static comparator wrappers enabling generic property comparisons across inventory operations.


* **`HashInventory`:** Specialized inventory using `std::unordered_set<Item>` for constant-time lookup, insertion, and deletion.


* **`ItemAVL` & `TreeInventory`:** Self-balancing AVL tree implementation supporting custom branch-pruned range queries (`queryHelper`).


* **`ItemGenerator`:** Seeded pseudorandom item generation (seed 42) for deterministic benchmarking.



---

## 📊 Performance Benchmarks

All execution times represent average runtimes measured in milliseconds (`ms`) using `std::chrono::high_resolution_clock`.

### 1. `contains()` Execution Time (ms)

Measures single-item existence lookup times for 100 contained items and 100 missing items ($N=200$ checks averaged):

| $N$ | `std::vector` | `std::list` | `HashInventory` | `TreeInventory` (AVL) |
| --- | --- | --- | --- | --- |
| **1000** | 0.000031 ms | 0.000032 ms | **0.000151 ms** | 0.004898 ms |
| **2000** | 0.000031 ms | 0.000030 ms | **0.000161 ms** | 0.014010 ms |
| **4000** | 0.000030 ms | 0.000029 ms | **0.000163 ms** | 0.038676 ms |
| **8000** | 0.000031 ms | 0.000031 ms | **0.000176 ms** | 0.085183 ms |

* **Insight:** `HashInventory` exhibits constant $O(1)$ lookup performance regardless of $N$. `TreeInventory` scales linearly ($O(N)$) when looking up by name because the AVL tree is structured by another comparator, requiring a full tree traversal.



---

### 2. Range `query()` Execution Time by Weight (ms)

Measures execution time when selecting items within a floating-point weight range $[min, min + 0.1]$ over 10 randomized trials:

| $N$ | `std::vector` | `std::list` | `HashInventory` | `TreeInventory` (AVL) |
| --- | --- | --- | --- | --- |
| **1000** | 0.010108 ms | 0.010203 ms | 0.010975 ms | **0.001174 ms** |
| **2000** | 0.016901 ms | 0.021707 ms | 0.031657 ms | **0.002574 ms** |
| **4000** | 0.032527 ms | 0.043616 ms | 0.073995 ms | **0.004806 ms** |
| **8000** | 0.071338 ms | 0.079840 ms | 0.153839 ms | **0.010377 ms** |

* **Insight:** `TreeInventory` outperforms all other containers by an order of magnitude (0.010 ms vs 0.153 ms at $N=8000$). The AVL tree leverages branch pruning in `queryHelper()` to eliminate subtrees outside the query range, executing in $O(\log N + K)$ time, whereas linear and hash containers must scan every element ($O(N)$).



---

### 3. Range `query()` Execution Time by Name (ms)

Measures execution time when selecting items between two randomized names over 10 trials:

| $N$ | `std::vector` | `std::list` | `HashInventory` | `TreeInventory` (AVL) |
| --- | --- | --- | --- | --- |
| **1000** | 0.073084 ms | 0.070175 ms | 0.083012 ms | 0.258651 ms |
| **2000** | 0.145662 ms | 0.326843 ms | 0.532731 ms | 0.123408 ms |
| **4000** | 1.083370 ms | 0.309434 ms | 0.408855 ms | 0.256793 ms |
| **8000** | 1.057846 ms | 0.993995 ms | 1.357883 ms | 0.848649 ms |

---

## 🔑 Key Engineering Takeaways

1. **Hash Tables for Point Access:** Choose a Hash-based inventory when workloads prioritize rapid single-item lookups, insertions, or deletions by key ($O(1)$ average).


2. **Balanced Trees for Range Access:** Choose a Tree-based inventory when workloads require range-based queries on ordered properties or sorted traversals. Tree pruning limits work to relevant subtrees ($O(\log N + K)$).



---

## 💻 Building and Running

### Prerequisites

* `g++` compiler supporting C++11 or higher
* GNU `make`

### Compilation & Execution

```bash
# Clone the repository
git clone https://github.com/ruhma-a/Project-2-Trees-and-Hashes.git
cd Project-2-Trees-and-Hashes

# Build the executable using Makefile
make

# Run the benchmark test suite
./main

```

---

## 📂 Repository Structure

```text
├── Makefile                # Build configuration
├── README.md               # Main repository documentation & summary
├── Project2_Report.pdf     # Full analysis report & theoretical discussion
├── main.cpp                # Benchmark runner & timing harness
├── Item.hpp / .cpp         # Core Item object & specialized std::hash<Item>
├── Compare.hpp / .cpp      # Custom static property comparators
├── Inventory.hpp           # Base templated inventory interface
├── HashInventory.hpp / .cpp# std::unordered_set inventory specialization
├── ItemAVL.hpp / .cpp      # Custom self-balancing AVL Tree implementation
└── TreeInventory.hpp / .cpp# Tree-based inventory specialization & query pruning

```
