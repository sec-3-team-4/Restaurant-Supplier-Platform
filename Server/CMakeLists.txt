cmake_minimum_required(VERSION 3.10)

project(Server)

set(CMAKE_CXX_STANDARD 17)

include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

add_executable(server
    main.cpp
    server.cpp
    UserManager.cpp
    JsonValidator.cpp
    MessageRouter.cpp
)

enable_testing()

find_package(GTest REQUIRED)

add_executable(server_tests
    tests/server_tests.cpp
    UserManager.cpp
    JsonValidator.cpp
    MessageRouter.cpp
)

target_include_directories(server_tests PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(server_tests
    GTest::gtest
    GTest::gtest_main
    pthread
)

add_test(NAME ServerTests COMMAND server_tests)

