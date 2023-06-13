#pragma once

#include "HashMapEntryLL.h"
#include "Constants.h"

#include <iostream>

template <typename K, typename V, typename H = std::hash<K>>
class HashMapLL {
private:
    HashMapEntryLL<K, V> **_buckets;
    H _hasher;
    size_t _capacity;
    float _loadFactor;
    size_t _size;

    size_t threshold();
    void rehash();

public:
    HashMapLL();
    explicit HashMapLL(size_t capacity);
    HashMapLL(size_t capacity, float loadFactor);
    ~HashMapLL();

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
HashMapLL<K, V, H>::HashMapLL() : _capacity(constants::DEFAULT_CAPACITY), _loadFactor(constants::DEFAULT_LOAD_FACTOR),
                                _size(0)  {
    _buckets = new HashMapEntryLL<K, V> *[_capacity]();
}

template <typename K, typename V, typename H>
HashMapLL<K, V, H>::HashMapLL(size_t capacity) : _capacity(capacity), _loadFactor(constants::DEFAULT_LOAD_FACTOR),
                                                _size(0) {
    _buckets = new HashMapEntryLL<K, V> *[_capacity]();
}

template <typename K, typename V, typename H>
HashMapLL<K, V, H>::HashMapLL(size_t capacity, float loadFactor) : _capacity(capacity), _loadFactor(loadFactor), _size(0) {
    _buckets = new HashMapEntryLL<K, V> *[_capacity]();
}

template <typename K, typename V, typename H>
HashMapLL<K, V, H>::~HashMapLL() {
    if (!this->isEmpty()) this->clear();
    delete []_buckets;
}

template <typename K, typename V, typename H>
size_t HashMapLL<K, V, H>::getCapacity() { return _capacity; }

template <typename K, typename V, typename H>
size_t HashMapLL<K, V, H>::getSize() { return _size; }

template <typename K, typename V, typename H>
float HashMapLL<K, V, H>::getLoadFactor() { return _loadFactor; }

template <typename K, typename V, typename H>
V HashMapLL<K, V, H>::put(const K &key, const V &value) {
    size_t hashValue = _hasher(key) % _capacity;
    HashMapEntryLL<K, V> *entry = _buckets[hashValue];
    HashMapEntryLL<K, V> *prev = nullptr;

    while (entry != nullptr && entry->getKey() != key) {
        prev = entry;
        entry = entry->getNext();
    }

    if (entry == nullptr) {
        if (prev == nullptr) _buckets[hashValue] = new HashMapEntryLL<K, V>(key, value);
        else prev->setNext(new HashMapEntryLL<K, V>(key, value));

        _size++;
        if (this->threshold() < _size) { this->rehash(); }
        return V();
    }

    V rtnValue = entry->getValue();
    entry->setValue(value);
    return rtnValue;
}

template <typename K, typename V, typename H>
V HashMapLL<K, V, H>::get(const K &key) {
    size_t hashValue = _hasher(key) % _capacity;
    HashMapEntryLL<K, V> *entry = _buckets[hashValue];

    while (entry != nullptr && entry->getKey() != key) entry = entry->getNext();
    if (entry == nullptr) throw std::out_of_range("KeyError: Given key does not exist in map");
    return entry->getValue();
}

template <typename K, typename V, typename H>
V HashMapLL<K, V, H>::remove(const K &key) {
    size_t hashValue = _hasher(key) % _capacity;
    HashMapEntryLL<K, V> *entry = _buckets[hashValue];
    HashMapEntryLL<K, V> *prev = nullptr;

    while (entry != nullptr && entry->getKey() != key) {
        prev = entry;
        entry = entry->getNext();
    }

    if (entry == nullptr) throw std::out_of_range("KeyError: Given key does not exist in map");

    V rtnValue;
    if (prev == nullptr) {
        rtnValue = entry->getValue();
        if (entry->getNext() != nullptr) _buckets[hashValue] = entry->getNext();
        else _buckets[hashValue] = nullptr;

        _size--;
        delete entry;
        return rtnValue;
    }

    if (entry->getNext() == nullptr) {
        rtnValue = entry->getValue();
        prev->setNext(nullptr);

        _size--;
        delete entry;
        return rtnValue;
    }

    rtnValue = entry->getValue();
    prev->setNext(entry->getNext());

    _size--;
    delete entry;
    return rtnValue;
}

template <typename K, typename V, typename H>
bool HashMapLL<K, V, H>::containsKey(const K &key) {
    size_t hashValue = _hasher(key) % _capacity;
    HashMapEntryLL<K, V> *entry = _buckets[hashValue];
    while (entry != nullptr && entry->getKey() != key) entry = entry->getNext();

    if (entry == nullptr) return false;
    return true;
}

template <typename K, typename V, typename H>
bool HashMapLL<K, V, H>::isEmpty() { return _size == 0; }

template <typename K, typename V, typename H>
void HashMapLL<K, V, H>::clear() {
    for (size_t i = 0; i < _capacity; i++) {
        HashMapEntryLL<K, V> *current = _buckets[i];
        HashMapEntryLL<K, V> *helper;

        while (current != nullptr) {
            helper = current;
            current = current->getNext();

            _size--;
            delete helper;
        }
        _buckets[i] = nullptr;
    }
}

template <typename K, typename V, typename H>
size_t HashMapLL<K, V, H>::threshold() { return static_cast<size_t>(_capacity * _loadFactor); }

template <typename K, typename V, typename H>
void HashMapLL<K, V, H>::rehash() {
    size_t prevCapacity = _capacity; _capacity *= 2;
    HashMapEntryLL<K, V> **temp = _buckets;
    _buckets = new HashMapEntryLL<K, V> *[_capacity]();
    _size = 0;

    for (size_t i = 0; i < prevCapacity; i++) {
        HashMapEntryLL<K, V> *current = temp[i];
        HashMapEntryLL<K, V> *helper;

        while (current != nullptr) {
            this->put(current->getKey(), current->getValue());
            helper = current;
            current = current->getNext();
            delete helper;
        }
    }
    delete []temp;
}
