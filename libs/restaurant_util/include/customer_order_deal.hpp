#ifndef CUSTOMER_ORDER_DEAL_HPP_
#define CUSTOMER_ORDER_DEAL_HPP_

#include <map>
#include <food.hpp>
#include <table.hpp>
using namespace std;

class Order {
public:
    void add_item(Food item);
    void delete_item(Food item);
    double get_bill();
private:
    map<Food,unsigned short> food_list_;
    double bill;
};

class Deal {
public:
    Deal(Order& order, Table& table)
        :order_(order), table_(table), bill_(order.get_bill()) { }
    void get_bill();
    void set_order(Order& order);
    void set_table(Table& table);
    string get_deal_str();
private:
    Order& order_;
    Table& table_;
    double bill_;
};

#endif