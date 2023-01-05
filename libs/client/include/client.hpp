/**
 * @file client.hpp
 * @brief Header of Client classes.
 * @details
 * @version
 */

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
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

struct FoodIDVector{
    std::vector<std::pair<unsigned int, FoodID>> vector;
    std::mutex mtx;
};

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
            
        }
    Client(const Client& other)
        :io_context_(other.io_context_), network_io_(other.io_context_), client_id_(other.client_id_), menu_(other.menu_) { }
    /**
     * @brief Get menu.
     * 
     * @return Reference to menu.
     */
    Menu& menu() { return menu_; }
    /**
     * @brief Write an object to the server.
     * 
     * @tparam T The type of the object.
     * @param object The object to write.
     */
    template<typename T>
    void write(T& object)
    {
        //std::cout << "client write called" << std::endl;
        network_io_.async_write( object,
                                std::bind(&Client::write_completion_handler, this, std::placeholders::_1));
        //std::cout << "client write returned" << std::endl;
    }
    /**
     * @brief Close the client.
     */
    void close();
protected:
    /**
     * @brief Handles completion of write.
     */
    void write_completion_handler(const asio::error_code& error);
    /**
     * @brief Implementation of closing the client.
     */
    void close_implementation();
    asio::io_context& io_context_; /**< The io_context. */
    std::string client_id_; /**< The client id. */
    Menu menu_; /**< The read menu. */
    NetworkIO network_io_; /**< The NetworkIO module. */
};

class CustomerClient
    : public Client {
public:
    /**
     * Construct a CustomerClient object from client ID, io_context and TCP endpoint.
     * @brief Constructor.
     * 
     * @param client_id The client ID which user designated.
     * @param io_context Asio I/O context(service).
     * @param endpoint_iterator Iterator of TCP endpoint.
     */
    CustomerClient(const std::string& client_id,
            asio::io_context& io_context,
            tcp::resolver::iterator endpoint_iterator)
        :Client{client_id, io_context, endpoint_iterator} 
        {
            asio::async_connect(network_io_.socket(), endpoint_iterator, std::bind(&CustomerClient::on_connect, this, std::placeholders::_1)); // Connect to server
        }
    /**
     * @brief Copy constructor.
     */
    CustomerClient(const CustomerClient& other)
        :Client{other} { }
    /**
     * @brief Save the deal to past_deals_.
     * 
     * @param deal The deal to save.
     */
    void save_deal(Deal deal) { past_deals_.push_back(deal); }
    /**
     * @brief Print the current (latest) deal.
     */
    void print_current_deal();
    /**
     * @brief Print all past deals made.
     */
    void print_past_deals();
protected:
    /**
     * @brief Writes client ID to server.
     */
    void on_connect(const asio::error_code& error);
    /**
     * @brief Reads menu from server.
     */
    void connect_handler(const asio::error_code& error);
    /**
     * @brief Reads deal number and displays it.
     */
    void deal_number_handler(const asio::error_code& error);
    /**
     * @brief Reads notification and displays it.
     */
    void notification_handler(const asio::error_code& error);
    vector<Deal> past_deals_; /**< The deals made. */
    unsigned int deal_number_; /**< The deal number. */
    std::string notification_; /**< The notification. */
};

class KitchenClient
    : public Client {
public:
    /**
     * Construct a KitchenClient object from client ID, io_context, TCP endpoint and vector of items.
     * @brief Constructor.
     * 
     * @param client_id The client ID which user designated.
     * @param io_context Asio I/O context(service).
     * @param endpoint_iterator Iterator of TCP endpoint.
     * @param foodid_vector Vector of items.
     */
    KitchenClient(const std::string& client_id,
            asio::io_context& io_context,
            tcp::resolver::iterator endpoint_iterator,
            FoodIDVector* foodid_vector)
        :Client{client_id, io_context, endpoint_iterator}, foodid_vector_(foodid_vector)
        {
            asio::async_connect(network_io_.socket(), endpoint_iterator, std::bind(&KitchenClient::on_connect, this, std::placeholders::_1)); // Connect to server
        }
protected:
    /**
     * @brief Writes client ID to server.
     */
    void on_connect(const asio::error_code& error);
    /**
     * @brief Reads menu from server.
     */
    void connect_handler(const asio::error_code& error);
    /**
     * @brief Prints menu.
     */
    void connect_handler_step2(const asio::error_code& error);
    /**
     * @brief Reads item and displays it.
     */
    void read_handler(const asio::error_code& error);
    std::pair<unsigned int, FoodID> read_foodid_; /**< Read item. */
    FoodIDVector* foodid_vector_; /**< Vector of items. */
};

#endif