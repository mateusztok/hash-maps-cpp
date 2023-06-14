cmake_minimum_required(VERSION 3.20)
project(hashmaps VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")

include(GNUInstallDirs)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set_target_properties(
        ${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION}
)

install(
        TARGETS ${PROJECT_NAME} EXPORT hashmapsConfig
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
install(EXPORT hashmapsConfig DESTINATION share/hashmaps/cmake)
export(TARGETS ${PROJECT_NAME} FILE hashmapsConfig.cmake)

enable_testing()
add_subdirectory(tests)
add_test(NAME HashMapLLTests COMMAND HashMapLLTest)
add_test(NAME HashMapDHTests COMMAND HashMapDHTest)
add_test(NAME HashMapRHTests COMMAND HashMapRHTest)