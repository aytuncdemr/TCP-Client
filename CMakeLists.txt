cmake_minimum_required(VERSION 3.10.0)

project(TCPClient)

add_executable(TCPClient main.cpp ./src/Client.cpp)

target_include_directories(TCPClient PRIVATE ./include)

target_link_libraries(TCPClient wsock32)