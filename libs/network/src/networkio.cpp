#include <networkio.hpp>

/*
    FOR: Handles reading from socket.
    TAKES an error code,
    RETURNS nothing.
*/
void ServerNetworkIO::read_handler(const asio::error_code& error)
{
    std::cout << "network io read handler called" << std::endl;
    if (!error) {
        std::string pack;
        ins >> pack;
        std::cout << pack << std::endl;
        //red_zone_.write_to_client(pack);
        asio::async_read(socket_,
                         buf_in_,
                         strand_.wrap(std::bind(&ServerNetworkIO::read_handler, shared_from_this(), std::placeholders::_1)));
    } else {
        std::cout << error.message() << std::endl;
        //red_zone_.leave(shared_from_this());
    }
    std::cout << "network io read handler returned" << std::endl;
}

/*
    FOR: Handles writing packs waiting to be written.
    TAKES an error code,
    RETURNS nothing.
*/
void ServerNetworkIO::write_handler(const asio::error_code& error)
{
    std::cout << "network io write handler called" << std::endl;
    if (!error) {
        asio::async_write(socket_,
                          buf_out_,
                          strand_.wrap(std::bind(&ServerNetworkIO::write_handler, shared_from_this(), std::placeholders::_1)));
    } else {
        std::cout << error.message() << std::endl;
        //red_zone_.leave(shared_from_this());
    }
    std::cout << "network io write handler returned" << std::endl;
}
