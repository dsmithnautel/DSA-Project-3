cmake_minimum_required(VERSION 3.30)
project(Project3_DSA)

set(CMAKE_CXX_STANDARD 20)

add_executable(Project3_DSA src/main.cpp
        src/DataLoader.h
        src/Movie.h
        src/User.h
        src/Graph.h
        src/HashTable.h
        src/DataLoader.cpp
        src/User.cpp
        src/HashTable.cpp
        src/Graph.cpp)
