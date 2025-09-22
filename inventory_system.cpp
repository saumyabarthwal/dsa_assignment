/*
 Lab assignment 
 Inventory System for Grocery Store
 
*/

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

//Inventory Item ADT>>
   

struct Item {
    int ItemID;
    string ItemName;
    int Quantity;
    double Price;

    Item(): ItemID(-1), ItemName(""), Quantity(0), Price(0.0) {}
    Item(int id, const string &name, int qty, double pr)
        : ItemID(id), ItemName(name), Quantity(qty), Price(pr) {}
};

// Complexity printing helpers

   
void printComplexities() {
    cout << "\n--- Complexity Summary (per function) ---\n";
    cout << "insertItem: Time O(n), Space O(1) extra.\n";
    cout << "deleteItem: Time O(n), Space O(1).\n";
    cout << "searchByID: Time O(n), Space O(1).\n";
    cout << "searchByName: Time O(n), Space O(1).\n";
    cout << "managePriceQuantity: O(r*c) time, O(r*c) space.\n";
    cout << "sparse representation: ~O(1) average access, space O(k).\n";
    cout << "-----------------------------------------\n\n";
}

//   Inventory Management System

   
class Inventory {
private:
    Item *items;       // dynamic array
    int size;          // current number of items
    int capacity;      // allocated capacity

    unordered_map<int, Item> sparseMap; // sparse storage for rare items

    int indexOfID(int id) const {
        for (int i = 0; i < size; ++i) {
            if (items[i].ItemID == id) return i;
        }
        return -1;
    }

    int indexOfName(const string &name) const {
        for (int i = 0; i < size; ++i) {
            if (items[i].ItemName == name) return i;
        }
        return -1;
    }

    void ensureCapacity() {
        if (size < capacity) return;
        int newCap = max(1, capacity * 2);
        Item *newArr = new Item[newCap];
        for (int i = 0; i < size; ++i) newArr[i] = items[i];
        delete[] items;
        items = newArr;
        capacity = newCap;
    }

public:
    Inventory(int initialCapacity = 4) {
        capacity = max(1, initialCapacity);
        items = new Item[capacity];
        size = 0;
    }

    ~Inventory() {
        delete[] items;
    }

    bool insertItem(const Item &it, bool markRare = false) {
        if (indexOfID(it.ItemID) != -1) {
            cout << "[insertItem] Failed: ItemID " << it.ItemID << " already exists.\n";
            return false;
        }
        if (markRare) {
            sparseMap[it.ItemID] = it;
            cout << "[insertItem] ItemID " << it.ItemID << " stored in sparse map (rare item).\n";
            return true;
        }
        ensureCapacity();
        items[size++] = it;
        cout << "[insertItem] Inserted ItemID " << it.ItemID << ".\n";
        return true;
    }

    bool deleteItem(int id) {
        auto itSparse = sparseMap.find(id);
        if (itSparse != sparseMap.end()) {
            sparseMap.erase(itSparse);
            cout << "[deleteItem] Removed ItemID " << id << " from sparse storage.\n";
            return true;
        }

        int idx = indexOfID(id);
        if (idx == -1) {
            cout << "[deleteItem] ItemID " << id << " not found.\n";
            return false;
        }
        for (int i = idx; i < size - 1; ++i) items[i] = items[i+1];
        --size;
        cout << "[deleteItem] Deleted ItemID " << id << ".\n";
        return true;
    }

    bool searchByID(int id, Item &out) const {
        auto itSparse = sparseMap.find(id);
        if (itSparse != sparseMap.end()) {
            out = itSparse->second;
            return true;
        }
        int idx = indexOfID(id);
        if (idx == -1) return false;
        out = items[idx];
        return true;
    }

    bool searchByName(const string &name, Item &out) const {
        for (const auto &kv : sparseMap) {
            if (kv.second.ItemName == name) {
                out = kv.second;
                return true;
            }
        }
        int idx = indexOfName(name);
        if (idx == -1) return false;
        out = items[idx];
        return true;
    }

    bool addItemRecord(int id, const string &name, int qty, double price, bool isRare=false) {
        Item it(id, name, qty, price);
        return insertItem(it, isRare);
    }

    bool removeItemRecord(int id) {
        return deleteItem(id);
    }

    void managePriceQuantity(int rows, int cols) const {
        cout << "\n[managePriceQuantity] Creating " << rows << "x" << cols << " tables.\n";
        vector<double> priceTable(rows * cols, 0.0);
        vector<int> qtyTable(rows * cols, 0);

        int totalCells = rows * cols;
        int fillIndex = 0;
        vector<Item> allItems;
        for (int i = 0; i < size; ++i) allItems.push_back(items[i]);
        for (const auto &kv : sparseMap) allItems.push_back(kv.second);

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                int pos = r * cols + c;
                if (fillIndex < (int)allItems.size()) {
                    priceTable[pos] = allItems[fillIndex].Price;
                    qtyTable[pos]   = allItems[fillIndex].Quantity;
                }
                fillIndex++;
            }
        }

        cout << "\nRow-major traversal (Price table):\n";
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                int pos = r * cols + c;
                cout << priceTable[pos] << "\t";
            }
            cout << "\n";
        }

        cout << "\nColumn-major traversal:\n";
        for (int c = 0; c < cols; ++c) {
            for (int r = 0; r < rows; ++r) {
                int pos = r * cols + c;
                cout << priceTable[pos] << "\t";
            }
            cout << "\n";
        }
    }

    void optimizeSparseStorage(int thresholdQty) {
        cout << "\n[optimizeSparseStorage] Moving items with Qty <= " << thresholdQty << ".\n";
        int write = 0;
        for (int read = 0; read < size; ++read) {
            if (items[read].Quantity <= thresholdQty) {
                sparseMap[items[read].ItemID] = items[read];
                cout << "  -> Moved ItemID " << items[read].ItemID << " (" << items[read].ItemName << ")\n";
            } else {
                items[write++] = items[read];
            }
        }
        size = write;
    }

    void printInventory() const {
        cout << "\n--- Inventory (Main Array) ---\n";
        if (size == 0) cout << "  (none)\n";
        for (int i = 0; i < size; ++i) {
            cout << "  ID:" << items[i].ItemID
                 << " | Name:" << items[i].ItemName
                 << " | Qty:" << items[i].Quantity
                 << " | Price:" << items[i].Price << "\n";
        }
        cout << "--- Sparse (Rare Items) ---\n";
        if (sparseMap.empty()) cout << "  (none)\n";
        for (const auto &kv : sparseMap) {
            const Item &it = kv.second;
            cout << "  ID:" << it.ItemID
                 << " | Name:" << it.ItemName
                 << " | Qty:" << it.Quantity
                 << " | Price:" << it.Price << "\n";
        }
    }
};

// sample test cases>>
void runSampleTests() {
    Inventory inv(4);

    inv.addItemRecord(101, "Rice_5kg", 20, 40.50);
    inv.addItemRecord(102, "Sugar_1kg", 5, 30.00);
    inv.addItemRecord(103, "Salt_1kg", 50, 10.25);
    inv.addItemRecord(104, "Wheat_10kg", 2, 400.00, true);
    inv.addItemRecord(105, "Oil_1L", 8, 120.0);
    inv.addItemRecord(106, "Tea_250g", 1, 220.0, true);

    inv.printInventory();

    Item out;
    if (inv.searchByID(103, out)) {
        cout << "\nFound by ID 103: " << out.ItemName << " Qty:" << out.Quantity << " Price:" << out.Price << "\n";
    }

    if (inv.searchByName("Sugar_1kg", out)) {
        cout << "Found by Name Sugar_1kg: ID " << out.ItemID << "\n";
    }

    inv.removeItemRecord(102);
    inv.printInventory();

    inv.optimizeSparseStorage(5);
    inv.printInventory();

    inv.managePriceQuantity(2, 3);

    printComplexities();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "===== Grocery Inventory System (C++) =====\n";
    runSampleTests();

    return 0;
}
