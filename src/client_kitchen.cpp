#include <iostream>
#include <map>
#include <mutex>
#include <algorithm>
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
        std::cout << "Kitchen Client ID: " << client_id << std::endl;
        // Start network client
        FoodIDVector vector;
        KitchenClient client(client_id, io_context, iterator, &vector);
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
        while(true)
        {
            bool done = false;
            unsigned short id1, id2, id3;
            char dash;
            for (std::string line; std::cout << "Enter the ID of the finished item:\n" && std::getline(std::cin, line); )
            {
                std::istringstream iss(line);
                if (iss >> id1 >> dash >> id2 >> dash >> id3 && iss.get() == EOF) { done = true; break; }
                std::cerr << "Failed to parse input '" << line << "', please try again.\n";
            }
            if (!done) { std::cerr << "Premature end of input.\n"; }
            
            pair<unsigned int, FoodID> item{id1, {id2, id3}};
            if (find(vector.vector.begin(), vector.vector.end(), item) != vector.vector.end()) {
                client.write(item);
                auto it = find(vector.vector.begin(), vector.vector.end(), item);
                vector.vector.erase(it);
            } else {
                std::cerr << "No such item, please try again." << std::endl;
            }
        }

        client.close();
        t.join();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}