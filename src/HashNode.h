#pragma once

template <typename K, typename V>
class HashNode {
private:
    K _key;
    V _value;
    HashNode *_next;

public:
    HashNode(const K &key, const V &value);
    HashNode(const K &key, const V &value, HashNode *next);
    ~HashNode();

    K getKey();
    V getValue();
    HashNode *getNext();
    void setValue(V value);
    void setNext(HashNode *next);
};

template <typename K, typename V>
HashNode<K, V>::HashNode(const K &key, const V &value) : _key(key), _value(value), _next(nullptr) {}

template <typename K, typename V>
HashNode<K, V>::HashNode(const K &key, const V &value, HashNode *next) : _key(key), _value(value), _next(next) {}

template <typename K, typename V>
HashNode<K, V>::~HashNode() = default;

template <typename K, typename V>
K HashNode<K, V>::getKey() { return _key; }

template <typename K, typename V>
V HashNode<K, V>::getValue() { return _value; }

template <typename K, typename V>
HashNode<K, V> *HashNode<K, V>::getNext() { return _next; }

template <typename K, typename V>
void HashNode<K, V>::setValue(V value) { _value = value; }

template <typename K, typename V>
void HashNode<K, V>::setNext(HashNode<K, V> *next) { _next = next; }