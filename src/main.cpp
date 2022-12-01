#include <iostream>
#include <asio.hpp>

int main()
{
    asio::io_context io;
    asio::steady_timer t(io, asio::chrono::seconds(5));
    t.wait();
    std::cout << "Hi Asio" << std::endl;
    return 0;
}