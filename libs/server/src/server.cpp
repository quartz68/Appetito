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
    prod_cons_thread_ = new std::thread(std::bind(KitchenServer::prod_cons_exec, this));
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

void KitchenServer::prod_cons_exec()
{
    {
        std::lock_guard<std::mutex> lock(mtx_);
        std::cout << "KitchenServer Producer Consumer Thread " << std::this_thread::get_id() << " starts." << std::endl;
    }
    producer_.async_wait(lock_.wrap(std::bind(&KitchenServer::producer_handler, this, std::placeholders::_1)));
    consumer_.async_wait(lock_.wrap(std::bind(&KitchenServer::consumer_handler, this, std::placeholders::_1)));
    ioc_.run();
    {
        std::lock_guard<std::mutex> lock(mtx_);
        std::cout << "KitchenServer Producer Consumer Thread " << std::this_thread::get_id() << " ends." << std::endl;
    }
}

void KitchenServer::producer_handler(const asio::error_code& error)
{

}

void KitchenServer::consumer_handler(const asio::error_code& error)
{
    //std::cout << "kitchen server consumer_handler called" << std::endl;
    if(!(ctok_queue_ptr_->foodid_queue.empty())) {
        {
            std::lock_guard<std::mutex> lock(ctok_queue_ptr_->mtx);
            while (!(ctok_queue_ptr_->foodid_queue.empty())) {
                std::pair<unsigned int, FoodID> tmp = ctok_queue_ptr_->foodid_queue.front();
                ctok_queue_ptr_->foodid_queue.pop();
                red_zone_.write_to_client<std::pair<unsigned int, FoodID>>(tmp);
            }
        }
    }
    consumer_.expires_from_now(ci_);
    consumer_.async_wait(lock_.wrap(std::bind(&KitchenServer::consumer_handler, this, std::placeholders::_1)));
    //std::cout << "kitchen server consumer_handler returned" << std::endl;
}