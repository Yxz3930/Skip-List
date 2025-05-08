cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)

# 编译选项
add_compile_options(-Wall -O2)
# add_compile_options(-Wall)

# 设置库和可执行文件输出路径 在preject之前
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/output)

project(SkipListTemplate)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB src_files src/*.cpp)

add_executable(SkipListTemplate_Test ${CMAKE_CURRENT_SOURCE_DIR}/test/SkipListTemplate_Test.cpp ${src_files})

add_executable(stress_test ${CMAKE_CURRENT_SOURCE_DIR}/test/stress_test.cpp ${src_files})
