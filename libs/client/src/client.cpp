#include <client.hpp>

/*
    FOR: Interface for writing Pack to socket, to send it to server
    TAKES a Pack to write,
    RETURNS nothing.
*/
void Client::write(const std::string& object) {
    //std::cout << "client write called" << std::endl;
    io_context_.post(std::bind(&Client::write_implementation, this, object));
    //std::cout << "client write returned" << std::endl;
}
/* void Client::write(const Pack& pack) 
{
    //std::cout << "client write called" << std::endl;
    io_context_.post(std::bind(&Client::write_implementation, this, pack));
    //std::cout << "client write returned" << std::endl;
} */

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
        network_io_.async_write(client_id_,
                          std::bind(&Client::connect_handler, this, std::placeholders::_1));
    } else {
        std::cout << error.message() << std::endl;
        close_implementation();
    }
    //std::cout << "client on connect returned" << std::endl;
}

void Client::connect_handler(const asio::error_code& error)
{
    if (!error) {
        network_io_.async_read(read_string_,
                         std::bind(&Client::read_handler, this, std::placeholders::_1));
    } else {
        std::cout << error.message() << std::endl;
        close_implementation();
    }
}


/*
    FOR: Handles reading a received pack into read_pack_
    NEEDS FURTHER CHANGE
*/
void Client::read_handler(const asio::error_code& error)
{
    //std::cout << "client read handler called" << std::endl;
    if (!error) {
        std::cout << read_string_ << std::endl;
        network_io_.async_read(read_string_,
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
void Client::write_implementation(std::string object)
{
    //std::cout << "client write implementation called" << std::endl;
    strings_to_write_.push_back(object);
    if (!strings_to_write_.empty()) {
        network_io_.async_write(strings_to_write_.front(),
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
        strings_to_write_.pop_front();
        if (!strings_to_write_.empty()) {
            network_io_.async_write(strings_to_write_.front(),
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
    network_io_.socket().close();
    strings_to_write_.clear();
    //std::cout << "client close implementation returned" << std::endl;
}