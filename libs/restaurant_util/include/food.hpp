#ifndef FOOD_HPP_
#define FOOD_HPP_

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <cereal/types/string.hpp>
using namespace std;

class Food;

struct FoodID {
    bool operator< (const FoodID& other) const {
        if (type < other.type) return true;
        else if (type > other.type) return false;
        else return id < other.id;
    }
    friend istream& operator>>(istream& is, FoodID& id) {
        string comma;
        is >> id.type;
        is >> comma;
        is >> id.id;
        return is;
    }
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
    Price() 
        :abs_price_(0), discount_(0) { }
    Price(double price)
        :abs_price_(price) { }
    double get_cny();
    double get_usd();
    void set_price(double price);
    void set_discount(double percentage);

    friend istream& operator>>(istream& is, Price& price) {
        is >> price.abs_price_;
        return is;
    }
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
    bool operator< (const FoodType& other) const {
        return id < other.id;
    }
    friend istream& operator>>(istream& is, FoodType& foodtype) {
        string comma;
        is >> foodtype.name;
        is >> comma;
        is >> foodtype.id;
        return is;
    }
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(name,id);
    }

    string name;
    unsigned short id;
};

const FoodType NULL_FOOD_TYPE = FoodType{"",0};


class Food {
public:
    Food()
        :type_(NULL_FOOD_TYPE) { }
    Food(string name, double price, FoodType type, FoodID id)
        :name_(name), type_(type), price_(price), food_id_(id) { }
    string get_name();
    double get_price_cny();
    double get_price_usd();
    string get_type_name();
    FoodID get_id() { return food_id_; }
    FoodType& get_type() { return type_; }
    string get_id_str();
    void set_name(string name);
    void set_price(Price price);
    void set_type(FoodType type);

    friend istream& operator>>(istream& is, Food& food) {
        string comma;
        is >> food.food_id_;
        is >> comma;
        is >> food.name_;
        is >> comma;
        is >> food.type_;
        is >> comma;
        is >> food.price_;
        return is;
    }
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(food_id_,name_,type_,price_);
    }
private:
    FoodID food_id_;
    string name_;
    FoodType type_;
    Price price_;
};

struct FoodContainer {
public:
    FoodContainer (ifstream& file) {
        Food tmp;
        while (file >> tmp) {
            types.emplace(tmp.get_type().id, tmp.get_type());
            foods.emplace(tmp.get_id(),tmp);
        }
    }
    map<unsigned short, FoodType> types;
    map<FoodID, Food> foods;
};


#endif