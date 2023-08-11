cmake_minimum_required(VERSION 3.0.0)
file(GLOB NORMAL_TEST_CODES LIST_DIRECTORIES false ${CMAKE_CURRENT_SOURCE_DIR}/*_test.cpp)
AddTest(normal-test ${NORMAL_TEST_CODES})
