#include <redirector.hpp>

void Redirector::enter(std::shared_ptr<ConnectedClient> entering_client, const ClientID& client_id)
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

void Redirector::write_to_client(std::string& pack, std::shared_ptr<ConnectedClient> client)
{
    //Temporary
    //std::cout << "redirector write to client called" << std::endl;
    Pack fpack;
    strcpy(fpack.data(),client->get_id().data());
    strcat(fpack.data(), pack.data());
    //std::cout << pack.data() << std::endl;
    std::for_each(connected_clients_.begin(), connected_clients_.end(),
                      std::bind(&ConnectedClient::write, std::placeholders::_1, std::ref(fpack)));
    //std::cout << "redirector write to client returned" << std::endl;
}