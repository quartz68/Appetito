#ifndef CONNECTED_CLIENT_HPP_
#define CONNECTED_CLIENT_HPP_

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

class Redirector;

class ConnectedClient
    : public std::enable_shared_from_this<ConnectedClient> {
public:
    ConnectedClient(asio::io_context& io_context,
                    asio::io_context::strand& strand,
                    Redirector& red_zone)
                    :network_io_(io_context), strand_(strand), red_zone_(red_zone) { }

    tcp::socket& socket();
    void start();
    void write(std::string& object);
    std::string get_id();
private:
    void id_handler(const asio::error_code& error);
    void read_handler(const asio::error_code& error);
    void write_handler(const asio::error_code& error);
    NetworkIO network_io_;
    std::string client_id_;
    asio::io_context::strand& strand_;
    std::deque<std::string> strings_to_write_;
    std::string read_string_;
    Redirector& red_zone_;
};

#endif