/**
 * @file networkio.hpp
 * @brief Header of NetworkIO class.
 * @details
 * @version
 */

#ifndef NETWORKIO_HPP_
#define NETWORKIO_HPP_

#include <iostream>
#include <iomanip>
#include <vector>
#include <tuple>
#include <asio.hpp>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>

/**
 * @brief Handles TCP/IP network input/output.
 */
class NetworkIO
{
public:
    /**
     * Construct a new NetworkIO object from io context.
     * @brief Constructor.
     * 
     * @param io_context 
     */
    NetworkIO(asio::io_context& io_context)
        : io_context_(io_context), socket_(io_context) { }
    /**
     * @brief Get the underlying socket.
     * 
     * @return Reference to the TCP socket.
     */
    asio::ip::tcp::socket& socket()
    {
        return socket_;
    }
    /**
     * @brief Asynchronously write a data structure to the socket.
     * 
     * @tparam T The type of the data structure.
     * @tparam Handler The type of the handler.
     * @param t The data structure.
     * @param handler The handler.
     */
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

        // Write the serialized data to the socket, both the header and the data.
        std::vector<asio::const_buffer> buffers;
        buffers.push_back(asio::buffer(outbound_header_));
        buffers.push_back(asio::buffer(outbound_data_));
        asio::async_write(socket_, buffers, handler);
    }
    /**
     * @brief Asynchronously read a data structure from the socket.
     * 
     * @tparam T The type of the data structure.
     * @tparam Handler The type of the handler.
     * @param t The data structure.
     * @param handler The handler.
     */
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
    /**
     * @brief Handle a completed read of a message header. 
     * 
     * @param t 
     * @param handler The handler is passed using a std::tuple.
    */
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
    /**
     * @brief Handle a completed read of a message data. 
     * 
     * @param t 
     * @param handler The handler is passed using a std::tuple.
    */
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
    asio::ip::tcp::socket socket_; /**< The underlying socket. */
    asio::io_context& io_context_; /**< The io_context. */
    enum { HEADER_SIZE = 8 }; /**< The size of a fixed length header. */
    std::string outbound_header_; /**< Holds an outbound header. */
    std::string outbound_data_; /**< Holds the outbound data. */
    char inbound_header_[HEADER_SIZE]; /**< Holds an inbound header. */
    std::vector<char> inbound_data_; /**< Holds the inbound data. */
};

#endif