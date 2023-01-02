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

/**
 * @brief The redirector of objects to diffrent clients of the same kind.
 */
class Redirector {
public:
    /**
     * Construct a new Redirector object with pointers to FoodContainer and TableContainer.
     * @brief Constructor.
     * 
     * @param all_foods Raw pointer to FoodContainer.
     * @param all_tables Raw pointer to TableContainer.
     */
    Redirector(FoodContainer* all_foods, TableContainer* all_tables)
        :all_foods_ptr_(all_foods), all_tables_ptr_(all_tables), menu_{all_foods,all_tables} { if (all_foods == nullptr || all_tables == nullptr) cerr << "Trying to set nullptr as FoodContainer or TableContainer pointer!" << endl;}
    /**
     * @brief Gets a raw pointer to menu.
     * 
     * @return Raw pointer to menu.
     */
    Menu* menu_ptr() { return &menu_; }
protected:
    Menu menu_;
    FoodContainer* all_foods_ptr_;
    TableContainer* all_tables_ptr_;
};

/**
 * @brief The specialized redirector for customer clients, which redirects objects to diffrent customer clients.
 */
class CustomerRedirector
    : public Redirector {
public:
/**
     * Construct a new CustomerRedirector object with pointers to FoodContainer and TableContainer.
     * @brief Constructor.
     * 
     * @param all_foods Raw pointer to FoodContainer.
     * @param all_tables Raw pointer to TableContainer.
     */
    CustomerRedirector(FoodContainer* all_foods, TableContainer* all_tables)
        :Redirector{all_foods, all_tables} { }
    /**
     * @brief Let a ConnectedCustomerClient enter the CustomerRedirector.
     * 
     * @param entering_client Shared_ptr to the entering ConnectedCustomerClient.
     * @param client_id The string of the customer client ID.
     */
    void enter(std::shared_ptr<ConnectedCustomerClient> entering_client, const std::string& client_id);
    /**
     * @brief Let a ConnectedCustomerClient leave the CustomerRedirector.
     * 
     * @param leaving_client Shared_ptr to the leaving ConnectedCustomerClient.
     */
    void leave(std::shared_ptr<ConnectedCustomerClient> leaving_client);
    /**
     * @brief Write object to each customer client in the CustomerRedirector.
     * 
     * @tparam Type of the object.
     * @param object The object to write.
     */
    template<typename T>
    void write_to_client(T& object)
    {
        //std::cout << "redirector write to client called" << std::endl;
        std::for_each(connected_clients_.begin(), connected_clients_.end(),
                        std::bind(&ConnectedClient::write, std::placeholders::_1, std::ref(object)));
        //std::cout << "redirector write to client returned" << std::endl;
    }
protected:
    std::unordered_set<std::shared_ptr<ConnectedCustomerClient>> connected_clients_;
    std::unordered_map<std::shared_ptr<ConnectedCustomerClient>, std::string> id_table_;
};

#endif