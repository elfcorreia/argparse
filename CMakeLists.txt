cmake_minimum_required(VERSION 3.10)
project(argparse)

set(CMAKE_CXX_FLAGS "-std=c++17 -g -Wall")
set(CMAKE_INSTALL_PREFIX /usr)

add_executable(main main.cpp)

install(FILES argparse.hpp DESTINATION include)
