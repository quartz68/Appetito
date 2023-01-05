/**
 * @file menu.cpp
 * @brief Implementation of Menu class.
 * @details
 * @version
 */

#include <menu.hpp>

void Menu::print_foods()
{
    set<FoodType> food_types;
    for (auto food : foods_) {
        food_types.insert(food.second.get_type());
    }
    cout << "Foods:" << endl;
    cout << small_field << "ID" << large_field << "Name" << small_field << "Price(CNY)" << endl;
    for (auto type : food_types) {
        cout << type.name << endl;
        for (auto food : foods_) {
            if (food.second.get_type().id == type.id)
                cout << small_field << food.second.get_id_str() << large_field << food.second.get_name() << small_field << food.second.get_price_cny() << endl;
        }
    }
}

void Menu::print_tables()
{
    set<TableType> table_types;
    for (auto table : tables_) {
        table_types.insert(table.second.get_type());
    }
    cout << "Tables:" << endl;
    cout << small_field << "ID" << large_field << "Price(CNY)" << small_field << "Location" << endl;
    for (auto type : table_types) {
        cout << type.name  << endl;
        for (auto table : tables_) {
            if (table.second.get_type().id == type.id)
                cout << small_field << table.second.get_id_str() << large_field << table.second.get_capacity() << small_field << table.second.get_location_str() << endl;
        }
    }
}

void Menu::print()
{
    print_foods();
    cout << DIVIDE << endl;
    print_tables();
}

void Menu::refresh()
{
    //TO-DO
}