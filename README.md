# hash-maps-cpp

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=flat&logo=c%2B%2B&logoColor=white)
[![ci](https://github.com/raczu/hash-maps-cpp/actions/workflows/ci.yml/badge.svg)](https://github.com/raczu/hash-maps-cpp/actions/workflows/ci.yml)

`hash-maps-cpp` is a project looking at ways of resolving collisions in hash maps. The aim of the project is to compare
the worst-case scenario for inserting, removing and looking up operations. Current implementation compares three
different ways of resolving collisions: separate chaining based on linked list, open addressing based on double hashing
and robin hood hashing.