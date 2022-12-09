set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

cmake_minimum_required(VERSION 3.25)

project(Appetito)

add_definitions(-DASIO_STANDALONE)

include_directories(./libs)
include_directories(./libs/asio/include)
include_directories(./libs/cereal)
include_directories(./libs/client/include)
include_directories(./libs/server/include)
include_directories(./libs/network/include)
include_directories(./libs/restaurant_util/include)

# Libraries

# Communication libraries
add_library(networkio_lib STATIC libs/network/include/networkio.hpp libs/network/src/networkio.cpp)
add_library(client_lib STATIC libs/client/include/client.hpp libs/client/src/client.cpp)
add_library(red_lib STATIC libs/server/include/redirector.hpp libs/server/src/redirector.cpp)
add_library(concli_lib STATIC libs/server/include/connected_client.hpp libs/server/src/connected_client.cpp)
add_library(server_lib STATIC libs/server/include/server.hpp libs/server/src/server.cpp)
target_link_libraries(client_lib networkio_lib)
target_link_libraries(concli_lib networkio_lib)
target_link_libraries(red_lib customer_menu_lib)
target_link_libraries(concli_lib red_lib)
target_link_libraries(red_lib concli_lib)
target_link_libraries(server_lib red_lib)
target_link_libraries(server_lib concli_lib)
# Restaurant Libraries
add_library(food_lib STATIC libs/restaurant_util/include/food.hpp libs/restaurant_util/src/food.cpp)
add_library(table_lib STATIC libs/restaurant_util/include/table.hpp libs/restaurant_util/src/table.cpp)
add_library(customer_menu_lib STATIC libs/restaurant_util/include/customer_menu.hpp libs/restaurant_util/src/customer_menu.cpp)
add_library(customer_order_deal_lib STATIC libs/restaurant_util/include/customer_order_deal.hpp libs/restaurant_util/src/customer_order_deal.cpp)
add_library(customer_notif_lib STATIC libs/restaurant_util/include/customer_notif.hpp libs/restaurant_util/src/customer_notif.cpp)
add_library(kitchen_request_lib STATIC libs/restaurant_util/include/kitchen_request.hpp libs/restaurant_util/src/kitchen_request.cpp)
target_link_libraries(customer_order_deal_lib food_lib)
target_link_libraries(customer_order_deal_lib table_lib)
target_link_libraries(customer_menu_lib food_lib)
target_link_libraries(customer_menu_lib table_lib)
# Executables

add_executable(client_customer src/client_customer.cpp)
target_link_libraries(client_customer client_lib)

#add_executable(client_kitchen src/client_kitchen.cpp)
#target_link_libraries(client_kitchen client_lib)

#add_executable(client_waiter src/client_waiter.cpp)
#target_link_libraries(client_waiter client_lib)

add_executable(server src/server_main.cpp)
target_link_libraries(server server_lib)

if(MINGW)
  target_link_libraries(client_lib ws2_32)
  target_link_libraries(red_lib ws2_32)
  target_link_libraries(concli_lib ws2_32)
  target_link_libraries(server_lib ws2_32)
  target_link_libraries(client_customer ws2_32)
  #target_link_libraries(client_kitchen ws2_32)
  #target_link_libraries(client_waiter ws2_32)
  target_link_libraries(server ws2_32)
  target_link_libraries(server wsock32)
endif()