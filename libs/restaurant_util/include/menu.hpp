/**
 * @file menu.hpp
 * @brief Header of Menu class.
 * @details
 * @version
 */

#ifndef CUSTOMER_MENU_HPP_
#define CUSTOMER_MENU_HPP_

#include <unordered_map>
#include <regex>
#include <food.hpp>
#include <table.hpp>
#include <protocol.hpp>
#include <set>
#include <cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
using namespace std;

class Menu {
public:
    Menu() { }
    Menu(FoodContainer* all_foods, TableContainer* all_tables) //构造函数，遍历FoodContainer和TableContainer，分别将Food和Table存入二维vector types_foods和types_tables
    {
        if (all_foods != nullptr && all_tables != nullptr) {
            for (auto food : (*all_foods).foods) {
                foods_.emplace(food.first,food.second);
            }
            for (auto table : (*all_tables).tables) {
                tables_.emplace(table.first,table.second);
            }
        } else {
            cerr << "Trying to set nullptr as FoodContainer or TableContainer pointer!" << endl;
        }
    }
    Menu(const Menu& other)
        :foods_(other.foods_), tables_(other.tables_) { }
    void print_foods();
    void print_tables();
    void print(); //从二维vector types_foods和types_tables打印菜单
    void refresh(); //刷新菜单，操作基本上和构造函数相同，向服务器请求的部分我后面来加
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(foods_,tables_);
    }
    map<FoodID,Food> foods_; // 二维vector，一个类型的Food是一行
    map<TableID,Table> tables_; // 参考上一个
};

#endif