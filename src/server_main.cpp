#include <iostream>
#include <fstream>
#include <deque>
#include <list>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <asio.hpp>
#include <protocol.hpp>
#include <server.hpp>
using asio::ip::tcp;


int main(int argc, char* argv[])
{
    try {
        std::shared_ptr<asio::io_context> io_context(new asio::io_context);
        std::shared_ptr<asio::io_context::work> work(new asio::io_context::work(*io_context));
        std::shared_ptr<asio::io_context::strand> strand(new asio::io_context::strand(*io_context));

        // FoodContainer and TableContainer
        ifstream food_file;
        food_file.open("food_file.csv");
        if(!food_file) return -1;
        ifstream table_file;
        table_file.open("table_file.csv");
        if(!table_file) return -1;
        FoodContainer all_foods(food_file);
        TableContainer all_tables(table_file);
        std::cout << "Server " << std::this_thread::get_id() << " starts." << std::endl;

        // Deal counter and queues
        unsigned int deal_counter = 1;
        map<unsigned int, Deal> deals;
        CustomerToKitchenQueue ctok_queue;
        KitchenToCustomerQueue ktoc_queue;

        // Servers
        std::list<std::shared_ptr<CustomerServer>> customer_servers;
        std::list<std::shared_ptr<KitchenServer>> kitchen_servers;

        // Start customer server
        tcp::endpoint endpoint_customer(tcp::v4(), std::atoi("9000"));
        std::shared_ptr<CustomerServer> acserver(new CustomerServer(*io_context, *strand, endpoint_customer, &all_foods, &all_tables, &deal_counter, &ctok_queue, &ktoc_queue, &deals));
        customer_servers.push_back(acserver);

        // Start kitchen server
        tcp::endpoint endpoint_kitchen(tcp::v4(), std::atoi("9001"));
        std::shared_ptr<KitchenServer> akserver(new KitchenServer(*io_context, *strand, endpoint_kitchen, &all_foods, &all_tables, &deal_counter, &ktoc_queue, &ctok_queue, &deals));
        kitchen_servers.push_back(akserver);

        // Worker threads
        std::vector<std::thread*> workers; // Thread group
        for (int i = 0; i < 2; ++i) {
            std::thread* t = new std::thread{ std::bind(WorkerThread::run_ioc, io_context) };
            workers.push_back(t);
        }

        // Join threads
        for (auto thread : workers) {
            thread->join();
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}