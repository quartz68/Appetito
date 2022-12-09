#ifndef FOOD_HPP_
#define FOOD_HPP_

#include <string>
#include <unordered_map>
#include <cereal/types/string.hpp>
using namespace std;

class Food;

struct FoodID {
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(type,id);
    }
    unsigned short type;
    unsigned short id;
};

class Price {
public:
    Price(double price)
        :abs_price_(price) { }
    double get_cny();
    double get_usd();
    void set_price(double price);
    void set_discount(double percentage);
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(abs_price_,discount_);
    }
private:
    double abs_price_;
    double discount_;
};

struct FoodType {
    string name;
    unsigned short id;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(name,id);
    }
};

struct FoodContainer {
public:
    unordered_map<unsigned short, FoodType> types;
    unordered_map<FoodID, Food> foods;
};

class Food {
public:
    Food(string name, double price, unsigned short type_id, unsigned short id, FoodContainer& container)
        :container_(container), name_(name), type_(container_.types[type_id]), price_(price), food_id_{type_id, id} { }
    string get_name();
    double get_price_cny();
    double get_price_usd();
    string get_type_name();
    string get_id_str();
    void set_name(string name);
    void set_price(double price);
    void set_type(unsigned short type_id);

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(name_,type_,price_,food_id_);
    }
private:
    string name_;
    FoodType& type_;
    Price price_;
    FoodID food_id_;
    FoodContainer& container_;
};



#endif