cmake_minimum_required(VERSION 3.2)
project(FangRunner)

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

#Add -mwindows to remove console window
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -o3")

set(SOURCE_FILES src/main.cpp)
add_executable(FangRunner ${SOURCE_FILES})

target_link_libraries(FangRunner ${CMAKE_CURRENT_SOURCE_DIR}/lib/libsfml-graphics.a
                              ${CMAKE_CURRENT_SOURCE_DIR}/lib/libsfml-window.a
                              ${CMAKE_CURRENT_SOURCE_DIR}/lib/libsfml-audio.a
                              ${CMAKE_CURRENT_SOURCE_DIR}/lib/libsfml-network.a
                              ${CMAKE_CURRENT_SOURCE_DIR}/lib/libsfml-system.a)