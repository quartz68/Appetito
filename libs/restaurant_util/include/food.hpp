#ifndef FOOD_HPP_
#define FOOD_HPP_

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <cereal/types/string.hpp>
#include <cereal/types/map.hpp>
using namespace std;

class Food;

/**
 * @brief ID of food.
 */
struct FoodID {
    bool operator== (const FoodID& other) const {
        return (type == other.type) && (id == other.id);
    }
    bool operator< (const FoodID& other) const {
        if (type < other.type) return true;
        else if (type > other.type) return false;
        else return id < other.id;
    }
    /**
     * @brief Overloading operator>> : typeid , id
     */
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

/**
 * @brief Price of food.
 */
class Price {
public:
    /**
     * Construct a new Price object with abs_price_ and discount_ set to 0.
     * @brief Default constructor.
     * 
     * @see Price(double price)
     */
    Price() 
        :abs_price_(0), discount_(0) { }
    /**
     * Construct a new Price object from a price.
     * @brief Constructor.
     * 
     * @param price The absolute price.
     * 
     * @see Price()
     */
    Price(double price)
        :abs_price_(price) { }
    /**
     * @brief Get the price in CNY.
     * 
     * @return The price in CNY.
     */
    double get_cny();
    /**
     * @brief Get the price in USD.
     * 
     * @return The price in USD.
     */
    double get_usd();
    /**
     * @brief Set the price.
     * 
     * @param price The absolute price to set to.
     */
    void set_price(double price);
    /**
     * @brief Set the discount.
     * 
     * @param percentage The discount (0~1).
     */
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

/**
 * @brief Type of food.
 */
struct FoodType {
    bool operator< (const FoodType& other) const {
        return id < other.id;
    }
    /**
     * @brief Overloading operator>> : name , id
     */
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

/**
 * @brief Food.
 */
class Food {
public:
    /**
     * Construct a Food object of NULL_FOOD_TYPE.
     * @brief Default constructor.
     * 
     * @see Food(string name, double price, FoodType type, FoodID id)
     */
    Food()
        :type_(NULL_FOOD_TYPE) { }
    /**
     * Construct a Food object from name, price, type and ID.
     * @brief Constructor.
     * 
     * @param name Name of the food.
     * @param price Price of the food.
     * @param type Type of the food.
     * @param id ID of the food.
     * 
     * @see Food()
     */
    Food(string name, double price, FoodType type, FoodID id)
        :name_(name), type_(type), price_(price), food_id_(id) { }
    /**
     * @brief Get the name of the food.
     * 
     * @return Name of the food.
     */
    string get_name();
    /**
     * @brief Get the price in CNY.
     * 
     * @return The price in CNY.
     */
    double get_price_cny();
    /**
     * @brief Get the price in USD.
     * 
     * @return The price in USD.
     */
    double get_price_usd();
    /**
     * @brief Get the name of the type of the food.
     * 
     * @return Name of the type of the food.
     */
    string get_type_name();
    /**
     * @brief Get the ID of the food.
     * 
     * @return ID of the food.
     */
    FoodID get_id() { return food_id_; }
    /**
     * @brief Get the type of the food.
     * 
     * @return Reference to the type of the food.
     */
    FoodType& get_type() { return type_; }
    /**
     * @brief Get the ID in string.
     * 
     * @return ID (typeid-id)
     */
    string get_id_str();
    /**
     * @brief Set the name of the food.
     * 
     * @param name Name of the food.
     */
    void set_name(string name);
    /**
     * @brief Set the price of the food.
     * 
     * @param price Price of the food.
     */
    void set_price(Price price);
    /**
     * @brief Set the type of the food.
     * 
     * @param type Type of the food.
     */
    void set_type(FoodType type);
    /**
     * @brief Overloading operator>> : FoodID , name , FoodType , Price
     */
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

/**
 * @brief Container that holds FoodType and Food in two maps.
 */
struct FoodContainer {
public:
    /**
     * @brief Default constructor.
     */
    FoodContainer()
        :types(), foods() { }
    /**
     * Construct a FoodContainer object from a input file stream.
     * @brief Constructor.
     * 
     * @param file Input file stream. Format: typeid , id , name , typename , typeid , price\n
     */
    FoodContainer (ifstream& file) {
        Food tmp;
        while (file >> tmp) {
            types.emplace(tmp.get_type().id, tmp.get_type());
            foods.emplace(tmp.get_id(),tmp);
        }
    }
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(types,foods);
    }
    map<unsigned short, FoodType> types;
    map<FoodID, Food> foods;
};


#endif