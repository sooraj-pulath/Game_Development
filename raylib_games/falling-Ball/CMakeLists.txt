cmake_minimum_required(VERSION 3.17)
project(Fallingball)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cpp
    src/game.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/include)

find_package(raylib REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} raylib dl pthread)

file(COPY ${PROJECT_SOURCE_DIR}/sounds DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})