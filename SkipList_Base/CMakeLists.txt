cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)

# 编译选项
# add_compile_options(-Wall -O2)

# 设置库和可执行文件输出路径 在preject之前
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/output)

project(SkipListBase)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB src_files src/*.cpp)

add_executable(SkipListBase_Test ${CMAKE_CURRENT_SOURCE_DIR}/test/SkipListBase_Test.cpp ${src_files})
