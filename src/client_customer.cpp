#include <iostream>
#include <asio.hpp>
#include <client.hpp>
#include <food.hpp>
#include <table.hpp>
#include <menu.hpp>
#include <order_deal.hpp>



int main(int argc, char* argv[])
{
    try {
        if (argc != 4) {
            std::cerr << "Usage: client_customer <client_id> <host> <port>\n";
            return 1;
        }
        asio::io_context io_context;
        tcp::resolver resolver(io_context);
        tcp::resolver::query query(argv[2], argv[3]);
        tcp::resolver::iterator iterator = resolver.resolve(query);
        std::string client_id(argv[1]);
        std::cout << "Client ID: " << client_id << std::endl;
        Client client(client_id, io_context, iterator);

        std::thread t([&io_context](){io_context.run();});

        std::string pack;

        while(true)
        {
            cin >> pack;
            client.write(pack);
        }

        client.close();
        t.join();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}