#pragma once

#include "HashMapEntryRH.h"
#include "Hasher.h"

#include <cmath>

const size_t DEFAULT_CAPACITY = 32;
const float DEFAULT_LOAD_FACTOR = 0.75f;

template <typename K, typename V, typename H = Hasher<K>>
class HashMapRH {
private:
    HashMapEntryRH<K, V> **_buckets;
    H _hasher;
    size_t _capacity;
    float _loadFactor;
    size_t _size;

    size_t _longestPSL;
    size_t _totalCost;

    size_t threshold();
    int search(const K &key);
    void rehash();

public:
    HashMapRH();
    explicit HashMapRH(size_t capacity);
    HashMapRH(size_t capacity, float loadFactor);
    ~HashMapRH();

    size_t getCapacity();
    size_t getSize();
    float getLoadFactor();

    V put(const K &key, const V &value);
    V get(const K &key);
    V remove(const K &key);

    void clear();

    bool containsKey(const K &key);
    bool isEmpty();
};

template <typename K, typename V, typename H>
HashMapRH<K, V, H>::HashMapRH() : _capacity(DEFAULT_CAPACITY), _loadFactor(DEFAULT_LOAD_FACTOR), _size(0),
                                _longestPSL(0), _totalCost(0) {
    _buckets = new HashMapEntryRH<K, V> *[_capacity]();
}

template <typename K, typename V, typename H>
HashMapRH<K, V, H>::HashMapRH(size_t capacity) : _capacity(capacity), _loadFactor(DEFAULT_LOAD_FACTOR), _size(0),
                                                _longestPSL(0), _totalCost(0) {
    _buckets = new HashMapEntryRH<K, V> *[_capacity]();
}

template <typename K, typename V, typename H>
HashMapRH<K, V, H>::HashMapRH(size_t capacity, float loadFactor) : _capacity(capacity), _loadFactor(loadFactor),
                                                                _size(0), _longestPSL(0), _totalCost(0) {
    _buckets = new HashMapEntryRH<K, V> *[_capacity]();
}

template <typename K, typename V, typename H>
HashMapRH<K, V, H>::~HashMapRH() {
     if (!this->isEmpty()) this->clear();
     delete []_buckets;
}

template <typename K, typename V, typename H>
size_t HashMapRH<K, V, H>::getCapacity() { return _capacity; }

template <typename K, typename V, typename H>
size_t HashMapRH<K, V, H>::getSize() { return _size; }

template <typename K, typename V, typename H>
float HashMapRH<K, V, H>::getLoadFactor() { return _loadFactor; }

template <typename K, typename V, typename H>
V HashMapRH<K, V, H>::put(const K &key, const V &value) {
    size_t hashValue = _hasher(key) % _capacity;

    auto *entry = new HashMapEntryRH<K, V>(key, value);
    HashMapEntryRH<K, V> *current;

    size_t itr = 0; size_t idx;
    while (itr < _capacity) {
        idx = (hashValue + itr) % _capacity;
        current = _buckets[idx];

        _totalCost++;

        if (current == nullptr) {
            _buckets[idx] = entry;
            _size++;
            _longestPSL = std::max(entry->getPSL(), _longestPSL);

            if (this->threshold() < _size) { this->rehash(); }
            return V();
        }

        if (current->getKey() == key) break;

        if (current->getPSL() < entry->getPSL()) std::swap(_buckets[idx], entry);
        entry->setPSL(entry->getPSL() + 1);
        itr++;
    }

    _totalCost -= itr + 1;
    V rtnValue = current->getValue();
    current->setValue(value);
    return rtnValue;
}

template <typename K, typename V, typename H>
V HashMapRH<K, V, H>::get(const K &key) {
    int idx = this->search(key);

    if (idx != -1) return _buckets[idx]->getValue();
    throw std::out_of_range("KeyError: Given key does not exist in map");

//    size_t itr = 0; size_t idx;
//    while (itr < _capacity) {
//        idx = hashValue + itr;
//        if (idx >= _capacity) idx -= _capacity;
//        current = _buckets[idx];
//
//        if (current != nullptr && current->getKey() == key) return current->getValue();
//        itr++;
//    }

}

template <typename K, typename V, typename H>
V HashMapRH<K, V, H>::remove(const K &key) {
    int idx = this->search(key);
    if (idx == -1) throw std::out_of_range("KeyError: Given key does not exist in map");

    HashMapEntryRH<K, V> *current = _buckets[idx];
    V rtnValue = current->getValue();
    delete current;

    int next = idx + 1;
    // TODO: Left shift after deletion

    return rtnValue;
}

template <typename K, typename V, typename H>
bool HashMapRH<K, V, H>::containsKey(const K &key) {
    if (this->search(key) != -1) return true;
    return false;
}

template <typename K, typename V, typename H>
bool HashMapRH<K, V, H>::isEmpty() { return _size == 0; }

template<typename K, typename V, typename H>
int HashMapRH<K, V, H>::search(const K &key) {
    if (this->isEmpty()) return -1;

    size_t hashValue = _hasher(key) % _capacity;
    HashMapEntryRH<K, V> *current;
    auto mean = static_cast<size_t>(std::round(_totalCost / _size));

    size_t down = mean;
    size_t up = down + 1;
    size_t idx;

    while (down >= 1 && up <= _longestPSL + 1) {
        idx = (hashValue + down - 1) % _capacity;
        current = _buckets[idx];
        if (current != nullptr && current->getKey() == key) return static_cast<int>(idx);

        idx = (hashValue + up - 1) % _capacity;
        current = _buckets[idx];
        if (current != nullptr && current->getKey() == key) return static_cast<int>(idx);

        down--;
        up++;
    }

    while (down >= 1) {
        idx = (hashValue + down - 1) % _capacity;
        current = _buckets[idx];
        if (current != nullptr && current->getKey() == key) return static_cast<int>(idx);
        down--;
    }

    while (up <= _longestPSL + 1) {
        idx = (hashValue + up - 1) % _capacity;
        current = _buckets[idx];
        if (current != nullptr && current->getKey() == key) return static_cast<int>(idx);
        up++;
    }

    return -1;
}

template <typename K, typename V, typename H>
void HashMapRH<K, V, H>::clear() {
    for (size_t i = 0; i < _capacity; i++) {
        HashMapEntryRH<K, V> *current = _buckets[i];
        if (current != nullptr) {
            delete current;
            _buckets[i] = nullptr;
            _size--;
        }
    }
}

template <typename K, typename V, typename H>
size_t HashMapRH<K, V, H>::threshold() { return static_cast<size_t>(_capacity * _loadFactor); }

template <typename K, typename V, typename H>
void HashMapRH<K, V, H>::rehash() { }