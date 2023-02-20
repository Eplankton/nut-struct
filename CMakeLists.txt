cmake_minimum_required(VERSION 3.0.0)
project(new VERSION 0.1.0)  # 项目名称

set(CMAKE_CXX_STANDARD 23)          # 设置 C/C++ 标准

set(SRC_LIST src/main.cpp)            # 设置源文件

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++2b")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -std=c++2b")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Og -g -std=c++2b -fsanitize=address -fsanitize=leak -fno-omit-frame-pointer")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build) # 设置目标二进制输出位置,

include_directories(${PROJECT_SOURCE_DIR}/include)    # 头文件路径

add_executable(main ${SRC_LIST})    # 生成可执行二进制文件 main

add_subdirectory(doctest)
add_subdirectory(nanobench)

target_link_libraries(main PRIVATE nanobench)