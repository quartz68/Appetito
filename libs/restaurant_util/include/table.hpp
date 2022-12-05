#ifndef TABLE_HPP_
#define TABLE_HPP_

#include <string>
#include <unordered_map>
using namespace std;

struct TableID { //桌子的id
    unsigned short type : 16; //桌子类型的id，也就是TableType::id
    unsigned short id : 16; //桌子自己在这个类型的id，两个接起来就是这个桌子独有的id
};

//桌子类型
struct TableType {
    TableType(string name, unsigned short capacity, unsigned short id)
        :name(name), capacity(capacity), id(id) { }
    string name;//桌子类型名
    unsigned short capacity;//桌子能坐人数
    unsigned short id;//桌子类型的id
};

class Table {
public:
    Table(unsigned short type_id, unsigned short id); //构造函数
    string get_type_name(); //返回桌子类型名
    string get_id_str(); //返回桌子id字符串
    unsigned short get_capacity(); // 返回桌子能坐的人数
    void set_type(unsigned short type_id); // 设置桌子类型
private:
    TableType& type_;
    TableID table_id_;
};

class TableContainer {
protected:
    unordered_map<unsigned short, TableType> types;
    unordered_map<TableID, Table> tables;
};

#endif