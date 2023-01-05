#include <iostream>
#include <map>
#include <limits>
#include <asio.hpp>
#include <client.hpp>
#include <food.hpp>
#include <table.hpp>
#include <menu.hpp>
#include <order_deal.hpp>
#include <console_menu.hpp>
#include <protocol.hpp>

#define TIMER_WAIT_MILLISECONDS 200

void make_order(Menu& menu, CustomerClient* client){
    Order order(menu);
    // Select table
    system("cls");
    std::cout << SELECT_TABLE_HEADER << std::endl;
    menu.print_tables();
    std::cout << BOTTOM << std::endl;
    Table table;
    while (true) {

        bool done = false;
        unsigned short id1, id2;
        char dash;
        for (std::string line; std::cout << "Select your table: " && std::getline(std::cin, line); )
        {
            std::istringstream iss(line);
            if (iss >> id1 >> dash >> id2 && iss.get() == EOF) { done = true; break; }
            std::cerr << "Failed to parse input '" << line << "', please try again.\n";
        }
        if (!done) { std::cerr << "Premature end of input.\n"; }

        TableID table_id;
        table_id.type = id1;
        table_id.id = id2;
        auto itemit = menu.tables_.find(table_id);
        if (itemit != menu.tables_.end()) {
            table = (*itemit).second;
            break;
        } else {
            std::cout << "The table does not exist." << std::endl;
        }
    }
    // Select food
    system("cls");
    std::cout << SELECT_FOOD_HEADER << std::endl;
    menu.print_foods();
    std::cout << BOTTOM << std::endl;
    std::cout << "Add item : \t\t1 ID quantity\nDelete item : \t\t2 ID quantity\nPreview order : \t3\nFinish ordering : \t4\n" << std::endl;
    unsigned short choice;
    while(std::cin >> choice){
        char dash;
        switch(choice){
            case 1: // Add item
            {
                unsigned short id1,id2,quantity;
                FoodID food_id;
                std::cin >> id1 >> dash >> id2 >> quantity;
                food_id.type = id1;
                food_id.id = id2;
                auto itemit = menu.foods_.find(food_id);
                if (itemit != menu.foods_.end()) {
                    order.add_item(itemit->second,quantity);
                    std::cout << "Added: " << quantity << " * " << itemit->second.get_name() << std::endl;
                } else {
                    std::cout << "The item does not exist." << std::endl;
                }
                break;
            }
            case 2: // Delete item
            {
                unsigned short id1,id2,quantity;
                FoodID food_id;
                std::cin >> id1 >> dash >> id2 >> quantity;
                food_id.type = id1;
                food_id.id = id2;
                auto itemit = menu.foods_.find(food_id);
                if (itemit != menu.foods_.end()) {
                    order.delete_item((*itemit).second,quantity);
                    std::cout << "Deleted: " << quantity << " * " << itemit->second.get_name() << std::endl;
                } else {
                    std::cout << "The item does not exist." << std::endl;
                }
                break;
            }
            case 3: // Preview order
            {
                Deal deal_tmp(order,table);
                std::cout << PREVIEW_ORDER_HEADER<< std::endl;
                deal_tmp.print();
                std::cout << BOTTOM << std::endl;
                break;
            }
            case 4: // Send order
            {
                Deal deal(order,table);
                client->save_deal(deal);
                client->write(deal);
                break;
            }
            default:
            {
                std::cout << "Invalid choice." << std::endl;
                break;
            }
        }
        if(choice == 4) {
            cin.ignore();
            asio::io_context ioc;
            asio::steady_timer timer(ioc, std::chrono::milliseconds(TIMER_WAIT_MILLISECONDS));
            timer.wait();
            break;
        }
    }
}

void print_menu(CustomerClient* client) {
    system("cls");
    client->menu().print();
}

void print_current_deal(CustomerClient* client) {
    client->print_current_deal();
}

void print_past_deals(CustomerClient* client) {
    client->print_past_deals();
}

void menu_exit(bool* is_exit) {
    *is_exit = true;
}

int main(int argc, char* argv[])
{
    try {
        if (argc != 4) {
            std::cerr << "Usage: client_customer <client_id> <host> <port>\n";
            return 1;
        }
        // Get TCP endpoint
        asio::io_context io_context;
        tcp::resolver resolver(io_context);
        tcp::resolver::query query(argv[2], argv[3]);
        tcp::resolver::iterator iterator = resolver.resolve(query);
        // Get client ID
        std::string client_id(argv[1]);
        // Start network client
        CustomerClient client(client_id, io_context, iterator);
        std::thread t([&io_context](){io_context.run();});
        asio::steady_timer timer(io_context, std::chrono::milliseconds(TIMER_WAIT_MILLISECONDS));
        timer.wait();
        // Construct menus
        bool is_exit = false;
        const std::string title = "Main Menu\n";
        const std::string invalid_choice_message = "The option you chose does not exist.\n";
        const std::string prompt = "Enter your choice:\n";
        const std::map< string, pair<string,function<void()>> > commands = {
            { "1", {"See menu",std::bind(&print_menu, &client)} },
            { "2", {"Make order",std::bind(&make_order, client.menu(), &client)} },
            { "3", {"Current deal",std::bind(&print_current_deal, &client)} },
            { "4", {"Past deals",std::bind(&print_past_deals, &client)} },
            { "5", {"Exit",std::bind(&menu_exit, &is_exit)} }
        };
        ConsoleMenu main_menu(title, invalid_choice_message, prompt, commands, cin, cout);
        // Display menu
        bool is_first_time_opening_menu = true;
        while(true) {
            if (is_first_time_opening_menu) {
                system("cls");
                std::cout << "Customer Client ID: " << client_id << std::endl;
                is_first_time_opening_menu = false;
            }
            main_menu();
            if (is_exit == true) break;
        }

        client.close();
        t.join();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}