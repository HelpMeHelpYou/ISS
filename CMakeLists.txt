cmake_minimum_required(VERSION 2.8)

project(ISS)

set (CMAKE_CXX_STANDARD 11)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

set (SOURCE "test.cpp" "Receiver.cpp" "TestReceiver.cpp")

add_executable( runUnitTests ${SOURCE}  )
target_link_libraries(runUnitTests gtest gtest_main gmock gmock_main)
target_link_libraries(runUnitTests Threads::Threads)

add_test( runUnitTests runUnitTests )

