cmake_minimum_required(VERSION 3.30)
project(TeaPlusPlus)

set(CMAKE_CXX_STANDARD 14)

add_executable(TeaPlusPlus main.cpp
        displayBuffer.cpp
        displayBuffer.h
        physicsEntity.cpp
        physicsEntity.h
        sprite.cpp
        sprite.h
        physicsEngine.cpp
        physicsEngine.h
        entityType.h
        customTypes.h
        tileType.h
        levelLoader.h
        levelLoader.cpp
        tileType.cpp)
