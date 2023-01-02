#ifndef CUSTOMER_ORDER_DEAL_HPP_
#define CUSTOMER_ORDER_DEAL_HPP_

#include <vector>
#include <food.hpp>
#include <table.hpp>
#include <menu.hpp>
#include <cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/vector.hpp>
using namespace std;

/**
 * @brief Order.
 */
class Order {
public:
    /**
     * Construct a new Order object from menu.
     * @brief Constructor.
     * 
     * @param menu Menu.
     */
    Order(Menu& menu) 
        :menu_ptr_(&menu) { }
    /**
     * @brief Copy constructor.
     */
    Order(const Order& order)
        :food_list_(order.food_list_), bill_(order.bill_), menu_ptr_(order.menu_ptr_) { }
    void operator=(const Order& order)
    {
        food_list_ = order.food_list_;
        bill_ = order.bill_;
        menu_ptr_ = order.menu_ptr_;
    }
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
     * @brief Set the menu.
     * 
     * @param menu_ptr Raw pointer to menu.
     */
    void set_menu_ptr(Menu* menu_ptr) { if (menu_ptr != nullptr) menu_ptr_ = menu_ptr; else cerr << "Trying to set nullptr as menu pointer!" << endl; }
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
    Menu* menu_ptr_;
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
        :order_(order), tableid_(table.get_id()), bill_(order.get_bill()) { }
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
    void set_table(Table& table) { tableid_ = table.get_id(); }
    /**
     * @brief Set the menu.
     * 
     * @param menu_ptr Raw pointer to menu.
     */
    void set_menu_ptr(Menu* menu_ptr) { if (menu_ptr != nullptr) order_.set_menu_ptr(menu_ptr); else cerr << "Trying to set nullptr as menu pointer!" << endl; }
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