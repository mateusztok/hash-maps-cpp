Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.3.2
)

FetchContent_MakeAvailable(Catch2)

add_executable(HashMapLLTest HashMapLL.test.cpp)
add_executable(HashMapDHTest HashMapDH.test.cpp)

set(ALL_EXAMPLE_TARGETS
        HashMapLLTest
        HashMapDHTest
        )

foreach( name ${ALL_EXAMPLE_TARGETS} )
    target_link_libraries( ${name} Catch2 Catch2WithMain )
endforeach()