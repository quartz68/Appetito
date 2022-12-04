#include <client.hpp>

/*
    FOR: Interface for writing Pack to socket, to send it to server
    TAKES a Pack to write,
    RETURNS nothing.
*/
void Client::write(const Pack& pack) 
{
    //std::cout << "client write called" << std::endl;
    io_context_.post(std::bind(&Client::write_implementation, this, pack));
    //std::cout << "client write returned" << std::endl;
}

/*
    FOR: Interface for closing the socket
    TAKES nothing,
    RETURNS nothing.
*/
void Client::close() 
{
    //std::cout << "client close called" << std::endl;
    io_context_.post(std::bind(&Client::close_implementation, this));
    //std::cout << "client close returned" << std::endl;
}

/*
    FOR: Writes the client ID to socket, to send to server for identification
    TAKES nothing,
    RETURNS nothing.
*/
void Client::on_connect(const asio::error_code& error)
{
    //std::cout << "client on connect called" << std::endl;
    if (!error) {
        asio::async_write(socket_,
                          asio::buffer(client_id_, client_id_.size()),
                          std::bind(&Client::read_handler, this, std::placeholders::_1));
    } else {
        std::cout << error.message() << std::endl;
        close_implementation();
    }
    //std::cout << "client on connect returned" << std::endl;
}

/*
    FOR: Handles reading a received pack into read_pack_
    NEEDS FURTHER CHANGE
*/
void Client::read_handler(const asio::error_code& error)
{
    //std::cout << "client read handler called" << std::endl;
    std::cout << read_pack_.data() << std::endl;
    if (!error) {
        asio::async_read(socket_,
                         asio::buffer(read_pack_, read_pack_.size()),
                         std::bind(&Client::read_handler, this, std::placeholders::_1));
    } else {
        std::cout << error.message() << std::endl;
        close_implementation();
    }
    //std::cout << "client read handler returned" << std::endl;
}

/*
    FOR: Implementation of Client::write(): writes a Pack to socket, to send it to server
    TAKES a Pack to write,
    RETURNS nothing.
*/
void Client::write_implementation(Pack pack)
{
    //std::cout << "client write implementation called" << std::endl;
    packs_to_write_.push_back(pack);
    if (!packs_to_write_.empty()) {
        asio::async_write(socket_,
                          asio::buffer(packs_to_write_.front(), packs_to_write_.front().size()),
                          std::bind(&Client::write_handler, this, std::placeholders::_1));
    }
    //std::cout << "client write implementation returned" << std::endl;
}

/*
    FOR: Handles writing the Packs waiting to write in deque to socket
    TAKES an error_code,
    RETURNS nothing.
*/
void Client::write_handler(const asio::error_code& error)
{
    //std::cout << "client write handler called" << std::endl;
    if (!error) {
        packs_to_write_.pop_front();
        if (!packs_to_write_.empty()) {
            asio::async_write(socket_,
                              asio::buffer(packs_to_write_.front(), packs_to_write_.front().size()),
                              std::bind(&Client::write_handler, this, std::placeholders::_1));
            
        } 
    } else {
            std::cout << error.message() << std::endl;
            close_implementation();
    }
    //std::cout << "client write handler returned" << std::endl;
}

/*
    FOR: Implementation of Client::close() : closes the socket
    TAKES nothing,
    RETURNS nothing.
*/
void Client::close_implementation() 
{
    //std::cout << "client close implementation called" << std::endl;
    socket_.close();
    //std::cout << "client close implementation returned" << std::endl;
}