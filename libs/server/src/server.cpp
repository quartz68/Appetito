#include <server.hpp>

std::mutex WorkerThread::m;

void Server::run()
{
    //std::cout << "server run called" << std::endl;
    std::shared_ptr<ConnectedClient> new_client(new ConnectedClient(io_context_, strand_, red_zone_));
    acceptor_.async_accept(new_client->socket(), strand_.wrap(std::bind(&Server::on_accept, this, new_client, std::placeholders::_1)));
    //std::cout << "server run returned" << std::endl;
}

void Server::on_accept(std::shared_ptr<ConnectedClient> new_client, const asio::error_code& error)
{
    //std::cout << "server on accept called" << std::endl;
    if (!error) {
        new_client->start();
    } else {
        std::cout << error.message() << std::endl;
    }
    run();
    //std::cout << "server on accept returned" << std::endl;
}