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

