/**
 * @file server.cpp
 * @brief Implementation of Server classes.
 * @details
 * @version
 */

#include <server.hpp>

std::mutex WorkerThread::mtx;

// CustomerServer

void CustomerServer::run()
{
    //std::cout << "server run called" << std::endl;
    std::shared_ptr<ConnectedCustomerClient> new_client(new ConnectedCustomerClient(io_context_, strand_, red_zone_));
    prod_cons_thread_ = new std::thread(std::bind(CustomerServer::prod_cons_exec, this));
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

void CustomerServer::prod_cons_exec()
{
    {
        std::lock_guard<std::mutex> lock(mtx_);
        std::cout << "CustomerServer Producer Consumer Thread " << std::this_thread::get_id() << " starts." << std::endl;
    }
    //producer_.async_wait(lock_.wrap(std::bind(&CustomerServer::producer_handler, this, std::placeholders::_1)));
    consumer_.async_wait(lock_.wrap(std::bind(&CustomerServer::consumer_handler, this, std::placeholders::_1)));
    ioc_.run();
    {
        std::lock_guard<std::mutex> lock(mtx_);
        std::cout << "CustomerServer Producer Consumer Thread " << std::this_thread::get_id() << " ends." << std::endl;
    }
}

void CustomerServer::producer_handler(const asio::error_code& error)
{

}

void CustomerServer::consumer_handler(const asio::error_code& error)
{
    //std::cout << "customer server consumer_handler called" << std::endl;
    if(!(ktoc_queue_ptr_->foodid_queue.empty())) {
        {
            std::lock_guard<std::mutex> lock(ktoc_queue_ptr_->mtx);
            while (!(ktoc_queue_ptr_->foodid_queue.empty())) {
                {
                    std::lock_guard<std::mutex> lock_deals(deals_ptr_->mtx);
                    std::pair<unsigned int, FoodID> tmp = ktoc_queue_ptr_->foodid_queue.front();
                    auto it = deals_ptr_->deals.find(tmp.first);
                    if (it == deals_ptr_->deals.end()) cerr << "No such deal." << endl;
                    vector<pair<FoodID, unsigned short>> food_list = it->second.get_food_list();
                    vector<pair<bool,unsigned short>> fulfill = deals_ptr_->fulfill[tmp.first];
                    for (int i = 0; i < food_list.size(); ++i) {
                        if (food_list[i].first == tmp.second && fulfill[i].first == false) {
                            --deals_ptr_->fulfill[tmp.first][i].second;
                            if (deals_ptr_->fulfill[tmp.first][i].second == 0) {
                                deals_ptr_->fulfill[tmp.first][i].first = true;
                            }
                            break;
                        }
                    }
                    bool is_deal_fulfilled = true;
                    for (auto is_item_fulfilled : (deals_ptr_->fulfill[tmp.first])) {
                        is_deal_fulfilled &= is_item_fulfilled.first;
                    }
                    if (is_deal_fulfilled && !(deals_ptr_->fulfill[tmp.first]).empty()) {
                        std::string str = "Deal number : " + to_string(tmp.first) + " ,\nPlease retrieve your meal.";
                        red_zone_.write_to_client<std::string>(str);
                    }
                }
                ktoc_queue_ptr_->foodid_queue.pop();
            }
        }
    }
    consumer_.wait();
    consumer_.expires_from_now(ci_);
    consumer_.async_wait(lock_.wrap(std::bind(&CustomerServer::consumer_handler, this, std::placeholders::_1)));
    //std::cout << "customer server consumer_handler returned" << std::endl;
}

// CustomerServer

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
    //producer_.async_wait(lock_.wrap(std::bind(&KitchenServer::producer_handler, this, std::placeholders::_1)));
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
    consumer_.wait();
    consumer_.expires_from_now(ci_);
    consumer_.async_wait(lock_.wrap(std::bind(&KitchenServer::consumer_handler, this, std::placeholders::_1)));
    //std::cout << "kitchen server consumer_handler returned" << std::endl;
}