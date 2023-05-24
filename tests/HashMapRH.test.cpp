#include "../src/HashMapRH.h"

#include <catch2/catch_test_macros.hpp>

HashMapRH<int, int> *map;

TEST_CASE("Creating HashMapRH of given capacities and load factors", "[HashMapRH]") {
    SECTION("Using default capacity") {
        map = new HashMapRH<int, int>();
        REQUIRE(map->isEmpty());
        REQUIRE(map->getCapacity() == 32);
        REQUIRE(map->getSize() == 0);
    }

    SECTION("Setting the HashMapRH capacity equal to 128") {
        map = new HashMapRH<int, int>(128);
        REQUIRE(map->getCapacity() == 128);
        REQUIRE(map->getSize() == 0);
    }

    SECTION("Setting the HashMapRH capacity equal to 64 and load factor equal to 0.5") {
        map = new HashMapRH<int, int>(64, 0.5f);
        REQUIRE(map->getCapacity() == 64);
        REQUIRE(map->getLoadFactor() == 0.5f);
    }
}

TEST_CASE("Adding and getting elements from HashMapRH", "[HashMapRH]") {
    map = new HashMapRH<int, int>();

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
        map->put(129, 100000);

        map->put(6, 600);
        map->put(38, 6000);

        map->put(31, 120);
        map->put(63, 140);

        map->put(2, 200);

        REQUIRE(map->get(1) == 100);
        REQUIRE(map->get(33) == 1000);
        REQUIRE(map->get(65) == 10000);
        REQUIRE(map->get(129) == 100000);

        REQUIRE(map->get(6) == 600);
        REQUIRE(map->get(38) == 6000);

        REQUIRE(map->get(31) == 120);
        REQUIRE(map->get(63) == 140);

        REQUIRE(map->get(2) == 200);
    }

    SECTION("Exception when getting non existing elements") {
        REQUIRE_THROWS_AS(map->get(1), std::out_of_range);
        REQUIRE_THROWS_AS(map->get(200), std::out_of_range);
    }

    SECTION("Adding elements after clearing HashMapRH") {
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

TEST_CASE("Removing from HashMapRH", "[HashMapRH]") {
    map = new HashMapRH<int, int>();
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

        map->put(31, 310);
        map->put(63, 630);
        map->remove(31);
        REQUIRE_FALSE(map->containsKey(31));
    }

    SECTION("Clearing all elements in HashMapRH") {
        map->clear();
        REQUIRE(map->getSize() == 0);
    }

    SECTION("Removing elements that does not exist") {
        REQUIRE_THROWS_AS(map->remove(1000), std::out_of_range);
        REQUIRE_THROWS_AS(map->remove(5000), std::out_of_range);
    }
}

TEST_CASE("Rehashing HashMapRH after exceeded threshold", "[HashMapRH]") {
    map = new HashMapRH<int, int>(8);
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
        map->put(33, 1000);
        map->put(65, 650);
        map->put(129, 1290);

        REQUIRE(map->getCapacity() == 16);
        REQUIRE(map->getSize() == 7);
        REQUIRE(map->containsKey(65));
        REQUIRE(map->get(65) == 650);
        REQUIRE(map->containsKey(129));
        REQUIRE(map->get(129) == 1290);
    }
}