#include <iostream>
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
using Pack = std::array<char, MAX_PACK_SIZE>; // Pack is an array of char, for transmitting pack
using ClientID = std::array<char, MAX_ID_SIZE>; // ID is an array of char, for identifying the client


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

        std::cout << "Server " << std::this_thread::get_id() << " starts." << std::endl;
        
        std::list<std::shared_ptr<Server>> servers;
        for (int i = 1; i < argc; ++i)
        {
            tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
            std::shared_ptr<Server> aserver(new Server(*io_context, *strand, endpoint));
            servers.push_back(aserver);
        }

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