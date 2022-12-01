set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

cmake_minimum_required(VERSION 3.25)

project(Appetito)

add_definitions(-DASIO_STANDALONE)

include_directories(./libs/asio/include)

add_executable(main src/main.cpp)

if(MINGW)
  target_link_libraries(main ws2_32)
endif()