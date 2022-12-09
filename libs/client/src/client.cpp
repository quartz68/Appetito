#include <client.hpp>

/*
    FOR: Interface for writing Pack to socket, to send it to server
    TAKES a Pack to write,
    RETURNS nothing.
*/
void Client::write(const std::string& object) {
    std::cout << "client write called" << std::endl;
    io_context_.post(std::bind(&Client::write_implementation, this, object));
    std::cout << "client write returned" << std::endl;
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
    std::cout << "client close called" << std::endl;
    io_context_.post(std::bind(&Client::close_implementation, this));
    std::cout << "client close returned" << std::endl;
}

/*
    FOR: Writes the client ID to socket, to send to server for identification
    TAKES nothing,
    RETURNS nothing.
*/
void Client::on_connect(const asio::error_code& error)
{
    std::cout << "client on connect called" << std::endl;
    if (!error) {
        asio::async_write(network_io_.socket_,
                          asio::buffer(client_id_, MAX_ID_SIZE),
                          std::bind(&Client::connect_handler, this, std::placeholders::_1));
    } else {
        std::cout << error.message() << std::endl;
        close_implementation();
    }
    std::cout << "client on connect returned" << std::endl;
}

void Client::connect_handler(const asio::error_code& error)
{
    if (!error) {
        asio::async_read(network_io_.socket_,
                         asio::buffer(network_io_.inbound_data_,MAX_PACK_SIZE),
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
    std::cout << "client read handler called" << std::endl;
    if (!error) {
        std::string pack;
        {
            std::istringstream in_archive_stream;
            strcpy(in_archive_stream.str().data(),network_io_.inbound_data_.data());
            cereal::BinaryInputArchive in_archive(in_archive_stream);
            in_archive >> pack;
        }
        std::cout << network_io_.inbound_data_.data() << std::endl;
        std::cout << pack << std::endl;
        //red_zone_.write_to_client(pack,shared_from_this());
        asio::async_read(network_io_.socket_,
                         asio::buffer(network_io_.inbound_data_,MAX_PACK_SIZE),
                         std::bind(&Client::read_handler, this, std::placeholders::_1));
    } else {
        std::cout << error.message() << std::endl;
        close_implementation();
    }
    std::cout << "client read handler returned" << std::endl;
}

/*
    FOR: Implementation of Client::write(): writes a Pack to socket, to send it to server
    TAKES a Pack to write,
    RETURNS nothing.
*/
void Client::write_implementation(std::string object)
{
    std::cout << "client write implementation called" << std::endl;
    strings_to_write_.push_back(object);
    if (!strings_to_write_.empty()) {
        {
            std::ostringstream out_archive_stream;
            cereal::BinaryOutputArchive out_archive(out_archive_stream);
            out_archive << strings_to_write_.front();
            strcpy(network_io_.outbound_data_.data(), out_archive_stream.str().data());
        }
        asio::async_write(network_io_.socket_,
                          asio::buffer(network_io_.outbound_data_, MAX_PACK_SIZE),
                          std::bind(&Client::write_handler, this, std::placeholders::_1));
    }
    std::cout << "client write implementation returned" << std::endl;
}

/*
    FOR: Handles writing the Packs waiting to write in deque to socket
    TAKES an error_code,
    RETURNS nothing.
*/
void Client::write_handler(const asio::error_code& error)
{
    std::cout << "client write handler called" << std::endl;
    if (!error) {
        strings_to_write_.pop_front();
        if (!strings_to_write_.empty()) {
            {
                std::cout << strings_to_write_.front();
                std::ostringstream out_archive_stream;
                cereal::BinaryOutputArchive out_archive(out_archive_stream);
                out_archive << strings_to_write_.front();
                strcpy(network_io_.outbound_data_.data(), out_archive_stream.str().data());
            }
            asio::async_write(network_io_.socket_, 
                            asio::buffer(network_io_.outbound_data_, MAX_PACK_SIZE),
                            std::bind(&Client::write_handler, this, std::placeholders::_1));
        } 
    } else {
            std::cout << error.message() << std::endl;
            close_implementation();
    }
    std::cout << "client write handler returned" << std::endl;
}

/*
    FOR: Implementation of Client::close() : closes the socket
    TAKES nothing,
    RETURNS nothing.
*/
void Client::close_implementation() 
{
    std::cout << "client close implementation called" << std::endl;
    network_io_.socket_.close();
    std::cout << "client close implementation returned" << std::endl;
}