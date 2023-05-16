#include "../src/HashMapDH.h"

#include <catch2/catch_test_macros.hpp>

HashMapDH<int, int>* map;

TEST_CASE("Creating HashMapDH of given capacities and load factors", "[HashMapDH]") {
    SECTION("Using default capacity") {
        map = new HashMapDH<int, int>();
        REQUIRE(map->isEmpty());
        REQUIRE(map->getCapacity() == 31);
        REQUIRE(map->getSize() == 0);
    }

    SECTION("Setting the HashMapDH capacity equal to 128") {
        map = new HashMapDH<int, int>(128);
        REQUIRE(map->getCapacity() == 131);
        REQUIRE(map->getSize() == 0);
    }

    SECTION("Setting the HashMapDH capacity equal to 64 and load factor equal to 0.5") {
        map = new HashMapDH<int, int>(64, 0.5f);
        REQUIRE(map->getCapacity() == 67);
        REQUIRE(map->getLoadFactor() == 0.5f);
    }
}

TEST_CASE("Adding and getting elements from HashMapDH", "[HashMapDH]") {
    map = new HashMapDH<int, int>();

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
        map->put(32, 1000);
        map->put(63, 10000);

        REQUIRE(map->get(1) == 100);
        REQUIRE(map->get(33) == 1000);
        REQUIRE(map->get(65) == 10000);
    }

    SECTION("Exception when getting non existing elements") {
        REQUIRE_THROWS_AS(map->get(1), std::out_of_range);
        REQUIRE_THROWS_AS(map->get(200), std::out_of_range);
    }

    SECTION("Adding elements after clearing HashMapDH") {
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

TEST_CASE("Removing from HashMapDH", "[HashMapDH]") {
    map = new HashMapDH<int, int>();
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

    SECTION("Clearing all elements in HashMapDH") {
        map->clear();
        REQUIRE(map->getSize() == 0);
    }

    SECTION("Removing elements that does not exist") {
        REQUIRE_THROWS_AS(map->remove(1000), std::out_of_range);
        REQUIRE_THROWS_AS(map->remove(5000), std::out_of_range);
    }
}

TEST_CASE("Rehashing HashMapDH after exceeded threshold", "[HashMapDH]") {
    map = new HashMapDH<int, int>(6);
    map->put(1, 100);
    map->put(2, 200);
    map->put(3, 300);
    map->put(4, 400);

    SECTION("Rehashing without collisions") {
        map->put(5, 500);
        map->put(6, 600);
        map->put(7, 700);

        REQUIRE(map->getCapacity() == 17);
        REQUIRE(map->getSize() == 7);
        REQUIRE(map->containsKey(4));
        REQUIRE(map->get(4) == 400);
    }

    SECTION("Rehashing with collisions") {
        map->put(33, 330);
        map->put(65, 650);
        map->put(17, 170);

        REQUIRE(map->getCapacity() == 17);
        REQUIRE(map->getSize() == 7);
        REQUIRE(map->containsKey(65));
        REQUIRE(map->get(65) == 650);
    }
}