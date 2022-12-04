set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

cmake_minimum_required(VERSION 3.25)

project(Appetito)

add_definitions(-DASIO_STANDALONE)

include_directories(./libs/asio/include)
include_directories(./libs/client/include)
include_directories(./libs/server/include)
include_directories(./libs/include)

add_library(client_lib STATIC libs/client/include/client.hpp libs/client/src/client.cpp)
add_library(red_lib STATIC libs/server/include/redirector.hpp libs/server/src/redirector.cpp)
add_library(concli_lib STATIC libs/server/include/connected_client.hpp libs/server/src/connected_client.cpp)
add_library(server_lib STATIC libs/server/include/server.hpp libs/server/src/server.cpp)

add_executable(client src/Client1.cpp)
target_link_libraries(client client_lib)

add_executable(server src/Server1.cpp)
target_link_libraries(concli_lib red_lib)
target_link_libraries(red_lib concli_lib)
target_link_libraries(server_lib red_lib)
target_link_libraries(server_lib concli_lib)
target_link_libraries(server server_lib)

if(MINGW)
  target_link_libraries(client_lib ws2_32)
  target_link_libraries(red_lib ws2_32)
  target_link_libraries(concli_lib ws2_32)
  target_link_libraries(server_lib ws2_32)
  target_link_libraries(client ws2_32)
  target_link_libraries(server ws2_32)
  target_link_libraries(server wsock32)
endif()