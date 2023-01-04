/**
 * @file order_deal.cpp
 * @brief Implementation of Order and Deal classes.
 * @details
 * @version
 */

#include <order_deal.hpp>

void Order::add_item(Food item, unsigned short quantity) {
    // Find if there is FoodID in food_list_ equal to item.get_id().
    FoodID itemid = item.get_id();
    auto it = std::find_if(food_list_.begin(), food_list_.end(), 
                        [itemid = std::as_const(itemid)](const pair<FoodID, unsigned short>& p)
                        { return p.first == itemid; });
    // Add the quantity if there is; emplace if it isn't.
    if (it != food_list_.end()) {
        (*it).second += quantity;
    } else {
        food_list_.emplace_back(item.get_id(), quantity);
    }
}

void Order::delete_item(Food item, unsigned short quantity) {
    // Find if there is FoodID in food_list_ equal to item.get_id().
    FoodID itemid = item.get_id();
    auto it = std::find_if(food_list_.begin(), food_list_.end(), 
                        [itemid = std::as_const(itemid)](const pair<FoodID, unsigned short>& p)
                        { return p.first == itemid; });
    // Subtract the quantity if there is and quantity on the list is larger than that we would like to delete.
    if (it != food_list_.end()) {
        if ((*it).second >= quantity) (*it).second -= quantity;
        else cerr << "The quantity of items you want to delete is too large." << endl;
    } else {
        cerr << "The items you want to delete is not in the list." << endl;
    }
}

double Order::get_bill() {
    bill_ = 0;
    for (auto it : food_list_) {
        bill_ += (*menu_ptr_).foods_[it.first].get_price_cny() * it.second;
    }
    return bill_;
}

string Order::get_price_str(FoodID foodid, unsigned short quantity) {
    return to_string(menu_ptr_->foods_[foodid].get_price_cny()) + " * " + to_string(quantity) + " = " + to_string(menu_ptr_->foods_[foodid].get_price_cny() * quantity);
}

void Order::print() {
    // Organize all FoodTypes appeared in food_list_ for printing
    set<FoodType> food_types;
    for (auto food : food_list_) {
        food_types.insert((*menu_ptr_).foods_[food.first].get_type());
    }
    // Print the order
    cout << "Foods:" << endl;
    cout << small_field << "ID" << large_field << "Name" << large_field << "Price(CNY)" << endl;
    for (auto type : food_types) {
        cout << type.name << endl;
        for (auto food : food_list_) {
            if (food.first.type == type.id)
                cout << small_field << (*menu_ptr_).foods_[food.first].get_id_str() << large_field << (*menu_ptr_).foods_[food.first].get_name() << large_field << get_price_str(food.first,food.second) << endl;
        }
    }
}

double Deal::get_bill() {
    bill_ = order_.get_bill();
    return bill_;
}

void Deal::print() {
    order_.print();
    cout << DIVIDE << endl;
    cout << "Total: " << bill_ << endl;
    cout << "Table: " << tableid_.type << '-' << tableid_.id << endl;
}