/**
 * @file redirector.cpp
 * @brief Implementation of Redirector classes.
 * @details
 * @version
 */

#include <redirector.hpp>
#include <server.hpp>

void CustomerRedirector::enter(std::shared_ptr<ConnectedCustomerClient> entering_client, const std::string& client_id)
{
    //std::cout << "redirector enter called" << std::endl;
    connected_clients_.insert(entering_client);
    id_table_[entering_client] = client_id;
    //std::cout << "redirector enter returned" << std::endl;
}

void CustomerRedirector::leave(std::shared_ptr<ConnectedCustomerClient> leaving_client)
{
    //std::cout << "redirector leave called" << std::endl;
    connected_clients_.erase(leaving_client);
    //std::cout << "redirector leave returned" << std::endl;
}

void KitchenRedirector::enter(std::shared_ptr<ConnectedKitchenClient> entering_client, const std::string& client_id)
{
    //std::cout << "redirector enter called" << std::endl;
    connected_clients_.insert(entering_client);
    id_table_[entering_client] = client_id;
    //std::cout << "redirector enter returned" << std::endl;
}

void KitchenRedirector::leave(std::shared_ptr<ConnectedKitchenClient> leaving_client)
{
    //std::cout << "redirector leave called" << std::endl;
    connected_clients_.erase(leaving_client);
    //std::cout << "redirector leave returned" << std::endl;
}