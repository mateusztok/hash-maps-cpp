#pragma once

template <typename K, typename V>
class HashMapEntry {
protected:
    K _key;
    V _value;

public:
    HashMapEntry(const K &key, const V &value);
    ~HashMapEntry();

    K getKey();
    V getValue();
    void setValue(const V &value);
    void setKey(const K &key);
};

template <typename K, typename V>
HashMapEntry<K, V>::HashMapEntry(const K &key, const V &value) : _key(key), _value(value) {}

template <typename K, typename V>
HashMapEntry<K, V>::~HashMapEntry() = default;

template <typename K, typename V>
K HashMapEntry<K, V>::getKey() { return _key; }

template <typename K, typename V>
V HashMapEntry<K, V>::getValue() { return _value; }

template <typename K, typename V>
void HashMapEntry<K, V>::setValue(const V &value) { _value = value; }

template <typename K, typename V>
void HashMapEntry<K, V>::setKey(const K &key) { _key = key; }
