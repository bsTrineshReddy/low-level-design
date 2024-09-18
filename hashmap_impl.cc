#include <iostream>
#include <vector>
using namespace std;

template<typename K, typename V>
class MyHashMap {
private:
    static const int INITIAL_SIZE = 1 << 4; // 16
    static const int MAXIMUM_CAPACITY = 1 << 30;

    struct Entry {
        K key;
        V value;
        Entry* next;

        Entry(K k, V v) : key(k), value(v), next(nullptr) {}
    };

    vector<Entry*> hashTable;

    // find the next power of 2 value for given capacity, as hash map size must be power of 2
    // if cap = 14 then it returns 16
    // if cap = 16 it retuns 16
    // if cap = 24 it retuns 32
    int tableSizeFor(int cap) {
        int n = cap - 1;
        n |= n >> 1;
        n |= n >> 2;
        n |= n >> 4;
        n |= n >> 8;
        n |= n >> 16;
        return (n < 0) ? 1 : (n >= MAXIMUM_CAPACITY) ? MAXIMUM_CAPACITY : n + 1;
    }

public:
    MyHashMap() {
        hashTable.resize(INITIAL_SIZE, nullptr);
    }

    MyHashMap(int capacity) {
        int tableSize = tableSizeFor(capacity);
        hashTable.resize(tableSize, nullptr);
    }

    void put(K key, V value) {
        int hashCode = key % hashTable.size();
        Entry* node = hashTable[hashCode];

        if (node == nullptr) {
            hashTable[hashCode] = new Entry(key, value);
        } else {
            Entry* previousNode = nullptr;
            while (node != nullptr) {
                if (node->key == key) {
                    node->value = value;
                    return;
                }
                previousNode = node;
                node = node->next;
            }
            previousNode->next = new Entry(key, value);
        }
    }

    V get(K key) {
        int hashCode = key % hashTable.size();
        Entry* node = hashTable[hashCode];

        while (node != nullptr) {
            if (node->key == key) {
                return node->value;
            }
            node = node->next;
        }
        return V(); // Default return value if key not found
    }

    ~MyHashMap() {
        // Destructor to free memory
        for (auto& entry : hashTable) {
            Entry* node = entry;
            while (node != nullptr) {
                Entry* temp = node;
                node = node->next;
                delete temp;
            }
        }
    }
};

int main() {
    MyHashMap<int, string> map(7);
    map.put(1, "hi");
    map.put(2, "my");
    map.put(3, "name");
    map.put(4, "is");
    map.put(5, "Shrayansh");
    map.put(6, "how");
    map.put(7, "are");
    map.put(8, "you");
    map.put(9, "friends");
    map.put(10, "?");

    string value = map.get(8);
    cout << value << endl;

    return 0;
}
