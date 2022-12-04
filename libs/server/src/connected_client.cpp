#include <connected_client.hpp>
#include <redirector.hpp>

tcp::socket& ConnectedClient::socket() 
{
    return socket_;
}

void ConnectedClient::start()
{
    //std::cout << "connected client start called" << std::endl;
    asio::async_read(socket_,
                     asio::buffer(client_id_, client_id_.size()),
                     strand_.wrap(std::bind(&ConnectedClient::id_handler, shared_from_this(), std::placeholders::_1)));
    //std::cout << "connected client start returned" << std::endl;
}

void ConnectedClient::write(Pack& pack)
{
    //std::cout << "connected client write called" << std::endl;
    packs_to_write_.push_back(pack);
    if (!packs_to_write_.empty()) {
        asio::async_write(socket_,
                          asio::buffer(packs_to_write_.front(), packs_to_write_.front().size()),
                          strand_.wrap(std::bind(&ConnectedClient::write_handler, shared_from_this(), std::placeholders::_1)));
    }
    //std::cout << "connected client write returned" << std::endl;
}

void ConnectedClient::id_handler(const asio::error_code& error)
{
    //std::cout << "connected client id handler called" << std::endl;
    red_zone_.enter(shared_from_this(), client_id_);
    asio::async_read(socket_,
                     asio::buffer(read_pack_, read_pack_.size()),
                     strand_.wrap(std::bind(&ConnectedClient::read_handler, shared_from_this(), std::placeholders::_1)));
    //std::cout << "connected client id handler returned" << std::endl;
}

void ConnectedClient::read_handler(const asio::error_code& error)
{
    //std::cout << "connected client read handler called" << std::endl;
    if (!error) {
        std::cout << read_pack_.data() << std::endl;
        red_zone_.write_to_client(read_pack_, shared_from_this());
        asio::async_read(socket_,
                         asio::buffer(read_pack_, read_pack_.size()),
                         strand_.wrap(std::bind(&ConnectedClient::read_handler, shared_from_this(), std::placeholders::_1)));
    } else {
        std::cout << error.message() << std::endl;
        red_zone_.leave(shared_from_this());
    }
    //std::cout << "connected client read handler returned" << std::endl;
}

void ConnectedClient::write_handler(const asio::error_code& error)
{
    //std::cout << "connected client write handler called" << std::endl;
    if (!error) {
        packs_to_write_.pop_front();
        if (!packs_to_write_.empty()) {
            asio::async_write(socket_,
                              asio::buffer(packs_to_write_.front(), packs_to_write_.front().size()),
                              strand_.wrap(std::bind(&ConnectedClient::write_handler, shared_from_this(), std::placeholders::_1)));
        }
    } else {
        std::cout << error.message() << std::endl;
        red_zone_.leave(shared_from_this());
    }
    //std::cout << "connected client write handler returned" << std::endl;
}

ClientID ConnectedClient::get_id()
{
    //std::cout << "connected client get id called" << std::endl;
    return client_id_;
}