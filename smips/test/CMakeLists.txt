cmake_minimum_required(VERSION 3.2)

enable_testing()

find_package(Gurobi REQUIRED)
include_directories(${GUROBI_INCLUDE_DIRS})
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(data_tests
        test_lands.cpp
        test_sslp.cpp)

target_link_libraries(data_tests gtest_main gtest pthread)
add_test(DataTests data_tests)
