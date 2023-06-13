#pragma once

#include "HashMapEntry.h"

template <typename K, typename V>
class HashMapEntryDH : public HashMapEntry<K, V> {
private:
	char _status;   //'f'=free,'a'=accessed,'o'occupied

public:
	HashMapEntryDH();
	HashMapEntryDH(const K& key, const V& value);
	~HashMapEntryDH();

	char getStatus();
	void setStatus(char status);
};
template <typename K, typename V>
HashMapEntryDH<K, V>::HashMapEntryDH() : HashMapEntry<K, V>(K(), V()), _status('f') {}

template <typename K, typename V>
HashMapEntryDH<K, V>::HashMapEntryDH(const K& key, const V& value) : HashMapEntry<K, V>(key, value), _status('o') {}

template <typename K, typename V>
HashMapEntryDH<K, V>::~HashMapEntryDH() = default;


template <typename K, typename V>
char HashMapEntryDH<K, V>::getStatus() { return _status; }

template <typename K, typename V>
void HashMapEntryDH<K, V>::setStatus(char status) { _status = status; }

