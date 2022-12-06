#ifndef NETWORKIO_HPP_
#define NETWORKIO_HPP_

#include <iostream>
#include <asio.hpp>
#include <protocol.hpp>
using asio::ip::tcp;
using Pack = std::array<char, MAX_PACK_SIZE>; // Pack is an array of char, for transmitting pack
using ClientID = std::array<char, MAX_ID_SIZE>; // ID is an array of char, for identifying the client

class NetworkIO {
public:
    NetworkIO(asio::io_context& io_context)
              :socket_(io_context), buf_out_(MAX_PACK_SIZE), buf_in_(MAX_PACK_SIZE), outs(&buf_out_), ins(&buf_in_) { }
    tcp::socket socket_;
    asio::streambuf buf_out_;
    std::ostream outs;
    asio::streambuf buf_in_;
    std::istream ins;
};

class ClientNetworkIO : public NetworkIO {
public:
    void on_connect(const asio::error_code& error);
    void read_handler(const asio::error_code& error);
    void write_handler(const asio::error_code& error);
    void write_implementation(Pack pack);
    void close_implementation();
};

class ServerNetworkIO : public NetworkIO,
                        public std::enable_shared_from_this<ServerNetworkIO> {
public:
    ServerNetworkIO(asio::io_context& io_context,
                    asio::io_context::strand& strand)
                    :NetworkIO(io_context), strand_(strand) { }  
    void id_handler(const asio::error_code& error);
    void read_handler(const asio::error_code& error);
    void write_handler(const asio::error_code& error);
    asio::io_context::strand& strand_;
};

#endif