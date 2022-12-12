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
#include <customer_menu.hpp>
using asio::ip::tcp;
using Pack = std::array<char, MAX_PACK_SIZE>; // Pack is an array of char, for transmitting pack
using ClientID = std::array<char, MAX_ID_SIZE>; // ID is an array of char, for identifying the client

class Redirector { // How to redirect message based on ID?
public:
    Redirector(FoodContainer& all_foods, TableContainer& all_tables)
        :all_foods_(all_foods), all_tables_(all_tables) { }
    void enter(std::shared_ptr<ConnectedClient> entering_client, const std::string& client_id);
    void leave(std::shared_ptr<ConnectedClient> leaving_client);
    FoodContainer& food_container() { return all_foods_; } 
    TableContainer& table_container() { return all_tables_; } 
    template<typename T>
    void Redirector::write_to_client(T& object)
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
    std::deque<Pack> recent_packs_;
    FoodContainer& all_foods_;
    TableContainer& all_tables_;
};

#endif