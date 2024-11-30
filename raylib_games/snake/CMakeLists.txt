cmake_minimum_required(VERSION 3.17)
project(Snake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    snake.cpp
)

find_package(raylib REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} raylib dl pthread)

file(COPY ${PROJECT_SOURCE_DIR}/sounds DESTINATION ${CMAKE_BINARY_DIR})