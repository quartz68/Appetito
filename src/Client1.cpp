#include <iostream>
#include <asio.hpp>
#include <client.hpp>

int main(int argc, char* argv[])
{
    try {
        if (argc != 4) {
            std::cerr << "Usage: chat_client <nickname> <host> <port>\n";
            return 1;
        }
        asio::io_context io_context;
        tcp::resolver resolver(io_context);
        tcp::resolver::query query(argv[2], argv[3]);
        tcp::resolver::iterator iterator = resolver.resolve(query);
        ClientID client_id;
        strcpy(client_id.data(), argv[1]);

        Client client(client_id, io_context, iterator);

        std::thread t([&io_context](){io_context.run();});
        
        Pack pack;

        while(true)
        {
            memset(pack.data(), '\0', pack.size());
            std::cin.getline(pack.data(),222);
            client.write(pack);
        }

        client.close();
        t.join();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}