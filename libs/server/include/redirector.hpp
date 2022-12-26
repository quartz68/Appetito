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
    void enter(std::shared_ptr<ConnectedClient> entering_client, const std::string& client_id);
    void leave(std::shared_ptr<ConnectedClient> leaving_client);
    Menu& menu() { return menu_; }
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
private:
    std::unordered_set<std::shared_ptr<ConnectedClient>> connected_clients_;
    std::unordered_map<std::shared_ptr<ConnectedClient>, std::string> id_table_;
    Menu menu_;
    FoodContainer* all_foods_ptr_;
    TableContainer* all_tables_ptr_;
};

#endif