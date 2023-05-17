#pragma once

#include "HashMapEntry.h"

#include <iostream>

template<typename K, typename V>
class HashMapEntryRH : public HashMapEntry<K, V> {
private:
    size_t _psl;

public:
    HashMapEntryRH(const K &key, const V &value);
    ~HashMapEntryRH();

    size_t getPSL();
    void setPSL(const size_t &psl);
};

template<typename K, typename V>
HashMapEntryRH<K, V>::HashMapEntryRH(const K &key, const V &value) : HashMapEntry<K, V>(key, value), _psl(0) {}

template<typename K, typename V>
HashMapEntryRH<K, V>::~HashMapEntryRH() = default;

template<typename K, typename V>
size_t HashMapEntryRH<K, V>::getPSL() { return _psl; }

template<typename K, typename V>
void HashMapEntryRH<K, V>::setPSL(const size_t &psl) { _psl = psl; }
