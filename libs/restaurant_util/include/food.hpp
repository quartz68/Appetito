#ifndef FOOD_HPP_
#define FOOD_HPP_

#include <string>
#include <unordered_map>
using namespace std;

struct FoodID {
    unsigned short type : 16; // 菜品类型id
    unsigned short id : 16; // 菜品在这个类型的id，两个接起来就是这个菜品独有的id
};

class Price {
public:
    Price(double price)
        :abs_price_(price) { }
    double get_cny(); // 返回人民币价格
    double get_usd(); // 返回美元价格
    void set_price(double price); // 设置价格
    void set_discount(double percentage); // 设置打折
private:
    double abs_price_; // 人民币绝对值价格
    double discount_; // 折扣
};

struct FoodType {
    FoodType(string name, unsigned short id)
        :name(name), id(id) { }
    string name;
    unsigned short id;
};

class Food {
public:
    Food(string name, double price, unsigned short type_id, unsigned short id); //菜品构造函数
    string get_name(); // 返回菜品名
    double get_price_cny(); // 返回人民币价格
    double get_price_usd(); // 返回美元价格
    string get_type_name(); // 返回菜品类型名
    string get_id_str(); // 返回菜品ID字符串
    void set_name(string name); //设置菜品名称
    void set_price(double price); //设置菜品价格
    void set_type(unsigned short type_id);//设置菜品类型
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