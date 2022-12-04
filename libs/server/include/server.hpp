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
#include <connected_client.hpp>
using asio::ip::tcp;
using Pack = std::array<char, MAX_PACK_SIZE>; // Pack is an array of char, for transmitting pack
using ClientID = std::array<char, MAX_ID_SIZE>; // ID is an array of char, for identifying the client

enum IDs {
    customer_id = 0,
    kitchen_id
};

class WorkerThread {
public:
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

class Server {
public:
    Server(asio::io_context& io_context,
           asio::io_context::strand& strand,
           const tcp::endpoint& endpoint)
        :io_context_(io_context), strand_(strand), acceptor_(io_context, endpoint) { run(); }
private:
    void run();
    void on_accept(std::shared_ptr<ConnectedClient> new_client, const asio::error_code& error);
    asio::io_context& io_context_;
    asio::io_context::strand& strand_;
    tcp::acceptor acceptor_;
    Redirector red_zone_;
};

#endif