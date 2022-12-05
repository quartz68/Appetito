#ifndef CUSTOMER_MENU_HPP_
#define CUSTOMER_MENU_HPP_

#include <vector>
#include <food.hpp>

class Menu {
public:
    Menu(FoodContainer& all_foods)
    {
        for (auto i : all_foods.foods) {
            types_foods_[i.first.type].push_back(i.second);
        }
    };
    void get_menu();
    void refresh_menu();
    void print_menu();
private:
    vector<vector<Food>> types_foods_;
};

#endif