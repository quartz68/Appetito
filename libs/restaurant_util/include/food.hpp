#ifndef FOOD_HPP_
#define FOOD_HPP_

#include <string>
#include <unordered_map>
using namespace std;

struct FoodID {
    unsigned short type : 16;
    unsigned short id : 16;
};

class Price {
public:
    Price(double price)
        :abs_price_(price) { }
    double get_cny();
    double get_usd();
    void set_price(double price);
    void set_discount(double percentage);
private:
    double abs_price_; // in CNY
};

struct FoodType {
    FoodType(string name, unsigned short id)
        :name(name), id(id) { }
    string name;
    unsigned short id;
};

class Food {
public:
    Food(string name, double price, unsigned short type_id, unsigned short id);
    string get_name();
    double get_price_cny();
    double get_price_usd();
    string get_type_name();
    string get_id_str();
    void set_name(string name);
    void set_price(double price);
    void set_type(unsigned short type_id);
private:
    string name_;
    FoodType& type_;
    Price price_;
    FoodID food_id_;
};

class FoodContainer {
public:
    unordered_map<unsigned short, FoodType> types;
    unordered_map<FoodID, Food> foods;
};

#endif