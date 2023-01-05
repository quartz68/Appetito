/**
 * @file redirector.hpp
 * @brief Header of Redirector classes.
 * @details
 * @version
 */

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

struct CustomerToKitchenQueue;
struct KitchenToCustomerQueue;

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
     * @param deal_counter Raw pointer to deal counter.
     * @param deals Raw pointer to map of deals.
     */
    Redirector(FoodContainer* all_foods, TableContainer* all_tables, unsigned int* deal_counter, DealContainer* deals)
        :all_foods_ptr_(all_foods), all_tables_ptr_(all_tables), menu_{all_foods,all_tables}, deal_counter_ptr_(deal_counter), deals_ptr_(deals)
        { 
            if (all_foods == nullptr || all_tables == nullptr) {
                cerr << "Trying to set nullptr as FoodContainer or TableContainer pointer!" << endl;
            }
            if (deal_counter == nullptr) {
                cerr << "Trying to set nullptr as deal counter pointer!" << endl;
            }
            if (deals == nullptr) {
                cerr << "Trying to set nullptr as deals map pointer!" << endl;
            }
        }
    /**
     * @brief Gets a raw pointer to menu.
     * 
     * @return Raw pointer to menu.
     */
    Menu* menu_ptr() { return &menu_; }
    /**
     * @brief Gets a raw pointer to deal counter.
     * 
     * @return Raw pointer to deal counter.
     */
    unsigned int* deal_counter_ptr() { return deal_counter_ptr_; }
    /**
     * @brief Gets a raw pointer to deals map.
     * 
     * @return Raw pointer to deals map.
     */
    DealContainer* deals_ptr() { return deals_ptr_; }
protected:
    Menu menu_;
    FoodContainer* all_foods_ptr_;
    TableContainer* all_tables_ptr_;
    unsigned int* deal_counter_ptr_;
    DealContainer* deals_ptr_;
};

/**
 * @brief The specialized redirector for customer clients, which redirects objects to diffrent customer clients.
 */
class CustomerRedirector
    : public Redirector {
public:
/**
     * Construct a new CustomerRedirector object with pointers to FoodContainer, TableContainer, deal counter and queues.
     * @brief Constructor.
     * 
     * @param all_foods Raw pointer to FoodContainer.
     * @param all_tables Raw pointer to TableContainer.
     * @param deal_counter Raw pointer to deal counter.
     * @param ctok_queue Raw pointer to customer to kitchen queue.
     * @param ktoc_queue Raw pointer to kitchen to customer queue.
     * @param deals Raw pointer to map of deals.
     */
    CustomerRedirector(FoodContainer* all_foods,
                       TableContainer* all_tables,
                       unsigned int* deal_counter,
                       CustomerToKitchenQueue* ctok_queue,
                       KitchenToCustomerQueue* ktoc_queue,
                       DealContainer* deals)
        :Redirector{all_foods, all_tables, deal_counter, deals}, ctok_queue_ptr_(ctok_queue), ktoc_queue_ptr_(ktoc_queue)
        {
            if (ctok_queue == nullptr || ktoc_queue == nullptr) {
                cerr << "Trying to set nullptr as CustomerToKitchenQueue or KitchenToCustomerQueue pointer!" << endl;
            }
        }
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
     * @brief Gets a raw pointer to customer to kitchen queue.
     * 
     * @return Raw pointer to customer to kitchen queue.
     */
    CustomerToKitchenQueue* ctok_queue_ptr() { return ctok_queue_ptr_; }
    /**
     * @brief Gets a raw pointer to kitchen to customer queue.
     * 
     * @return Raw pointer to kitchen to customer queue.
     */
    KitchenToCustomerQueue* ktoc_queue_ptr() { return ktoc_queue_ptr_; }
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
        for (auto connected_client : connected_clients_) {
            connected_client->write(object);
        }
        //std::cout << "redirector write to client returned" << std::endl;
    }
protected:
    std::unordered_set<std::shared_ptr<ConnectedCustomerClient>> connected_clients_;
    std::unordered_map<std::shared_ptr<ConnectedCustomerClient>, std::string> id_table_;
    CustomerToKitchenQueue* ctok_queue_ptr_;
    KitchenToCustomerQueue* ktoc_queue_ptr_;
};

/**
 * @brief The specialized redirector for kitchen clients, which redirects objects to diffrent kitchen clients.
 */
class KitchenRedirector
    : public Redirector {
public:
/**
     * Construct a new CustomerRedirector object with pointers to FoodContainer, TableContainer, deal counter and queues.
     * @brief Constructor.
     * 
     * @param all_foods Raw pointer to FoodContainer.
     * @param all_tables Raw pointer to TableContainer.
     * @param deal_counter Raw pointer to deal counter.
     * @param ktoc_queue Raw pointer to kitchen to customer queue.
     * @param ctok_queue Raw pointer to customer to kitchen queue.
     * @param deals Raw pointer to map of deals.
     */
    KitchenRedirector( FoodContainer* all_foods,
                       TableContainer* all_tables,
                       unsigned int* deal_counter,
                       KitchenToCustomerQueue* ktoc_queue,
                       CustomerToKitchenQueue* ctok_queue,
                       DealContainer* deals)
        :Redirector{all_foods, all_tables, deal_counter, deals}, ctok_queue_ptr_(ctok_queue), ktoc_queue_ptr_(ktoc_queue)
        {
            if (ctok_queue == nullptr || ktoc_queue == nullptr) {
                cerr << "Trying to set nullptr as CustomerToKitchenQueue or KitchenToCustomerQueue pointer!" << endl;
            }
        }
    /**
     * @brief Let a ConnectedKitchenClient enter the CustomerRedirector.
     * 
     * @param entering_client Shared_ptr to the entering ConnectedKitchenClient.
     * @param client_id The string of the kitchen client ID.
     */
    void enter(std::shared_ptr<ConnectedKitchenClient> entering_client, const std::string& client_id);
    /**
     * @brief Let a ConnectedKitchenClient leave the KitchenRedirector.
     * 
     * @param leaving_client Shared_ptr to the leaving ConnectedKitchenClient.
     */
    void leave(std::shared_ptr<ConnectedKitchenClient> leaving_client);
    /**
     * @brief Gets a raw pointer to customer to kitchen queue.
     * 
     * @return Raw pointer to customer to kitchen queue.
     */
    CustomerToKitchenQueue* ctok_queue_ptr() { return ctok_queue_ptr_; }
    /**
     * @brief Gets a raw pointer to kitchen to customer queue.
     * 
     * @return Raw pointer to kitchen to customer queue.
     */
    KitchenToCustomerQueue* ktoc_queue_ptr() { return ktoc_queue_ptr_; }
    /**
     * @brief Write object to each customer client in the CustomerRedirector.
     * 
     * @tparam Type of the object.
     * @param object The object to write.
     */
    template<typename T>
    void write_to_client(T& object)
    {
        /* std::for_each<ConnectedClient, T>(connected_clients_.begin(), connected_clients_.end(),
                                std::bind(&ConnectedClient::write, std::placeholders::_1, std::ref(object))); */
        for (auto connected_client : connected_clients_) {
            connected_client->write(object);
        }
    }
protected:
    std::unordered_set<std::shared_ptr<ConnectedKitchenClient>> connected_clients_;
    std::unordered_map<std::shared_ptr<ConnectedKitchenClient>, std::string> id_table_;
    KitchenToCustomerQueue* ktoc_queue_ptr_;
    CustomerToKitchenQueue* ctok_queue_ptr_;
};

#endif