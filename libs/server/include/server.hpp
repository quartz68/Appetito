#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <iostream>
#include <queue>
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
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Thread " << std::this_thread::get_id() << " starts." << std::endl;
        }

        io_context->run();

        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Thread " << std::this_thread::get_id() << " ends." << std::endl;
        }
    }
private:
    static std::mutex mtx;
};

struct CustomerToKitchenQueue {
    std::queue<std::pair<unsigned int, FoodID>> foodid_queue;
    std::mutex mtx;
};

struct KitchenToCustomerQueue {
    std::queue<std::pair<unsigned int, FoodID>> foodid_queue;
    std::mutex myx;
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
     * @param deal_counter Raw pointer to deal counter.
     * @param deals Raw pointer to map of deals.
     */
    Server(asio::io_context& io_context,
           asio::io_context::strand& strand,
           const tcp::endpoint& endpoint,
           FoodContainer* all_foods,
           TableContainer* all_tables,
           unsigned int* deal_counter,
           map<unsigned int, Deal>* deals)
        :io_context_(io_context), strand_(strand), acceptor_(io_context, endpoint), all_foods_ptr_(all_foods), all_tables_ptr_(all_tables), deal_counter_ptr_(deal_counter), deals_ptr_(deals)
        { 
            if (all_foods == nullptr || all_tables == nullptr) {
                cerr << "Trying to set nullptr as FoodContainer or TableContainer pointer!" << endl;
            }
            if (deal_counter == nullptr) {
                cerr << "Trying to set nullptr as deal counter pointer!" << endl;
            }
            if (deals == nullptr) {
                cerr << "Trying to set nullptr as deals map pointer!" << endl;
            }
        }
protected:
    asio::io_context& io_context_;
    asio::io_context::strand& strand_;
    tcp::acceptor acceptor_;
    FoodContainer* all_foods_ptr_;
    TableContainer* all_tables_ptr_;
    unsigned int* deal_counter_ptr_;
    map<unsigned int, Deal>* deals_ptr_;
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
     * @param deal_counter Raw pointer to deal counter.
     * @param ctok_queue Raw pointer to customer to kitchen queue.
     * @param ktoc_queue Raw pointer to kitchen to customer queue.
     * @param deals Raw pointer to map of deals.
     */
    CustomerServer(asio::io_context& io_context,
        asio::io_context::strand& strand,
        const tcp::endpoint& endpoint,
        FoodContainer* all_foods,
        TableContainer* all_tables,
        unsigned int* deal_counter,
        CustomerToKitchenQueue* ctok_queue,
        KitchenToCustomerQueue* ktoc_queue,
        map<unsigned int, Deal>* deals)
        :Server{io_context, strand, endpoint, all_foods, all_tables, deal_counter, deals}, red_zone_{all_foods,all_tables,deal_counter,ctok_queue,ktoc_queue,deals}, ctok_queue_ptr_(ctok_queue), ktoc_queue_ptr_(ktoc_queue) 
        {
            if (ctok_queue == nullptr || ktoc_queue == nullptr) {
                cerr << "Trying to set nullptr as CustomerToKitchenQueue or KitchenToCustomerQueue pointer!" << endl;
            }
            run();
        }
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
    CustomerToKitchenQueue* ctok_queue_ptr_;
    KitchenToCustomerQueue* ktoc_queue_ptr_;
};

class KitchenServer
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
     * @param deal_counter Raw pointer to deal counter.
     * @param ktoc_queue Raw pointer to kitchen to customer queue.
     * @param ctok_queue Raw pointer to customer to kitchen queue.
     * @param deals Raw pointer to map of deals.
     */
    KitchenServer(asio::io_context& io_context,
        asio::io_context::strand& strand,
        const tcp::endpoint& endpoint,
        FoodContainer* all_foods,
        TableContainer* all_tables,
        unsigned int* deal_counter,
        KitchenToCustomerQueue* ktoc_queue,
        CustomerToKitchenQueue* ctok_queue,
        map<unsigned int, Deal>* deals)
        :Server{io_context, strand, endpoint, all_foods, all_tables, deal_counter, deals}, red_zone_{all_foods,all_tables,deal_counter,ktoc_queue,ctok_queue,deals}, ctok_queue_ptr_(ctok_queue), ktoc_queue_ptr_(ktoc_queue) 
        {
            if (ctok_queue == nullptr || ktoc_queue == nullptr) {
                cerr << "Trying to set nullptr as CustomerToKitchenQueue or KitchenToCustomerQueue pointer!" << endl;
            }
            run();
        }
protected:
    /**
     * @brief Run the kitchen server.
     */
    void run();
    /**
     * @brief Handles accepting the connection of a new kitchen client.
     * 
     * @param new_client New kitchen client.
     */
    void on_accept(std::shared_ptr<ConnectedKitchenClient> new_client, const asio::error_code& error);
    KitchenRedirector red_zone_;
    KitchenToCustomerQueue* ktoc_queue_ptr_;
    CustomerToKitchenQueue* ctok_queue_ptr_;
};

#endif