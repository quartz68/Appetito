#ifndef NETWORKIO_HPP_
#define NETWORKIO_HPP_

#include <iostream>
#include <iomanip>
#include <vector>
#include <tuple>
#include <asio.hpp>
#include <protocol.hpp>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>

class NetworkIO
{
public:
    NetworkIO(asio::io_context& io_context)
        : io_context_(io_context), socket_(io_context) { }

    // Get the underlying socket
    asio::ip::tcp::socket& socket()
    {
        return socket_;
    }

    // Asynchronously write a data structure to the socket
    template <typename T, typename Handler>
    void async_write(const T& t, Handler handler)
    {
        // Serialize the data first so we know how large it is.
        std::ostringstream archive_stream;
        cereal::BinaryOutputArchive archive(archive_stream);
        archive << t;
        outbound_data_ = archive_stream.str();

        // Format the header.
        std::ostringstream header_stream;
        header_stream << std::setw(HEADER_SIZE) << std::hex << outbound_data_.size();
        if (!header_stream || header_stream.str().size() != HEADER_SIZE)
        {
            // Something went wrong, inform the caller.
            asio::error_code error(asio::error::invalid_argument);
            io_context_.post(std::bind(handler, error));
            return;
        }
        outbound_header_ = header_stream.str();

        // Write the serialized data to the socket. We use "gather-write" to send
        // both the header and the data in a single write operation.
        std::vector<asio::const_buffer> buffers;
        buffers.push_back(asio::buffer(outbound_header_));
        buffers.push_back(asio::buffer(outbound_data_));
        asio::async_write(socket_, buffers, handler);
    }

    /// Asynchronously read a data structure from the socket.
    template <typename T, typename Handler>
    void async_read(T& t, Handler handler)
    {
        // Issue a read operation to read exactly the number of bytes in a header.
        void (NetworkIO::*f)(
            const asio::error_code&,
            T&, std::tuple<Handler>)
        = &NetworkIO::read_header_handler<T, Handler>;
        asio::async_read(socket_, asio::buffer(inbound_header_),
            std::bind(f,
            this, std::placeholders::_1, std::ref(t),
            std::make_tuple(handler)));
    }

    /// Handle a completed read of a message header. The handler is passed using
    /// a tuple.
    template <typename T, typename Handler>
    void read_header_handler(const asio::error_code& e,
        T& t, std::tuple<Handler> handler)
    {
        if (e) {
            std::get<0>(handler)(e);
        }
        else {
            // Determine the length of the serialized data.
            std::istringstream is(std::string(inbound_header_, HEADER_SIZE));
            std::size_t inbound_data_size = 0;
            if (!(is >> std::hex >> inbound_data_size))
            {
                // Header doesn't seem to be valid. Inform the caller.
                asio::error_code error(asio::error::invalid_argument);
                std::get<0>(handler)(error);
                return;
            }

            // Start an asynchronous call to receive the data.
            inbound_data_.resize(inbound_data_size);
            void (NetworkIO::*f)(
                const asio::error_code&,
                T&, std::tuple<Handler>)
                = &NetworkIO::read_data_handler<T, Handler>;
            asio::async_read(socket_, asio::buffer(inbound_data_),
                std::bind(f, this,
                std::placeholders::_1, std::ref(t), handler));
        }
    }

    /// Handle a completed read of message data.
    template <typename T, typename Handler>
    void read_data_handler(const asio::error_code& e,
        T& t, std::tuple<Handler> handler)
    {
        if (e) {
            std::get<0>(handler)(e);
        }
        else {
            // Extract the data structure from the data just received.
            try
            {
                std::string archive_data(&inbound_data_[0], inbound_data_.size());
                std::istringstream archive_stream(archive_data);
                cereal::BinaryInputArchive archive(archive_stream);
                archive >> t;
            }
            catch (std::exception& e)
            {
                // Unable to decode data.
                asio::error_code error(asio::error::invalid_argument);
                std::get<0>(handler)(error);
                return;
            }

        // Inform caller that data has been received ok.
        std::get<0>(handler)(e);
        }
    }

    private:
    /// The underlying socket.
    asio::ip::tcp::socket socket_;

    asio::io_context& io_context_;

    /// The size of a fixed length header.
    enum { HEADER_SIZE = 8 };

    /// Holds an outbound header.
    std::string outbound_header_;

    /// Holds the outbound data.
    std::string outbound_data_;

    /// Holds an inbound header.
    char inbound_header_[HEADER_SIZE];

    /// Holds the inbound data.
    std::vector<char> inbound_data_;
};

#endif