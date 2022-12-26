#include <menu.hpp>

void Menu::print()
{
    
    set<FoodType> food_types;
    for (auto food : foods_) {
        food_types.insert(food.second.get_type());
    }
    set<TableType> table_types;
    for (auto table : tables_) {
        table_types.insert(table.second.get_type());
    }
    cout << "Foods:" << endl;
    cout << "ID\tName\tPrice(CNY)" << endl;
    for (auto type : food_types) {
        cout << type.name << endl;
        for (auto food : foods_) {
            if (food.second.get_type().id == type.id)
                cout << food.second.get_id_str() << '\t' << food.second.get_name() << '\t' << food.second.get_price_cny() << endl;
        }
    }
    cout << "Tables:" << endl;
    cout << "ID\tPrice(CNY)\tLocation" << endl;
    for (auto type : table_types) {
        cout << type.name << endl;
        for (auto table : tables_) {
            if (table.second.get_type().id == type.id)
                cout << table.second.get_id_str() << '\t' << table.second.get_capacity() << "\t(" << table.second.get_location().col << ',' << table.second.get_location().row << ')' << endl;
        }
    }
}

void Menu::refresh()
{
    //TO-DO
}