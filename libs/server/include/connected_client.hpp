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
#include <menu.hpp>
#include <order_deal.hpp>
using asio::ip::tcp;

class Redirector;

class ConnectedClient
    : public std::enable_shared_from_this<ConnectedClient> {
public:
    ConnectedClient(asio::io_context& io_context,
                    asio::io_context::strand& strand,
                    Redirector& red_zone);
    tcp::socket& socket();
    void start();
    template<typename T>
    void write(T& object)
    {
        network_io_.async_write( object,
                                strand_.wrap(std::bind(&ConnectedClient::write_completion_handler, shared_from_this(), std::placeholders::_1)));
    }
    std::string get_id();
private:
    void id_handler(const asio::error_code& error);
    void read_handler(const asio::error_code& error);
    //void read_deal_handler(const asio::error_code& error);
    void read_deal_handler(const asio::error_code& error);
    void write_completion_handler(const asio::error_code& error);
    NetworkIO network_io_;
    std::string client_id_;
    asio::io_context::strand& strand_;
    std::deque<std::string> strings_to_write_;
    std::string read_string_;
    Order read_order_;
    Table read_table_;
    Deal read_deal_;
    Redirector& red_zone_;
};

#endif