#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <iostream>
#include <array>
#include <deque>
#include <networkio.hpp>
#include <asio.hpp>
#include <protocol.hpp>
#include <cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/array.hpp>
using asio::ip::tcp;
using Pack = std::array<char, MAX_PACK_SIZE>; // Pack is an array of char, for transmitting pack
using ClientID = std::array<char, MAX_ID_SIZE>; // ID is an array of char, for identifying the client

enum IDs {
    customer_id = 1,
    kitchen_id
};

/*
    CLIENT CLASS
    Specific ID, connects to server, sends and receives packs
*/
class Client {
public:
    Client(const std::string& client_id,
            asio::io_context& io_context,
            tcp::resolver::iterator endpoint_iterator)
        :io_context_(io_context), network_io_(io_context), client_id_(client_id)
        {
            asio::async_connect(network_io_.socket(), endpoint_iterator, std::bind(&Client::on_connect, this, std::placeholders::_1)); // Connect to server
        }
    void write(const std::string& object);
    void close();
private:
    void on_connect(const asio::error_code& error);
    void connect_handler(const asio::error_code& error);
    void read_handler(const asio::error_code& error);
    void write_implementation(std::string object);
    void write_handler(const asio::error_code& error);
    void close_implementation();
    asio::io_context& io_context_;
    std::string client_id_;
    std::deque<std::string> strings_to_write_;
    std::string read_string_;
    NetworkIO network_io_;
};

#endif