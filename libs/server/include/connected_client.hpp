#ifndef CONNECTED_CLIENT_HPP_
#define CONNECTED_CLIENT_HPP_

#include <iostream>
#include <array>
#include <deque>
#include <asio.hpp>
#include <protocol.hpp>
using asio::ip::tcp;
using Pack = std::array<char, MAX_PACK_SIZE>; // Pack is an array of char, for transmitting pack
using ClientID = std::array<char, MAX_ID_SIZE>; // ID is an array of char, for identifying the client

class Redirector;

struct ServerNetworkIO {
    ServerNetworkIO(asio::io_context& io_context)
              :socket_(io_context), buf_out_(MAX_PACK_SIZE), buf_in_(MAX_PACK_SIZE), outs(&buf_out_), ins(&buf_in_) { }
    tcp::socket socket_;
    asio::streambuf buf_out_;
    std::ostream outs;
    asio::streambuf buf_in_;
    std::istream ins;
};

class ConnectedClient
    : public std::enable_shared_from_this<ConnectedClient> {
public:
    ConnectedClient(asio::io_context& io_context,
                    asio::io_context::strand& strand,
                    Redirector& red_zone)
                    :network_io_(io_context), strand_(strand), red_zone_(red_zone) { }
    tcp::socket& socket();
    void start();
    void write(Pack& pack);
    ClientID get_id();
private:
    void id_handler(const asio::error_code& error);
    void read_handler(const asio::error_code& error);
    void write_handler(const asio::error_code& error);
    ServerNetworkIO network_io_;
    ClientID client_id_;
    Pack read_pack_;
    std::deque<Pack> packs_to_write_;
    asio::io_context::strand& strand_;
    Redirector& red_zone_;
};

#endif