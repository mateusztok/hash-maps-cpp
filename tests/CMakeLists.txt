Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.3.2
)

FetchContent_MakeAvailable(Catch2)

add_executable(HashMapLLTest HashMapLL.test.cpp)
add_executable(HashMapDHTest HashMapDH.test.cpp)
add_executable(HashMapRHTest HashMapRH.test.cpp)

set(ALL_TARGETS
        HashMapLLTest
        HashMapDHTest
        HashMapRHTest
        )

foreach(name ${ALL_TARGETS})
    target_link_libraries(${name} PRIVATE Catch2::Catch2WithMain)
endforeach()