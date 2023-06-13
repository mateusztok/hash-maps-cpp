#pragma once

#include "HashMapEntryDH.h"
#include "Constants.h"

#include <iostream>
#include <cmath>

template <typename K, typename V, typename H = std::hash<K>>
class HashMapDH
{
private:
    HashMapEntryDH<K, V>* _buckets;
    H _hasher;
    size_t _capacity;
    float _loadFactor;
    size_t _size;
    size_t _how_much_free;

    size_t threshold();
    void rehash();
    int getNextPrime(int capacity);
    bool isPrime(int n);

public:
    HashMapDH();
    explicit HashMapDH(size_t capacity);
    HashMapDH(size_t capacity, float loadFactor);
    ~HashMapDH();

    size_t getCapacity();
    size_t getSize();
    float getLoadFactor();

    V put(const K& key, const V& value);
    V get(const K& key);
    V remove(const K& key);

    void clear();

    bool containsKey(const K& key);
    bool isEmpty();
};

template <typename K, typename V, typename H>
HashMapDH<K, V, H>::HashMapDH() : _capacity(constants::DEFAULT_CAPACITY), _loadFactor(constants::DEFAULT_LOAD_FACTOR),
                                _size(0) {
    _buckets = new HashMapEntryDH<K, V>[_capacity]();
    _how_much_free = _capacity;
}

template <typename K, typename V, typename H>
HashMapDH<K, V, H>::HashMapDH(size_t capacity) : _loadFactor(constants::DEFAULT_LOAD_FACTOR), _size(0) {
    _capacity = getNextPrime(capacity);
    _buckets = new HashMapEntryDH<K, V>[_capacity];
    _how_much_free = _capacity;
}

template <typename K, typename V, typename H>
HashMapDH<K, V, H>::HashMapDH(size_t capacity, float loadFactor) : _loadFactor(loadFactor), _size(0) {
    _capacity = getNextPrime(capacity);
    _buckets = new HashMapEntryDH<K, V>[_capacity];
    _how_much_free = _capacity;
}

template <typename K, typename V, typename H>
HashMapDH<K, V, H>::~HashMapDH() {
    delete[]_buckets;
}

template <typename K, typename V, typename H>
size_t HashMapDH<K, V, H>::getCapacity() { return _capacity; }

template <typename K, typename V, typename H>
size_t HashMapDH<K, V, H>::getSize() { return _size; }

template <typename K, typename V, typename H>
float HashMapDH<K, V, H>::getLoadFactor() { return _loadFactor; }

template <typename K, typename V, typename H>
bool HashMapDH<K, V, H>::isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i <= sqrt(n); i = i + 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

template <typename K, typename V, typename H>
int HashMapDH<K, V, H>::getNextPrime(int capacity) {
    if (capacity <= 1) return 2;
    int prime = capacity;
    bool found = false;
    while (!found) {
        if (isPrime(prime)) {
            found = true;
        }
        prime++;
    }
    return prime - 1;
}

template <typename K, typename V, typename H>
V HashMapDH<K, V, H>::put(const K& key, const V& value) {
    size_t hashValue = _hasher(key) % _capacity;
    size_t first_a = -1;

    while (_buckets[hashValue].getStatus() != 'f' && _buckets[hashValue].getKey() != key) {
        if (_buckets[hashValue].getStatus() == 'a' && first_a == -1) { first_a = hashValue; }
        hashValue = (hashValue + (_hasher(key) % (_capacity - 1))) % _capacity;
    }

    if (_buckets[hashValue].getKey() != key) {
        if (first_a != -1) {
            _buckets[first_a] = HashMapEntryDH<K, V>(key, value);
            _size++;
            return V(0);
        }
        _buckets[hashValue] = HashMapEntryDH<K, V>(key, value);
        _size++;
        _how_much_free--;
        if (_capacity - this->threshold() >= _how_much_free) { this->rehash(); }

        return V(0);
    }

    V rtnValue = _buckets[hashValue].getValue();
    _buckets[hashValue].setValue(value);
    return rtnValue;
}

template <typename K, typename V, typename H>
V HashMapDH<K, V, H>::get(const K& key) {
    size_t hashValue = _hasher(key) % _capacity;
    while (_buckets[hashValue].getStatus() != 'f') {
        if (_buckets[hashValue].getKey() == key) {
            return _buckets[hashValue].getValue();
        }
        hashValue = (hashValue + _hasher(key) % (_capacity - 1)) % _capacity;
    }
    throw std::out_of_range("KeyError: Given key does not exist in map");
}

template <typename K, typename V, typename H>
V HashMapDH<K, V, H>::remove(const K& key) {
    int hashValue = _hasher(key) % _capacity;

    while (_buckets[hashValue].getStatus() != 'f') {
        if (_buckets[hashValue].getKey() == key) {
            _buckets[hashValue].setStatus('a');
            V rtnValue = _buckets[hashValue].getValue();
            _buckets[hashValue].setValue(V());
            _buckets[hashValue].setKey(K());
            _size--;
            return rtnValue;
        }
        hashValue = (hashValue + _hasher(key) % (_capacity - 1)) % _capacity;
    }
    throw std::out_of_range("KeyError: Given key does not exist in map");
}

template <typename K, typename V, typename H>
bool HashMapDH<K, V, H>::containsKey(const K& key) {
    size_t hashValue = _hasher(key) % _capacity;

    while (_buckets[hashValue].getStatus() != 'f') {
        if (_buckets[hashValue].getKey() == key) {
            return true;
        }
        hashValue = (hashValue + _hasher(key) % (_capacity - 1)) % _capacity;
    }
    return false;
}

template <typename K, typename V, typename H>
bool HashMapDH<K, V, H>::isEmpty() { return _size == 0; }

template <typename K, typename V, typename H>
void HashMapDH<K, V, H>::clear() {
    delete[] _buckets;
    _buckets = new HashMapEntryDH<K, V>[_capacity]();

    for (size_t i = 0; i < _capacity; i++) {
        _buckets[i].setStatus('f');
    }
    _size = 0;
}

template <typename K, typename V, typename H>
size_t HashMapDH<K, V, H>::threshold() { return static_cast<size_t>(_capacity * _loadFactor);
     }

template <typename K, typename V, typename H>
void HashMapDH<K, V, H>::rehash() {
    size_t prevCapacity = _capacity;
    _capacity = getNextPrime(_capacity * 2);
    HashMapEntryDH<K, V>* temp = _buckets;
    _buckets = new HashMapEntryDH<K, V>[_capacity]();
    _size = 0;
    _how_much_free = _capacity;

    for (size_t i = 0; i < _capacity; i++) {
        _buckets[i].setStatus('f');
    }

    for (size_t i = 0; i < prevCapacity; i++) {

        if (temp[i].getStatus() == 'o') {
            this->put(temp[i].getKey(), temp[i].getValue());
        }
    }
    delete[]temp;
}