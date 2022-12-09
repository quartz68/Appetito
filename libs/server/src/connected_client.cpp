#include <connected_client.hpp>
#include <redirector.hpp>

tcp::socket& ConnectedClient::socket() 
{
    return network_io_.socket_;
}

/*
    FOR: Starts the connection, gets the client ID.
    TAKES nothing,
    RETURNS nothing.
*/
void ConnectedClient::start()
{
    std::cout << "connected client start called" << std::endl;
    asio::async_read(network_io_.socket_,
                     asio::buffer(client_id_, client_id_.size()),
                     strand_.wrap(std::bind(&ConnectedClient::id_handler, shared_from_this(), std::placeholders::_1)));
    std::cout << "connected client start returned" << std::endl;
}

/*
    FOR: Writes a pack to the client.
    TAKES a pack to write,
    RETURNS nothing.
*/
void ConnectedClient::write(std::string& object) {
    std::cout << "connected client write called" << std::endl;
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
                            strand_.wrap(std::bind(&ConnectedClient::write_handler, shared_from_this(), std::placeholders::_1)));
    }
    std::cout << "connected client write returned" << std::endl;
}
/* void ConnectedClient::write(std::string& pack)
{
    if (!packs_to_write_.empty()) {
        // Create an ostream object that wraps the asio::streambuf.
        std::ostream out(&network_io_.buf_out_);

        // Write the data to the buffer using the ostream.
        out << packs_to_write_.front();

        // Initiate the asynchronous write operation using the buffer.
        asio::async_write(network_io_.socket_, network_io_.buf_out_,
            strand_.wrap(std::bind(&ConnectedClient::write_handler, shared_from_this(), std::placeholders::_1)));
    } else {
        std::cout << error.message() << std::endl;
        red_zone_.leave(shared_from_this());
    }
} */

/*
    FOR: Handles ID, client enters redirector, starts to read packs.
    TAKES an error code,
    RETURNS nothing.
*/
void ConnectedClient::id_handler(const asio::error_code& error)
{
    std::cout << "connected client id handler called" << std::endl;
    string s = "fasdfada!";
    red_zone_.enter(shared_from_this(), client_id_);
    write(s);
    asio::async_read(network_io_.socket_,
                    asio::buffer(network_io_.inbound_data_, MAX_PACK_SIZE),
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
        std::string pack;
        /* {
            std::istringstream in_archive_stream;
            strcpy(in_archive_stream.str().data(),network_io_.inbound_data_.data());
            cereal::BinaryInputArchive in_archive(in_archive_stream);
            in_archive >> pack;
            std::cout << pack << std::endl;
        } */
        std::cout << network_io_.inbound_data_.data() << std::endl;
        red_zone_.write_to_client(pack,shared_from_this());
        asio::async_read(network_io_.socket_,
                         asio::buffer(network_io_.inbound_data_,MAX_PACK_SIZE),
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
ClientID ConnectedClient::get_id()
{
    std::cout << "connected client get id called" << std::endl;
    return client_id_;
}