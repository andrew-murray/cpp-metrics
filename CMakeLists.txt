cmake_minimum_required (VERSION 2.6)
project (cpp_metrics)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -g")

set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS system unit_test_framework chrono thread serialization REQUIRED)

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

AUX_SOURCE_DIRECTORY(src METRICS_SRC)
message(STATUS ${METRICS_SRC})

AUX_SOURCE_DIRECTORY(test METRICS_TEST)

ADD_LIBRARY(metrics ${METRICS_SRC})
INCLUDE_DIRECTORIES(src)
ADD_EXECUTABLE(unittest ${METRICS_TEST})
TARGET_LINK_LIBRARIES(unittest
                      ${Boost_LIBRARIES})