#include "../src/HashMapLL.h"

#include <catch2/catch_test_macros.hpp>

HashMapLL<int, int> *map;

TEST_CASE("Creating HashMapLL of given capacities and load factors", "[HashMapLL]") {
    SECTION("Using default capacity") {
        map = new HashMapLL<int, int>();
        REQUIRE(map->isEmpty());
        REQUIRE(map->getCapacity() == 32);
        REQUIRE(map->getSize() == 0);
    }

    SECTION("Setting the HashMapLL capacity equal to 128") {
        map = new HashMapLL<int, int>(128);
        REQUIRE(map->getCapacity() == 128);
        REQUIRE(map->getSize() == 0);
    }

    SECTION("Setting the HashMapLL capacity equal to 64 and load factor equal to 0.5") {
        map = new HashMapLL<int, int>(64, 0.5f);
        REQUIRE(map->getCapacity() == 64);
        REQUIRE(map->getLoadFactor() == 0.5f);
    }
}

TEST_CASE("Adding and getting elements from HashMapLL", "[HashMapLL]") {
    map = new HashMapLL<int, int>();

    SECTION("Adding non existing keys") {
        map->put(1, 100);
        map->put(2, 200);

        REQUIRE_FALSE(map->isEmpty());
        REQUIRE(map->getSize() == 2);
        REQUIRE(map->get(1) == 100);
        REQUIRE(map->get(2) == 200);
    }

    SECTION("Checking returns when adding non existing/existing keys") {
        REQUIRE(map->put(1, 100) == 0);
        REQUIRE(map->put(1, 1000) == 100);
    }

    SECTION("Checking handling collisions") {
        map->put(1, 100);
        map->put(33, 1000);
        map->put(65, 10000);

        REQUIRE(map->get(1) == 100);
        REQUIRE(map->get(33) == 1000);
        REQUIRE(map->get(65) == 10000);
    }

    SECTION("Exception when getting non existing elements") {
        REQUIRE_THROWS_AS(map->get(1), std::out_of_range);
        REQUIRE_THROWS_AS(map->get(200), std::out_of_range);
    }

    SECTION("Adding elements after clearing HashMapLL") {
        map->put(1, 100);
        map->put(33, 1000);
        map->put(2, 200);
        map->clear();

        map->put(1, 100);
        REQUIRE(map->get(1) == 100);

        map->put(33, 1000);
        REQUIRE(map->get(33) == 1000);
    }
}

TEST_CASE("Removing from HashMapLL", "[HashMapLL]") {
    map = new HashMapLL<int, int>();
    map->put(1, 100);
    map->put(2, 200);
    map->put(3, 300);

    map->put(33, 1000);
    map->put(65, 10000);

    SECTION("Elements that did not cause a collision") {
        REQUIRE(map->getSize() == 5);
        map->remove(2);
        map->remove(3);

        REQUIRE_FALSE(map->containsKey(2));
        REQUIRE_FALSE(map->containsKey(3));
        REQUIRE(map->getSize() == 3);
    }

    SECTION("Elements that did cause a collision") {
        map->remove(1);
        REQUIRE_FALSE(map->containsKey(1));

        map->put(1, 100);
        map->remove(65);
        REQUIRE_FALSE(map->containsKey(65));

        map->remove(1);
        REQUIRE_FALSE(map->containsKey(1));
    }

    SECTION("Clearing all elements in HashMapLL") {
        map->clear();
        REQUIRE(map->getSize() == 0);
    }

    SECTION("Removing elements that does not exist") {
        REQUIRE_THROWS_AS(map->remove(1000), std::out_of_range);
        REQUIRE_THROWS_AS(map->remove(5000), std::out_of_range);
    }
}

TEST_CASE("Rehashing HashMapLL after exceeded threshold", "[HashMapLL]") {
    map = new HashMapLL<int, int>(8);
    map->put(1, 100);
    map->put(2, 200);
    map->put(3, 300);
    map->put(4, 400);

    SECTION("Rehashing without collisions") {
        map->put(5, 500);
        map->put(6, 600);
        map->put(7, 700);

        REQUIRE(map->getCapacity() == 16);
        REQUIRE(map->getSize() == 7);
        REQUIRE(map->containsKey(4));
        REQUIRE(map->get(4) == 400);
    }

    SECTION("Rehashing with collisions") {
        map->put(33, 330);
        map->put(65, 650);
        map->put(17, 170);

        REQUIRE(map->getCapacity() == 16);
        REQUIRE(map->getSize() == 7);
        REQUIRE(map->containsKey(65));
        REQUIRE(map->get(65) == 650);
    }
}