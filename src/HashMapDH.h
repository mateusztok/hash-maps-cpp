#pragma once

#include "HashNode2.h"
#include "Hasher.h"
#include "Hasher2.h"

#include <iostream>
#include <cmath>

const size_t DEFAULT_CAPACITY = 31;
const float DEFAULT_LOAD_FACTOR = 0.75f;

template <typename K, typename V, typename H = Hasher<K>, typename H2 = Hasher2<K>>
class HashMapDH
{
private:
    HashNode2<K, V>* _array;
    H _hasher;
    H _hasher2;
    size_t _capacity;
    float _loadFactor;
    size_t _size;
    int _collision = 0;

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
    int getCollision();

    V put(const K& key, const V& value);
    V get(const K& key);
    V remove(const K& key);

    void clear();

    bool containsKey(const K& key);
    bool isEmpty();
};



template <typename K, typename V, typename H, typename H2>
HashMapDH<K, V, H, H2>::HashMapDH() : _capacity(DEFAULT_CAPACITY), _loadFactor(DEFAULT_LOAD_FACTOR), _size(0) {
    _array = new HashNode2<K, V>[_capacity]();

    for (size_t i = 0; i < _capacity; i++) {
        _array[i].setStatus('f');
    }
}

template <typename K, typename V, typename H, typename H2 >
HashMapDH<K, V, H, H2>::HashMapDH(size_t capacity) : _loadFactor(DEFAULT_LOAD_FACTOR), _size(0) {
    _capacity = getNextPrime(capacity);
    _array = new HashNode2<K, V>[_capacity];

    for (size_t i = 0; i < _capacity; i++) {
        _array[i].setStatus('f');
    }
}

template <typename K, typename V, typename H, typename H2 >
HashMapDH<K, V, H, H2>::HashMapDH(size_t capacity, float loadFactor) : _loadFactor(loadFactor), _size(0) {
    _capacity = getNextPrime(capacity);
    _array = new HashNode2<K, V>[_capacity];

    for (size_t i = 0; i < _capacity; i++) {
        _array[i].setStatus('f');
    }
}

template <typename K, typename V, typename H, typename H2 >
HashMapDH<K, V, H, H2>::~HashMapDH() {
    delete[]_array;
}

template <typename K, typename V, typename H, typename H2 >
size_t HashMapDH<K, V, H, H2>::getCapacity() { return _capacity; }

template <typename K, typename V, typename H, typename H2 >
size_t HashMapDH<K, V, H, H2>::getSize() { return _size; }

template <typename K, typename V, typename H, typename H2 >
float HashMapDH<K, V, H, H2>::getLoadFactor() { return _loadFactor; }

template <typename K, typename V, typename H, typename H2 >
bool HashMapDH<K, V, H, H2>::isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i <= sqrt(n); i = i + 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

template <typename K, typename V, typename H, typename H2 >
int HashMapDH<K, V, H, H2>::getNextPrime(int capacity) {
    if (capacity <= 1) return 2;
    int prime = capacity;
    bool found = false;
    while (!found) {
        if (isPrime(prime)) {
            found = true;
        }
        prime++;
    }
    return prime-1;
}

template <typename K, typename V, typename H, typename H2 >
V HashMapDH<K, V, H, H2>::put(const K& key, const V& value) {
    size_t hashValue = _hasher(key) % _capacity;

    while (_array[hashValue].getStatus() != 'f'){
        if (_array[hashValue].getKey() == key) { break; }
        _collision = _collision + 1;
        hashValue = (hashValue + (_hasher2(key) % (_capacity - 1))) % _capacity;
    }
    if (_array[hashValue].getKey() != key) {
        _array[hashValue] = HashNode2<K, V>(key, value);
        _size++;
        if (this->threshold() < _size) { this->rehash(); }
        return V(0);
    }
    V rtnValue = _array[hashValue].getValue();
    _array[hashValue].setValue(value);
    return rtnValue;
}

template <typename K, typename V, typename H, typename H2 >
V HashMapDH<K, V, H, H2>::get(const K& key) {
    size_t hashValue = _hasher(key) % _capacity;
    while (_array[hashValue].getStatus() != 'f') {
        if (_array[hashValue].getKey() == key) {
            return _array[hashValue].getValue();
        }
        hashValue = (hashValue + _hasher2(key) % (_capacity - 1)) % _capacity;
    }
    throw std::out_of_range("KeyError: Given key does not exist in map");
}

template <typename K, typename V, typename H, typename H2 >
V HashMapDH<K, V, H, H2>::remove(const K& key) {
    int hashValue = _hasher(key) % _capacity;

    while (_array[hashValue].getStatus() != 'f') {
        if (_array[hashValue].getKey() == key) {
            _array[hashValue].setStatus('a');
            V rtnValue = _array[hashValue].getValue();
            _array[hashValue].setValue(V());
            _array[hashValue].setKey(K());
            _size--;
            return rtnValue;
        }
        hashValue = (hashValue + _hasher2(key) % (_capacity - 1)) % _capacity;
    }
    throw std::out_of_range("KeyError: Given key does not exist in map");
}

template <typename K, typename V, typename H, typename H2 >
int HashMapDH<K, V, H, H2>::getCollision() {
    return _collision;
}

template <typename K, typename V, typename H, typename H2 >
bool HashMapDH<K, V, H, H2>::containsKey(const K& key) {
    size_t hashValue = _hasher(key) % _capacity;

    while (_array[hashValue].getStatus() != 'f') {
        if (_array[hashValue].getKey() == key) {
            return true;
        }
        hashValue = (hashValue + _hasher2(key) % (_capacity - 1)) % _capacity;
    }
    return false;
}

template <typename K, typename V, typename H, typename H2 >
bool HashMapDH<K, V, H, H2>::isEmpty() { return _size == 0; }

template <typename K, typename V, typename H, typename H2 >
void HashMapDH<K, V, H, H2>::clear() {
    delete[] _array;
    _array = new HashNode2<K, V>[_capacity]();

    for (size_t i = 0; i < _capacity; i++) {
        _array[i].setStatus('f');
    }
    _size = 0;
    _collision = 0;
}

template <typename K, typename V, typename H, typename H2 >
size_t HashMapDH<K, V, H, H2>::threshold() { return static_cast<size_t>(_capacity * _loadFactor); 
     }

template <typename K, typename V, typename H, typename H2 >
void HashMapDH<K, V, H, H2>::rehash() {
    size_t prevCapacity = _capacity;
    _capacity = getNextPrime(_capacity * 2);
    HashNode2<K, V>* temp = _array;
    _array = new HashNode2<K, V>[_capacity]();
    _size = 0;

    for (size_t i = 0; i < _capacity; i++) {
        _array[i].setStatus('f');
    }

    for (size_t i = 0; i < prevCapacity; i++) {

        if (temp[i].getStatus() == 'o') {
            this->put(temp[i].getKey(), temp[i].getValue());
        }
    }
    delete[]temp;
}