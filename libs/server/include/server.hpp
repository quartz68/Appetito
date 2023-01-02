#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <iostream>
#include <deque>
#include <thread>
#include <mutex>
#include <unordered_set>
#include <unordered_map>
#include <asio.hpp>
#include <protocol.hpp>
#include <redirector.hpp>
#include <food.hpp>
#include <table.hpp>
#include <connected_client.hpp>
using asio::ip::tcp;

/**
 * @brief Worker thread with mutex.
 */
class WorkerThread {
public:
    /**
     * @brief Run the io_context in a thread and use lock_guard to achieve RAII for mutex.
     * 
     * @param io_context 
     */
    static void run(std::shared_ptr<asio::io_context> io_context)
    {
        {
            std::lock_guard<std::mutex> lock(m);
            std::cout << "Thread " << std::this_thread::get_id() << " starts." << std::endl;
        }

        io_context->run();

        {
            std::lock_guard<std::mutex> lock(m);
            std::cout << "Thread " << std::this_thread::get_id() << " ends." << std::endl;
        }
    }
private:
    static std::mutex m;
};

/**
 * @brief Server.
 */
class Server {
public:
    /**
     * @brief Constructor.
     * 
     * @param io_context 
     * @param strand 
     * @param endpoint 
     * @param all_foods Raw pointer to FoodContainer.
     * @param all_tables Raw pointer to TableContainer.
     */
    Server(asio::io_context& io_context,
           asio::io_context::strand& strand,
           const tcp::endpoint& endpoint,
           FoodContainer* all_foods,
           TableContainer* all_tables)
        :io_context_(io_context), strand_(strand), acceptor_(io_context, endpoint), all_foods_ptr_(all_foods), all_tables_ptr_(all_tables) { if (all_foods == nullptr || all_tables == nullptr) cerr << "Trying to set nullptr as FoodContainer or TableContainer pointer!" << endl; }
protected:
    asio::io_context& io_context_;
    asio::io_context::strand& strand_;
    tcp::acceptor acceptor_;
    FoodContainer* all_foods_ptr_;
    TableContainer* all_tables_ptr_;
};

class CustomerServer
    : public Server {
public:
    /**
     * @brief Constructor.
     * 
     * @param io_context 
     * @param strand 
     * @param endpoint 
     * @param all_foods Raw pointer to FoodContainer.
     * @param all_tables Raw pointer to TableContainer.
     */
    CustomerServer(asio::io_context& io_context,
        asio::io_context::strand& strand,
        const tcp::endpoint& endpoint,
        FoodContainer* all_foods,
        TableContainer* all_tables)
    :Server{io_context, strand, endpoint, all_foods, all_tables}, red_zone_{all_foods,all_tables} { run(); }
protected:
    /**
     * @brief Run the customer server.
     */
    void run();
    /**
     * @brief Handles accepting the connection of a new customer client.
     * 
     * @param new_client New customer client.
     */
    void on_accept(std::shared_ptr<ConnectedCustomerClient> new_client, const asio::error_code& error);
    CustomerRedirector red_zone_;
};

#endif