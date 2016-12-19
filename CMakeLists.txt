cmake_minimum_required(VERSION 2.8.7)
project(vie)

include_directories(include)
include_directories(test/catch)

set(CMAKE_CXX_STANDARD 14)
set(HEADER_FILES vie/file_iterator.hpp
                 vie/forward_list.hpp
                 vie/list.hpp
                 vie/vector.hpp)
set(SOURCE_FILES test/test_file_iterator.cpp
                 test/test_forward_list.cpp
                 test/test_list.cpp
                 test/test_vector.cpp)

add_executable(vie ${SOURCE_FILES})