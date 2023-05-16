#pragma once

#include <iostream>

template <typename K>
class Hasher2 {
public:
    size_t operator()(const K& key) const {
        return static_cast<size_t>(key);
    }
};