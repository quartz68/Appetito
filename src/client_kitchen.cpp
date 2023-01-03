#include <iostream>
#include <map>
#include <mutex>
#include <asio.hpp>
#include <client.hpp>
#include <food.hpp>
#include <table.hpp>
#include <menu.hpp>
#include <order_deal.hpp>
#include <console_menu.hpp>

int main(int argc, char* argv[])
{
    try {
        if (argc != 4) {
            std::cerr << "Usage: client_kitchen <client_id> <host> <port>\n";
            return 1;
        }
        // Get TCP endpoint
        asio::io_context io_context;
        tcp::resolver resolver(io_context);
        tcp::resolver::query query(argv[2], argv[3]);
        tcp::resolver::iterator iterator = resolver.resolve(query);
        // Get client ID
        std::string client_id(argv[1]);
        std::cout << "Client ID: " << client_id << std::endl;
        // Start network client
        FoodIDQueue queue;
        KitchenClient client(client_id, io_context, iterator, &queue);
        std::thread t([&io_context](){io_context.run();});
        asio::steady_timer timer(io_context, asio::chrono::milliseconds(200));
        timer.wait();
        // Construct menus
        /* const std::string title = "Main Menu\n";
        const std::string invalid_choice_message = "The option you chose does not exist.\n";
        const std::string prompt = "Enter your choice:\n";
        const std::map<string, std::function<void()>> commands = {
            {"See menu",std::bind(&Menu::print,std::bind(&Client::menu, client))},
            {"Make order",std::bind(&make_order, client.menu(), client)},
            {"Exit",std::bind(&Client::close, client)}
        };
        ConsoleMenu main_menu(title, invalid_choice_message, prompt, commands, cin, cout);
        // Display menu
        while(true) {
            main_menu();
        }
        Menu menutmp = client.menu();
        make_order(menutmp,client); */
        // Take input
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