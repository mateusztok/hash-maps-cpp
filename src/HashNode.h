#pragma once

#include "HashMapEntry.h"

template <typename K, typename V>
class HashNode : public HashMapEntry<K, V> {
private:
    HashNode *_next;

public:
    HashNode(const K &key, const V &value);
    ~HashNode();

    HashNode *getNext();
    void setNext(HashNode *next);
};

template <typename K, typename V>
HashNode<K, V>::HashNode(const K &key, const V &value) : HashMapEntry<K, V>(key, value), _next(nullptr) {}

template <typename K, typename V>
HashNode<K, V>::~HashNode() = default;

template <typename K, typename V>
HashNode<K, V> *HashNode<K, V>::getNext() { return _next; }

template <typename K, typename V>
void HashNode<K, V>::setNext(HashNode<K, V> *next) { _next = next; }