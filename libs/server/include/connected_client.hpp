#ifndef CONNECTED_CLIENT_HPP_
#define CONNECTED_CLIENT_HPP_

/**
 * @file connectedclient.hpp
 * @brief Header of ConnectedClient classes.
 * @details
 * @version
 */

#include <iostream>
#include <chrono>
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
class KitchenRedirector;

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
     * @brief Handles reading customer client ID, writing menu to client.
     */
    void id_handler(const asio::error_code& error);
    /**
     * @brief Handles reading a Deal, saving Deal to vector deals_, writing deal number to client.
     */
    void read_deal_handler(const asio::error_code& error);
    /**
     * @brief Handles .
     */
    void queueing_handler(const asio::error_code& error);
    Order read_order_;
    Table read_table_;
    Deal read_deal_;
    CustomerRedirector& red_zone_;
};

/**
 * @brief The kitchen client connected to this server.
 */
class ConnectedKitchenClient
    : public ConnectedClient {
public:
    /**
     * Construct a new Connected Kitchen Client object from io context and KitchenRedirector.
     * @brief Constructor.
     * 
     * @param io_context 
     * @param strand 
     * @param red_zone KitchenRedirector.
     */
    ConnectedKitchenClient(asio::io_context& io_context,
                    asio::io_context::strand& strand,
                    KitchenRedirector& red_zone);
    /**
     * @brief Starts the connection, gets the kitchen client ID.
     */
    void start();
    /**
     * @brief shared_from_this() for subclass
     * 
     * @return Casted shared_ptr of ConnectedKitchenClient from ConnectedClient::shared_from_this().
     */
    std::shared_ptr<ConnectedKitchenClient> shared_from_this() { return std::static_pointer_cast<ConnectedKitchenClient>(ConnectedClient::shared_from_this()); }
protected:
    /**
     * @brief Handles kitchen client ID.
     */
    void id_handler(const asio::error_code& error);
    /**
     * @brief Handles reading a finished item.
     */
    void read_item_handler(const asio::error_code& error);

    pair<unsigned int, FoodID> read_item_;
    KitchenRedirector& red_zone_;
};

#endif