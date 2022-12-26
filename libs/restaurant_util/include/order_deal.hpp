#ifndef CUSTOMER_ORDER_DEAL_HPP_
#define CUSTOMER_ORDER_DEAL_HPP_

#include <vector>
#include <food.hpp>
#include <table.hpp>
#include <customer_menu.hpp>
#include <cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
using namespace std;

/**
 * @brief Order.
 */
class Order {
public:
    Order(Menu& menu) 
        :menu_(menu) { }
    /**
     * @brief Add items of Food to the order.
     * 
     * @param item Item of Food.
     */
    void add_item(Food item, unsigned short quantity);
    /**
     * @brief Delete items of Food to the order.
     * 
     * @param item Item of Food.
     */
    void delete_item(Food item, unsigned short quantity);
    /**
     * @brief Calculate the bill and return the total price.
     * 
     * @return Total price.
     */
    double get_bill();
    /**
     * @brief Print the order.
     */
    void print();
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(food_list_,bill_);
    }
private:
    vector<pair<FoodID, unsigned short>> food_list_;
    double bill_;
    Menu& menu_;
};

/**
 * @brief Deal.
 */
class Deal {
public:
    /**
     * Construct a Deal object from Order and Table.
     * @brief Constructor
     * 
     * @param order Order.
     * @param table Table.
     */
    Deal(Order& order, Table& table)
        :order_(order), table_(table.get_id()), bill_(order.get_bill()) { }
    /**
     * @brief Calculate the bill and return the total price.
     * 
     * @return Total price.
     */
    double get_bill();
    /**
     * @brief Set the order.
     * 
     * @param order Order.
     */
    void set_order(Order& order) { order_ = order; }
    /**
     * @brief Set the table.
     * 
     * @param table Table.
     */
    void set_table(Table& table) { tableid_ = table.get_id; }
    /**
     * @brief Print the deal.
     */
    void print();
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(order_,tableid_,bill_);
    }
private:
    Order order_;
    TableID tableid_;
    double bill_;
};

#endif