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
#include <customer_menu.hpp>
using asio::ip::tcp;
using Pack = std::array<char, MAX_PACK_SIZE>; // Pack is an array of char, for transmitting pack
using ClientID = std::array<char, MAX_ID_SIZE>; // ID is an array of char, for identifying the client

class Redirector { // How to redirect message based on ID?
public:
    void enter(std::shared_ptr<ConnectedClient> entering_client, const std::string& client_id);
    void leave(std::shared_ptr<ConnectedClient> leaving_client);
    void write_to_client(std::string& object, std::shared_ptr<ConnectedClient> client);
private:
    std::unordered_set<std::shared_ptr<ConnectedClient>> connected_clients_;
    std::unordered_map<std::shared_ptr<ConnectedClient>, std::string> id_table_;
    std::deque<Pack> recent_packs_;
};

#endif