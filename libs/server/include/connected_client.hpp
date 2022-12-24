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
#include <customer_menu.hpp>
using asio::ip::tcp;

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
    template<typename T>
    void write(T& object)
    {
        network_io_.async_write( object,
                                strand_.wrap(std::bind(&ConnectedClient::write_handler, shared_from_this(), std::placeholders::_1)));
    }
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