cmake_minimum_required(VERSION 3.16)
project(ChatServer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)


add_executable(ChatServer
    main.cpp
    ClientSession.cpp
    MessageRouter.cpp
)

target_include_directories(ChatServer PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(ChatServer PRIVATE ${Boost_LIBRARIES})
