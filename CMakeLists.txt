project(tiny_infer)

cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(common INTERFACE)
target_include_directories(common INTERFACE ${CMAKE_SOURCE_DIR})

set(timer_src timer.cpp)

add_executable(op_tile op_tile.cpp ${timer_src})
target_link_libraries(op_tile common)

add_executable(op_transpose op_transpose.cpp ${timer_src})
target_link_libraries(op_transpose common)

add_executable(op_pooling op_pooling.cpp ${timer_src})
target_link_libraries(op_pooling common)

add_executable(op_transpose_c3 op_transpose_c3.cpp ${timer_src})
target_link_libraries(op_transpose_c3 common)