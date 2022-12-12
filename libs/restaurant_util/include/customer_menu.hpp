#ifndef CUSTOMER_MENU_HPP_
#define CUSTOMER_MENU_HPP_

#include <vector>
#include <regex>
#include <food.hpp>
#include <table.hpp>
#include <protocol.hpp>
#include <cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
using namespace std;

class Menu {
public:
    Menu(FoodContainer& all_foods, TableContainer& all_tables) //构造函数，遍历FoodContainer和TableContainer，分别将Food和Table存入二维vector types_foods和types_tables
    : all_foods_(all_foods), all_tables_(all_tables)
    {
        for (auto food : all_foods.foods) {
            types_foods_[food.first.type].push_back(food.second);
        }
        for (auto table : all_tables.tables) {
            types_tables_[table.first.type].push_back(table.second);
        }
    };
    void print(); //从二维vector types_foods和types_tables打印菜单
    void refresh(); //刷新菜单，操作基本上和构造函数相同，向服务器请求的部分我后面来加
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(types_foods_,types_tables_);
    }
protected:
    vector<vector<Food>> types_foods_; // 二维vector，一个类型的Food是一行
    vector<vector<Table>> types_tables_; // 参考上一个
    FoodContainer& all_foods_;
    TableContainer& all_tables_;
};

#endif