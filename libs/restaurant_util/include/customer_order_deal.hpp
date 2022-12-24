#ifndef CUSTOMER_ORDER_DEAL_HPP_
#define CUSTOMER_ORDER_DEAL_HPP_

#include <vector>
#include <food.hpp>
#include <cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
using namespace std;

/**
 * @brief Order.
 */
class Order {
public:
    /**
     * @brief Add an item of Food to the order.
     * 
     * @param item Item of Food.
     */
    void add_item(Food item);
    /**
     * @brief Delete an item of Food to the order.
     * 
     * @param item Item of Food.
     */
    void delete_item(Food item);
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
    vector<Food> food_list_;
    double bill_;
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
        :order_(order), table_(table), bill_(order.get_bill()) { }
    /**
     * @brief Calculate the bill and return the total price.
     * 
     * @return Total price.
     */
    void get_bill();
    /**
     * @brief Set the order.
     * 
     * @param order Order.
     */
    void set_order(Order& order);
    /**
     * @brief Set the table.
     * 
     * @param table Table.
     */
    void set_table(Table& table);
    /**
     * @brief Print the deal.
     */
    void print();
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(order_,table_,bill_);
    }
private:
    Order order_;
    Table table_;
    double bill_;
};

#endif