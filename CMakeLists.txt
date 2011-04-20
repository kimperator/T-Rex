cmake_minimum_required(VERSION 2.8)
project(crul C)

ADD_LIBRARY(trex STATIC trex)

SET(BUILD_TREX_TESTS ON CACHE BOOL "Build Tests")

if(BUILD_TREX_TESTS)
	ADD_EXECUTABLE(trex_test test)
	TARGET_LINK_LIBRARIES(trex_test trex)
endif(BUILD_TREX_TESTS)
