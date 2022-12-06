#ifndef CUSTOMER_MENU_HPP_
#define CUSTOMER_MENU_HPP_

#include <vector>
#include <regex>
#include <food.hpp>
#include <table.hpp>
#include <protocol.hpp>
#include <cereal.hpp>
#include <cereal/archives/binary.hpp>
using namespace std;
using Pack = std::array<char, MAX_PACK_SIZE>; // Pack is an array of char, for transmitting pack
using ClientID = std::array<char, MAX_ID_SIZE>; // ID is an array of char, for identifying the client

class Menu {
public:
    Menu(FoodContainer& all_foods, TableContainer& all_tables)
    {
        for (auto food : all_foods.foods) {
            types_foods_[food.first.type].push_back(food.second);
        }
        for (auto table : all_tables.tables) {
            types_tables_[table.first.type].push_back(table.second);
        }
    };
    void get_menu();
    void refresh_menu();
    void print_menu();
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(types_foods_,types_tables_);
    }
private:
    vector<vector<Food>> types_foods_;
    vector<vector<Table>> types_tables_;
};

#endif