cmake_minimum_required(VERSION 3.30)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

add_executable(untitled main.cpp
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
        levelLoader.cpp)
