#pragma once

#include "HashMapEntryRH.h"
#include "Constants.h"

#include <cmath>

template <typename K, typename V, typename H = std::hash<K>>
class HashMapRH {
private:
    HashMapEntryRH<K, V> **_buckets;
    H _hasher;
    size_t _capacity;
    float _loadFactor;
    size_t _size;

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
HashMapRH<K, V, H>::HashMapRH() : _capacity(constants::DEFAULT_CAPACITY), _loadFactor(constants::DEFAULT_LOAD_FACTOR),
                                _size(0) {
    _buckets = new HashMapEntryRH<K, V> *[_capacity]();
}

template <typename K, typename V, typename H>
HashMapRH<K, V, H>::HashMapRH(size_t capacity) : _capacity(capacity), _loadFactor(constants::DEFAULT_LOAD_FACTOR),
                                                _size(0) {
    _buckets = new HashMapEntryRH<K, V> *[_capacity]();
}

template <typename K, typename V, typename H>
HashMapRH<K, V, H>::HashMapRH(size_t capacity, float loadFactor) : _capacity(capacity), _loadFactor(loadFactor),
                                                                    _size(0) {
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

        if (current == nullptr) {
            _buckets[idx] = entry;
            _size++;

            if (this->threshold() < _size) this->rehash();
            return V();
        }

        if (current->getKey() == key) break;

        if (current->getPSL() < entry->getPSL()) std::swap(_buckets[idx], entry);
        entry->setPSL(entry->getPSL() + 1);
        itr++;
    }

    V rtnValue = current->getValue();
    current->setValue(value);
    return rtnValue;
}

template <typename K, typename V, typename H>
V HashMapRH<K, V, H>::get(const K &key) {
    int idx = this->search(key);

    if (idx != -1) return _buckets[idx]->getValue();
    throw std::out_of_range("KeyError: Given key does not exist in map");
}

template <typename K, typename V, typename H>
V HashMapRH<K, V, H>::remove(const K &key) {
    int idx = this->search(key);
    if (idx == -1) throw std::out_of_range("KeyError: Given key does not exist in map");

    V rtnValue = _buckets[idx]->getValue();
    delete _buckets[idx];
    _size--;

    HashMapEntryRH<K, V> *next; int itr;
    while (true) {
        itr = idx + 1;
        next = _buckets[itr % _capacity];
        if (next == nullptr) break;
        if (next->getPSL() <= 0) break;

        next->setPSL(next->getPSL() - 1);
        _buckets[idx % _capacity] = next;
        _buckets[itr % _capacity] = nullptr;

        idx = itr;
    }

    return rtnValue;
}

template <typename K, typename V, typename H>
bool HashMapRH<K, V, H>::containsKey(const K &key) {
    if (this->search(key) == -1) return false;
    return true;
}

template <typename K, typename V, typename H>
bool HashMapRH<K, V, H>::isEmpty() { return _size == 0; }

template<typename K, typename V, typename H>
int HashMapRH<K, V, H>::search(const K &key) {
    if (this->isEmpty()) return -1;

    size_t hashValue = _hasher(key) % _capacity;
    HashMapEntryRH<K, V> *current;

    size_t itr = 0; size_t idx;
    while (itr < _capacity) {
        idx = (hashValue + itr) % _capacity;
        current = _buckets[idx];

        if (current == nullptr) break;
        if (itr > current->getPSL()) break;
        if (current->getKey() == key) return static_cast<int>(idx);
        itr++;
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
void HashMapRH<K, V, H>::rehash() {
    size_t prevCapacity = _capacity; _capacity *= 2;
    HashMapEntryRH<K, V> **temp = _buckets;
    _buckets = new HashMapEntryRH<K, V> *[_capacity]();
    _size = 0;

    for (size_t i = 0; i < prevCapacity; i++) {
        HashMapEntryRH<K, V> *current = temp[i];
        if (current != nullptr) {
            this->put(current->getKey(), current->getValue());
            delete current;
        }
    }
    delete []temp;
}