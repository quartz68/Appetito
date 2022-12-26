#include <connected_client.hpp>
#include <redirector.hpp>

tcp::socket& ConnectedClient::socket() 
{
    return network_io_.socket();
}

/*
    FOR: Starts the connection, gets the client ID.
    TAKES nothing,
    RETURNS nothing.
*/
void ConnectedClient::start()
{
    //std::cout << "connected client start called" << std::endl;
    network_io_.async_read(client_id_,
                     strand_.wrap(std::bind(&ConnectedClient::id_handler, shared_from_this(), std::placeholders::_1)));
    //std::cout << "connected client start returned" << std::endl;
}

/*
    FOR: Writes a pack to the client.
    TAKES a pack to write,
    RETURNS nothing.
*/
/* void ConnectedClient::write(std::string& object) {
    //std::cout << "connected client write called" << std::endl;
    strings_to_write_.push_back(object);
    if (!strings_to_write_.empty()) {
        network_io_.async_write(strings_to_write_.front(),
                            strand_.wrap(std::bind(&ConnectedClient::write_handler, shared_from_this(), std::placeholders::_1)));
    }
    //std::cout << "connected client write returned" << std::endl;
}

void ConnectedClient::write(Menu menu) {
    network_io_.async_write( menu,
                             strand_.wrap(std::bind(&ConnectedClient::write_handler, shared_from_this(), std::placeholders::_1)));
} */

/*
    FOR: Handles ID, client enters redirector, starts to read packs.
    TAKES an error code,
    RETURNS nothing.
*/
void ConnectedClient::id_handler(const asio::error_code& error)
{
    std::cout << "connected client id handler called" << std::endl;
    red_zone_.enter(shared_from_this(), client_id_);
    write(red_zone_.menu());
    std::cout << client_id_ << " entered!" << std::endl;
    network_io_.async_read(read_string_,
                    strand_.wrap(std::bind(&ConnectedClient::read_handler, shared_from_this(), std::placeholders::_1)));
    std::cout << "connected client id handler returned" << std::endl;
}


/*
    FOR: Handles reading from socket.
    TAKES an error code,
    RETURNS nothing.
*/
void ConnectedClient::read_handler(const asio::error_code& error)
{
    std::cout << "connected client read handler called" << std::endl;
    if (!error) {
        /* std::cout << read_pack_.data() << std::endl;
        red_zone_.write_to_client(read_pack_, shared_from_this());
        asio::async_read(network_io_.socket_,
                         asio::buffer(read_pack_, read_pack_.size()),
                         strand_.wrap(std::bind(&ConnectedClient::read_handler, shared_from_this(), std::placeholders::_1))); */
        /* {
            std::istringstream in_archive_stream;
            strcpy(in_archive_stream.str().data(),network_io_.inbound_data_.data());
            cereal::BinaryInputArchive in_archive(in_archive_stream);
            in_archive >> pack;
            std::cout << pack << std::endl;
        } */
        network_io_.async_read(read_string_,
                         strand_.wrap(std::bind(&ConnectedClient::read_handler, shared_from_this(), std::placeholders::_1)));
    } else {
        std::cout << error.message() << std::endl;
        red_zone_.leave(shared_from_this());
    }
    std::cout << "connected client read handler returned" << std::endl;
}

/*
    FOR: Handles writing packs waiting to be written.
    TAKES an error code,
    RETURNS nothing.
*/
void ConnectedClient::write_handler(const asio::error_code& error)
{
    std::cout << "connected client write handler called" << std::endl;
    if (!error) {
        if (!strings_to_write_.empty()) {
            network_io_.async_write(strings_to_write_.front(),
                                strand_.wrap(std::bind(&ConnectedClient::read_handler, shared_from_this(), std::placeholders::_1)));
        }
    } else {
        std::cout << error.message() << std::endl;
        red_zone_.leave(shared_from_this());
    }
    std::cout << "connected client write handler returned" << std::endl;
}

/*
    RETURNS client id.
*/
std::string ConnectedClient::get_id()
{
    //std::cout << "connected client get id called" << std::endl;
    return client_id_;
}