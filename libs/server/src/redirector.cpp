#include <redirector.hpp>

void Redirector::enter(std::shared_ptr<ConnectedClient> entering_client, const std::string& client_id)
{
    //std::cout << "redirector enter called" << std::endl;
    connected_clients_.insert(entering_client);
    id_table_[entering_client] = client_id;
    //std::cout << "redirector enter returned" << std::endl;
}

void Redirector::leave(std::shared_ptr<ConnectedClient> leaving_client)
{
    //std::cout << "redirector leave called" << std::endl;
    connected_clients_.erase(leaving_client);
    //std::cout << "redirector leave returned" << std::endl;
}