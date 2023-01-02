#ifndef REDIRECTOR_HPP_
#define REDIRECTOR_HPP_

#include <iostream>
#include <mutex>
#include <deque>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <connected_client.hpp>
#include <asio.hpp>
#include <protocol.hpp>
#include <food.hpp>
#include <table.hpp>
#include <menu.hpp>
using asio::ip::tcp;

class Redirector { // How to redirect message based on ID?
public:
    Redirector(FoodContainer* all_foods, TableContainer* all_tables)
        :all_foods_ptr_(all_foods), all_tables_ptr_(all_tables), menu_{all_foods,all_tables} { }
    Menu* menu_ptr() { return &menu_; }
protected:
    Menu menu_;
    FoodContainer* all_foods_ptr_;
    TableContainer* all_tables_ptr_;
};

class CustomerRedirector
    : public Redirector {
public:
    CustomerRedirector(FoodContainer* all_foods, TableContainer* all_tables)
        :Redirector{all_foods, all_tables} { }
    void enter(std::shared_ptr<ConnectedCustomerClient> entering_client, const std::string& client_id);
    void leave(std::shared_ptr<ConnectedCustomerClient> leaving_client);
    template<typename T>
    void write_to_client(T& object)
    {
        //Temporary
        //std::cout << "redirector write to client called" << std::endl;
        //std::cout << pack.data() << std::endl;
        std::for_each(connected_clients_.begin(), connected_clients_.end(),
                        std::bind(&ConnectedClient::write, std::placeholders::_1, std::ref(object)));
        //std::cout << "redirector write to client returned" << std::endl;
    }
protected:
    std::unordered_set<std::shared_ptr<ConnectedCustomerClient>> connected_clients_;
    std::unordered_map<std::shared_ptr<ConnectedCustomerClient>, std::string> id_table_;
};

#endif