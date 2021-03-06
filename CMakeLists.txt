cmake_minimum_required (VERSION 2.6)
project (cpp_metrics)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -g -pthread")

set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system unit_test_framework chrono thread serialization date_time regex REQUIRED)

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

file(GLOB_RECURSE METRICS_H src "*.h")
file(GLOB_RECURSE METRICS_HPP src "*.hpp")
file(GLOB_RECURSE METRICS_CPP rc "*.cpp")

AUX_SOURCE_DIRECTORY(test METRICS_TEST)

ADD_LIBRARY(metrics ${METRICS_H} ${METRICS_CPP} ${METRICS_HPP})
TARGET_LINK_LIBRARIES(metrics ${Boost_LIBRARIES})

INCLUDE_DIRECTORIES(src)

ADD_EXECUTABLE(unittest ${METRICS_TEST})

TARGET_LINK_LIBRARIES(unittest ${Boost_LIBRARIES})