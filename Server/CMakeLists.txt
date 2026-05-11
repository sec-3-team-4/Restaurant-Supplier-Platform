cmake_minimum_required(VERSION 3.16)

project(Milestone2Server)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED)

add_executable(server
    main.cpp
    server.cpp
)

target_include_directories(server PRIVATE
    /opt/homebrew/include
)

target_link_libraries(server PRIVATE Boost::boost)

