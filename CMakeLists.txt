cmake_minimum_required(VERSION 2.8.7)
project(vie)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.54.0 COMPONENTS iterator)

include_directories(include)
include_directories(test/catch/include)
include_directories(${Boost_INCLUDE_DIRS})

# use c++14 and specify flags manually for travis
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(HEADER_FILES vie/file_iterator.hpp
                 vie/forward_list.hpp
                 vie/list.hpp
                 vie/vector.hpp)
set(SOURCE_FILES test/test_file_iterator.cpp
                 test/test_forward_list.cpp
                 test/test_list.cpp
                 test/test_vector.cpp)

add_executable(vie ${SOURCE_FILES})
target_link_libraries(vie ${Boost_LIBRARIES})
