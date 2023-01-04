/**
 * @file connectedclient.cpp
 * @brief Implementation of ConnectedClient classes.
 * @details
 * @version
 */

#include <connected_client.hpp>
#include <redirector.hpp>
#include <server.hpp>

// ConnectedClient

ConnectedClient::ConnectedClient(asio::io_context& io_context,
                    asio::io_context::strand& strand)
                    :network_io_(io_context), strand_(strand) { }

tcp::socket& ConnectedClient::socket() 
{
    return network_io_.socket();
}

std::string ConnectedClient::get_id()
{
    //std::cout << "connected client get id called" << std::endl;
    return client_id_;
}

void ConnectedClient::write_completion_handler(const asio::error_code& error)
{
    //std::cout << "connected client write handler called" << std::endl;
    if (!error) {
        std::cout << "Write completed!" << std::endl;
    } else {
        std::cout << error.message() << std::endl;
    }
    //std::cout << "connected client write handler returned" << std::endl;
}

// ConnectedCustomerClient

ConnectedCustomerClient::ConnectedCustomerClient(asio::io_context& io_context,
                    asio::io_context::strand& strand,
                    CustomerRedirector& red_zone)
                    :ConnectedClient{io_context, strand}, red_zone_(red_zone), read_order_(*(red_zone_.menu_ptr())), read_deal_(read_order_,read_table_) { }

void ConnectedCustomerClient::start()
{
    //std::cout << "connected client start called" << std::endl;
    network_io_.async_read(client_id_,
                     strand_.wrap(std::bind(&ConnectedCustomerClient::id_handler, shared_from_this(), std::placeholders::_1)));
    //std::cout << "connected client start returned" << std::endl;
}

void ConnectedCustomerClient::id_handler(const asio::error_code& error)
{
    //std::cout << "connected client id handler called" << std::endl;
    red_zone_.enter(shared_from_this(), client_id_);
    write(*(red_zone_.menu_ptr()));
    std::cout << "Customer " << client_id_ << " entered!" << std::endl;
    network_io_.async_read(read_deal_,
                    strand_.wrap(std::bind(&ConnectedCustomerClient::read_deal_handler, shared_from_this(), std::placeholders::_1)));
    //std::cout << "connected client id handler returned" << std::endl;
}

void ConnectedCustomerClient::read_handler(const asio::error_code& error)
{
    //std::cout << "connected client read handler called" << std::endl;
    if (!error) {
        std::cout << read_string_ << std::endl;
        network_io_.async_read(read_string_,
                         strand_.wrap(std::bind(&ConnectedCustomerClient::read_handler, shared_from_this(), std::placeholders::_1)));
    } else {
        std::cout << error.message() << std::endl;
        red_zone_.leave(shared_from_this());
    }
    //std::cout << "connected client read handler returned" << std::endl;
}

void ConnectedCustomerClient::read_deal_handler(const asio::error_code& error)
{
    //std::cout << "connected client read deal completion handler called" << std::endl;
    if (!error) {
        read_deal_.set_menu_ptr(red_zone_.menu_ptr());
        read_deal_.print();
        // Save the deal
        red_zone_.deals_ptr()->emplace(*(red_zone_.deal_counter_ptr()), read_deal_);
        // Push the FoodID items to customer to kitchen queue
        {
            std::lock_guard<std::mutex> lock(red_zone_.ctok_queue_ptr()->mtx);
            for (auto foodid : read_deal_.get_food_list()) {
                for (int i = 0; i < foodid.second; ++i) {
                    red_zone_.ctok_queue_ptr()->foodid_queue.emplace(*(red_zone_.deal_counter_ptr()), foodid.first);
                }
            }
        }
        std::cout << "Deal " << *(red_zone_.deal_counter_ptr()) << " saved and sent to kitchen client." << std::endl;
        // Write the deal number to the customer client
        network_io_.async_write(*(red_zone_.deal_counter_ptr()),
                    strand_.wrap(std::bind(&ConnectedCustomerClient::queueing_handler, shared_from_this(), std::placeholders::_1)));
        // Increment the deal counter
        ++*(red_zone_.deal_counter_ptr());
    } else {
        std::cout << error.message() << std::endl;
        red_zone_.leave(shared_from_this());
    }
    //std::cout << "connected client read deal completion handler returned" << std::endl;
}

void ConnectedCustomerClient::queueing_handler(const asio::error_code& error) {
    if (!error) {
        
    } else {
        std::cout << error.message() << std::endl;
        red_zone_.leave(shared_from_this());
    }
}

// ConnectedKitchenClient

ConnectedKitchenClient::ConnectedKitchenClient(asio::io_context& io_context,
                    asio::io_context::strand& strand,
                    KitchenRedirector& red_zone)
                    : ConnectedClient{io_context, strand},
                      red_zone_(red_zone) { }

void ConnectedKitchenClient::start()
{
    //std::cout << "connected client start called" << std::endl;
    network_io_.async_read(client_id_,
                     strand_.wrap(std::bind(&ConnectedKitchenClient::id_handler, shared_from_this(), std::placeholders::_1)));
    //std::cout << "connected client start returned" << std::endl;
}

void ConnectedKitchenClient::id_handler(const asio::error_code& error)
{
    //std::cout << "connected client id handler called" << std::endl;
    red_zone_.enter(shared_from_this(), client_id_);
    write(*(red_zone_.menu_ptr()));
    std::cout << "Kitchen " << client_id_ << " entered!" << std::endl;
    /* while(true) {
       if(!(red_zone_.ctok_queue_ptr()->foodid_queue.empty())) {
            {
                std::lock_guard<std::mutex> lock(red_zone_.ctok_queue_ptr()->mtx);
                while (!(red_zone_.ctok_queue_ptr()->foodid_queue.empty())) {
                    std::pair<unsigned int, FoodID> tmp = red_zone_.ctok_queue_ptr()->foodid_queue.front();
                    red_zone_.ctok_queue_ptr()->foodid_queue.pop();
                    write(tmp);
                }
            }
            break;
        }
    } */
    network_io_.async_read(read_item_,
                    strand_.wrap(std::bind(&ConnectedKitchenClient::read_item_handler, shared_from_this(), std::placeholders::_1)));
    //std::cout << "connected client id handler returned" << std::endl;
}

void ConnectedKitchenClient::read_item_handler(const asio::error_code& error)
{
    if (!error) {
        red_zone_.ktoc_queue_ptr()->foodid_queue.push(read_item_);
        network_io_.async_read(read_item_,
                        strand_.wrap(std::bind(&ConnectedKitchenClient::read_item_handler, shared_from_this(), std::placeholders::_1)));
    } else {
        std::cout << error.message() << std::endl;
        red_zone_.leave(shared_from_this());
    }
}
