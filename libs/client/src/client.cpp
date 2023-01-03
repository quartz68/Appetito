#include <client.hpp>

// Client

void Client::close() 
{
    //std::cout << "client close called" << std::endl;
    io_context_.post(std::bind(&Client::close_implementation, this));
    //std::cout << "client close returned" << std::endl;
}

void Client::write_completion_handler(const asio::error_code& error)
{
    //std::cout << "client write handler called" << std::endl;
    if (!error) {
        std::cout << "Write completed!" << std::endl;
    } else {
            std::cout << error.message() << std::endl;
            close_implementation();
    }
    //std::cout << "client write handler returned" << std::endl;
}

void Client::close_implementation() 
{
    //std::cout << "client close implementation called" << std::endl;
    network_io_.socket().close();
    //std::cout << "client close implementation returned" << std::endl;
}

// CustomerClient

void CustomerClient::on_connect(const asio::error_code& error)
{
    //std::cout << "client on connect called" << std::endl;
    if (!error) {
        network_io_.async_write(client_id_,
                          std::bind(&CustomerClient::connect_handler, this, std::placeholders::_1));
    } else {
        std::cout << error.message() << std::endl;
        close_implementation();
    }
    //std::cout << "client on connect returned" << std::endl;
}

void CustomerClient::connect_handler(const asio::error_code& error)
{
    if (!error) {
        network_io_.async_read(menu_,
                         std::bind(&CustomerClient::read_handler, this, std::placeholders::_1));
    } else {
        std::cout << error.message() << std::endl;
        close_implementation();
    }
}

void CustomerClient::connect_handler_step2(const asio::error_code& error)
{
    if (!error) {
        menu_.print();
        /* Order order(menu_);
        Table table;
        Deal deal(order,table);
        network_io_.async_write(deal,
                         std::bind(&Client::read_handler, this, std::placeholders::_1)); */
        
    } else {
        std::cout << error.message() << std::endl;
        close_implementation();
    }
}

void CustomerClient::read_handler(const asio::error_code& error)
{
    //std::cout << "client read handler called" << std::endl;
    if (!error) {
        std::cout << read_string_ << std::endl;
        network_io_.async_read(read_string_,
                         std::bind(&CustomerClient::read_handler, this, std::placeholders::_1));
    } else {
        std::cout << error.message() << std::endl;
        close_implementation();
    }
    //std::cout << "client read handler returned" << std::endl;
}

// KitchenClient

void KitchenClient::on_connect(const asio::error_code& error)
{
    if (!error) {
        network_io_.async_write(client_id_,
                          std::bind(&KitchenClient::connect_handler, this, std::placeholders::_1));
    } else {
        std::cout << error.message() << std::endl;
        close_implementation();
    }
}

void KitchenClient::connect_handler(const asio::error_code& error)
{
    if (!error) {
        network_io_.async_read(menu_,
                         std::bind(&KitchenClient::connect_handler_step2, this, std::placeholders::_1));
    } else {
        std::cout << error.message() << std::endl;
        close_implementation();
    }
}

void KitchenClient::connect_handler_step2(const asio::error_code& error)
{
    if (!error) {
        menu_.print();
        network_io_.async_read(read_foodid_,
                         std::bind(&KitchenClient::read_handler, this, std::placeholders::_1));
    } else {
        std::cout << error.message() << std::endl;
        close_implementation();
    }
}

void KitchenClient::read_handler(const asio::error_code& error)
{
    if (!error) {
        {
            std::lock_guard<std::mutex> lock(foodid_queue_->mtx);
            foodid_queue_->queue.push(read_foodid_);
        }
        std::cout << menu_.foods_[read_foodid_.second].get_name() << std::endl;
        network_io_.async_read(read_foodid_,
                         std::bind(&KitchenClient::read_handler, this, std::placeholders::_1));
    } else {
        std::cout << error.message() << std::endl;
        close_implementation();
    }
}