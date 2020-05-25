cmake_minimum_required(VERSION 3.2)

add_subdirectory(core)
add_subdirectory(stoch)
add_subdirectory(time)

add_library(smpslib smps.cpp)
target_link_libraries(smpslib core time stoch)
