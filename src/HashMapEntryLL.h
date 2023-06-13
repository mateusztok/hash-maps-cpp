#pragma once

#include "HashMapEntry.h"

template <typename K, typename V>
class HashMapEntryLL : public HashMapEntry<K, V> {
private:
    HashMapEntryLL *_next;

public:
    HashMapEntryLL(const K &key, const V &value);
    ~HashMapEntryLL();

    HashMapEntryLL *getNext();
    void setNext(HashMapEntryLL *next);
};

template <typename K, typename V>
HashMapEntryLL<K, V>::HashMapEntryLL(const K &key, const V &value) : HashMapEntry<K, V>(key, value), _next(nullptr) {}

template <typename K, typename V>
HashMapEntryLL<K, V>::~HashMapEntryLL() = default;

template <typename K, typename V>
HashMapEntryLL<K, V> *HashMapEntryLL<K, V>::getNext() { return _next; }

template <typename K, typename V>
void HashMapEntryLL<K, V>::setNext(HashMapEntryLL<K, V> *next) { _next = next; }