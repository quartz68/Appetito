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
class CustomerRedirector;

/**
 * @brief The client connected to this server.
 */
class ConnectedClient
    : public std::enable_shared_from_this<ConnectedClient> {
public:
    /**
     * Construct a new Connected Client object from io context
     * @brief Constructor.
     * 
     * @param io_context 
     * @param strand 
     */
    ConnectedClient(asio::io_context& io_context,
                    asio::io_context::strand& strand);
    /**
     * @brief Get the socket.
     * 
     * @return TCP socket.
     */
    tcp::socket& socket();
    /**
     * @brief Write object to client.
     * 
     * @tparam Type of the object.
     * @param object The object to write.
     */
    template<typename T>
    void write(T& object)
    {
        network_io_.async_write( object,
                                strand_.wrap(std::bind(&ConnectedClient::write_completion_handler, shared_from_this(), std::placeholders::_1)));
    }
    /**
     * @brief Get the id of the connected client.
     * 
     * @return String of client ID.
     */
    std::string get_id();
protected:
    /**
     * @brief Handles completion of writing.
     */
    void write_completion_handler(const asio::error_code& error);
    NetworkIO network_io_;
    std::string client_id_;
    asio::io_context::strand& strand_;
    std::deque<std::string> strings_to_write_;
    std::string read_string_;
};

/**
 * @brief The customer client connected to this server.
 */
class ConnectedCustomerClient
    : public ConnectedClient {
public:
    /**
     * Construct a new Connected Customer Client object from io context and CustomerRedirector.
     * @brief Constructor.
     * 
     * @param io_context 
     * @param strand 
     * @param red_zone CustomerRedirector.
     */
    ConnectedCustomerClient(asio::io_context& io_context,
                    asio::io_context::strand& strand,
                    CustomerRedirector& red_zone);
    /**
     * @brief Starts the connection, gets the customer client ID.
     */
    void start();
    /**
     * @brief shared_from_this() for subclass
     * 
     * @return Casted shared_ptr of ConnectedCustomerClient from ConnectedClient::shared_from_this().
     */
    std::shared_ptr<ConnectedCustomerClient> shared_from_this() { return std::static_pointer_cast<ConnectedCustomerClient>(ConnectedClient::shared_from_this()); }
protected:
    /**
     * @brief Handles customer client ID.
     */
    void id_handler(const asio::error_code& error);
    /**
     * @brief Handles reading a string.
     */
    void read_handler(const asio::error_code& error);
    /**
     * @brief Handles reading a Deal.
     */
    void read_deal_handler(const asio::error_code& error);
    Order read_order_;
    Table read_table_;
    Deal read_deal_;
    CustomerRedirector& red_zone_;
};

#endif