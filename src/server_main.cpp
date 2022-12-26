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
        if (argc < 2) {
            std::cerr << "Usage: server <port> <port> ..." << std::endl;
            return 1;
        }

        std::shared_ptr<asio::io_context> io_context(new asio::io_context);
        std::shared_ptr<asio::io_context::work> work(new asio::io_context::work(*io_context));
        std::shared_ptr<asio::io_context::strand> strand(new asio::io_context::strand(*io_context));

        ifstream food_file;
        food_file.open("food_file.csv");
        if(!food_file) return -1;
        ifstream table_file;
        table_file.open("table_file.csv");
        if(!table_file) return -1;
        FoodContainer all_foods(food_file);
        TableContainer all_tables(table_file);
        std::cout << "Server " << std::this_thread::get_id() << " starts." << std::endl;
        
        std::list<std::shared_ptr<Server>> servers;
        for (int i = 1; i < argc; ++i)
        {
            tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
            std::shared_ptr<Server> aserver(new Server(*io_context, *strand, endpoint, &all_foods, &all_tables));
            servers.push_back(aserver);
        }
        /* tcp::endpoint endpoint(tcp::v4(), std::atoi("9000"));
        std::shared_ptr<Server> aserver(new Server(*io_context, *strand, endpoint, &all_foods, &all_tables));
        servers.push_back(aserver); */

        std::vector<std::thread*> workers; // Thread group
        for (int i = 0; i < argc - 1; ++i) {
            std::thread* t = new std::thread{ std::bind(WorkerThread::run, io_context) };
            workers.push_back(t);
        }

        for (auto i : workers) {
            i->join();
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}