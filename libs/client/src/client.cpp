/**
 * @file client.cpp
 * @brief Implementation of Client classes.
 * @details
 * @version
 */

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
        //std::cout << "Write completed!" << std::endl;
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
        if (menu_.foods_.empty()) {
            network_io_.async_read(menu_,
                         std::bind(&CustomerClient::connect_handler, this, std::placeholders::_1));
        } else {
            network_io_.async_read(deal_number_,
                         std::bind(&CustomerClient::deal_number_handler, this, std::placeholders::_1));
        }
    } else {
        std::cout << error.message() << std::endl;
        close_implementation();
    }
}

void CustomerClient::deal_number_handler(const asio::error_code& error)
{
    if (!error) {
        system("cls");
        std::cout << DEAL_NUMBER_HEADER << std::endl;
        std::cout << "Your deal number is: " << deal_number_ << std::endl;
        std::cout << DIVIDE << std::endl;
        std::cout << "Please stay tuned for notification, or\nwait for the waiter to call your number." << std::endl;
        std::cout << BOTTOM << std::endl;
        network_io_.async_read(notification_,
                         std::bind(&CustomerClient::notification_handler, this, std::placeholders::_1));
    } else {
        std::cout << error.message() << std::endl;
        close_implementation();
    }
}

void CustomerClient::notification_handler(const asio::error_code& error)
{
    if (!error) {
        if (!notification_.empty()) {
            std::cout << NOTIFICATION_HEADER << std::endl;
            std::cout << notification_ << std::endl;
            std::cout << BOTTOM << std::endl;
        }
        network_io_.async_read(notification_,
                         std::bind(&CustomerClient::notification_handler, this, std::placeholders::_1));
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

void CustomerClient::print_current_deal() {
    if (past_deals_.empty()) {
        std::cout << "\nThere hasn't been any deals yet.\n" << std::endl;
    } else {
        system("cls");
        auto current_deal = past_deals_.back();
        std::cout << CURRENT_DEAL_HEADER << std::endl;
        current_deal.print();
        std::cout << BOTTOM << std::endl;
        std::cout << DEAL_NUMBER_HEADER << std::endl;
        std::cout << "Your deal number is: " << deal_number_ << std::endl;
        std::cout << DIVIDE << std::endl;
        std::cout << "Please stay tuned for notification, or\nwait for the waiter to call your number." << std::endl;
        std::cout << BOTTOM << std::endl;
    }
    
}

void CustomerClient::print_past_deals() {
    if (past_deals_.empty()) {
        std::cout << "\nThere hasn't been any deals yet.\n" << std::endl;
    } else {
        system("cls");
        for (auto deal : past_deals_){
            std::cout << DEAL_HEADER << std::endl;
            deal.print();
            std::cout << BOTTOM << std::endl;
        }
    }
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
        menu_.print_foods();
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
            std::lock_guard<std::mutex> lock(foodid_vector_->mtx);
            foodid_vector_->vector.push_back(read_foodid_);
        }
        std::cout << NEW_ITEM_HEADER << std::endl;
        std::string id = to_string(read_foodid_.first) + '-' + to_string(read_foodid_.second.type) +  '-' + to_string(read_foodid_.second.id);
        std::cout << small_field << id << large_field << menu_.foods_[read_foodid_.second].get_name() << std::endl;
        std::cout << BOTTOM << std::endl;
        network_io_.async_read(read_foodid_,
                         std::bind(&KitchenClient::read_handler, this, std::placeholders::_1));
    } else {
        std::cout << error.message() << std::endl;
        close_implementation();
    }
}