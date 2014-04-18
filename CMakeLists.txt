cmake_minimum_required (VERSION 2.6)
project (cpp_metrics)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -g -pthread")

set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS system unit_test_framework chrono thread serialization REQUIRED)

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

  
INCLUDE_DIRECTORIES(${CPP-NETLIB_SOURCE_DIR}/config/src
  ${CPP-NETLIB_SOURCE_DIR}/message/src
  ${CPP-NETLIB_SOURCE_DIR}/uri/src
  ${CPP-NETLIB_SOURCE_DIR}/logging/src
  ${CPP-NETLIB_SOURCE_DIR}/http/src)

ADD_SUBDIRECTORY(cpp-netlib)



AUX_SOURCE_DIRECTORY(src METRICS_SRC)
message(STATUS ${METRICS_SRC})

AUX_SOURCE_DIRECTORY(test METRICS_TEST)

ADD_LIBRARY(metrics ${METRICS_SRC})
TARGET_LINK_LIBRARIES(metrics
                      ${Boost_LIBRARIES}
				    cppnetlib-uri
				    #network-http-v2-client
				    cppnetlbi-client-connections
				    )

INCLUDE_DIRECTORIES(src)
INCLUDE_DIRECTORIES(cpp-netlib)
ADD_EXECUTABLE(unittest ${METRICS_TEST})
ADD_DEPENDENCIES(unittest network-http-message)
TARGET_LINK_LIBRARIES(unittest
                      ${Boost_LIBRARIES}
				    cppnetlib-uri
				    #network-http-v2-client
				    cppnetlib-client-connections
				    #network-http-message
				    #network-constants
				    #network-http-message-wrappers
				    )