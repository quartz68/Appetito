#include <food.hpp>

// Price class

double Price::get_cny() {
    // Apply the discount to the absolute price and return it.
    return abs_price_ * (1 - discount_);
}

double Price::get_usd() {
    // Convert the absolute price in CNY to USD and apply the discount to it.
    // Assume that the conversion rate is 1 USD = 7.0 CNY.
    return (abs_price_ * (1 - discount_)) / 7.0;
}

void Price::set_price(double price) {
    abs_price_ = price;
}

void Price::set_discount(double percentage) {
    discount_ = percentage;
}

// Food class



string Food::get_name() {
    return name_;
}

double Food::get_price_cny() {
    return price_.get_cny();
}

double Food::get_price_usd() {
    return price_.get_usd();
}

string Food::get_type_name() {
    return type_.name;
}

string Food::get_id_str() {
    // Convert the FoodID to a string of the form "type_id-id".
    return to_string(food_id_.type) + "-" + to_string(food_id_.id);
}

void Food::set_name(string name) {
    name_ = name;
}

void Food::set_price(double price) {
    price_.set_price(price);
}

void Food::set_type(unsigned short type_id) {
    type_ = container_.types[type_id];
}
