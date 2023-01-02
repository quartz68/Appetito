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
#include <menu.hpp>
#include <food.hpp>
#include <table.hpp>
#include <order_deal.hpp>
using asio::ip::tcp;

/**
 * @brief Network client.
 */
class Client {
public:
    /**
     * Construct a Client object from client ID, io_context and TCP endpoint.
     * @brief Constructor.
     * 
     * @param client_id The client ID which user designated.
     * @param io_context Asio I/O context(service).
     * @param endpoint_iterator Iterator of TCP endpoint.
     */
    Client(const std::string& client_id,
            asio::io_context& io_context,
            tcp::resolver::iterator endpoint_iterator)
        :io_context_(io_context), network_io_(io_context), client_id_(client_id)
        {
            asio::async_connect(network_io_.socket(), endpoint_iterator, std::bind(&Client::on_connect, this, std::placeholders::_1)); // Connect to server
        }
    Client(const Client& other)
        :io_context_(other.io_context_), network_io_(other.io_context_), client_id_(other.client_id_), menu_(other.menu_) { }
    Menu& menu() { return menu_; }
    /**
     * @brief 
     * 
     * @param object 
     */
    template<typename T>
    void write(T& object)
    {
        std::cout << "client write called" << std::endl;
        network_io_.async_write( object,
                                std::bind(&Client::write_completion_handler, this, std::placeholders::_1));
        std::cout << "client write returned" << std::endl;
    }
    void close();
private:
    void on_connect(const asio::error_code& error);
    void connect_handler(const asio::error_code& error);
    void connect_handler_step2(const asio::error_code& error);
    void read_handler(const asio::error_code& error);
    void write_completion_handler(const asio::error_code& error);
    void close_implementation();
    asio::io_context& io_context_;
    std::string client_id_;
    std::string read_string_;
    Menu menu_;
    NetworkIO network_io_;
};

#endif