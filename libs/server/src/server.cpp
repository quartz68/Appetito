#include <server.hpp>

std::mutex WorkerThread::mtx;

void CustomerServer::run()
{
    //std::cout << "server run called" << std::endl;
    std::shared_ptr<ConnectedCustomerClient> new_client(new ConnectedCustomerClient(io_context_, strand_, red_zone_));
    acceptor_.async_accept(new_client->socket(), strand_.wrap(std::bind(&CustomerServer::on_accept, this, new_client, std::placeholders::_1)));
    //std::cout << "server run returned" << std::endl;
}

void CustomerServer::on_accept(std::shared_ptr<ConnectedCustomerClient> new_client, const asio::error_code& error)
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

void KitchenServer::run()
{
    //std::cout << "server run called" << std::endl;
    std::shared_ptr<ConnectedKitchenClient> new_client(new ConnectedKitchenClient(io_context_, strand_, red_zone_));
    acceptor_.async_accept(new_client->socket(), strand_.wrap(std::bind(&KitchenServer::on_accept, this, new_client, std::placeholders::_1)));
    //std::cout << "server run returned" << std::endl;
}

void KitchenServer::on_accept(std::shared_ptr<ConnectedKitchenClient> new_client, const asio::error_code& error)
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