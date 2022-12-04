#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <iostream>
#include <array>
#include <deque>
#include <asio.hpp>
#include <protocol.hpp>
using asio::ip::tcp;
using Pack = std::array<char, MAX_PACK_SIZE>; // Pack is an array of char, for transmitting pack
using ClientID = std::array<char, MAX_ID_SIZE>; // ID is an array of char, for identifying the client

enum IDs {
    customer_id = 0,
    kitchen_id
};

/*
    CLIENT CLASS
    Specific ID, connects to server, sends and receives packs
*/
class Client {
public:
    Client(const ClientID& client_id,
            asio::io_context& io_context,
            tcp::resolver::iterator endpoint_iterator)
        :io_context_(io_context), socket_(io_context)
        {
            strcpy(client_id_.data(), client_id.data());
            memset(read_pack_.data(), '\0', MAX_PACK_SIZE);
            asio::async_connect(socket_, endpoint_iterator, std::bind(&Client::on_connect, this, std::placeholders::_1)); // Connect to server
        }
    void write(const Pack& pack);
    void close();
private:
    void on_connect(const asio::error_code& error);
    void read_handler(const asio::error_code& error);
    void write_handler(const asio::error_code& error);
    void write_implementation(Pack pack);
    void close_implementation();
    asio::io_context& io_context_;
    tcp::socket socket_;
    ClientID client_id_;
    Pack read_pack_;
    std::deque<Pack> packs_to_write_;
};

#endif