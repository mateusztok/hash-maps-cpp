#pragma once
template <typename K, typename V>
class HashNode2
{
private:
	K _key;
	V _value;
	char _status;   //'f'=free,'a'=accessed,'o'occupied
public:
	HashNode2(const K& key, const V& value, const char& status);
	~HashNode2();

	K getKey();
	V getValue();
	char getStatus();
	void setKey(K key);
	void setValue(V value);
	void setStatus(char status);
};
template <typename K, typename V>
HashNode2<K, V>::HashNode2(const K& key, const V& value, const char& status) : _key(key), _value(value), _status('o') {}

template <typename K, typename V>
HashNode2<K, V>::~HashNode2() = default;

template <typename K, typename V>
K HashNode2<K, V>::getKey() { return _key; }

template <typename K, typename V>
char HashNode2<K, V>::getStatus() { return _status; }

template <typename K, typename V>
V HashNode2<K, V>::getValue() { return _value; }

template <typename K, typename V>
void HashNode2<K, V>::setValue(V value) { _value = value; }

template <typename K, typename V>
void HashNode2<K, V>::setKey(K key) { _key = key; }

template <typename K, typename V>
void HashNode2<K, V>::setStatus(char status) { _status = status; }

