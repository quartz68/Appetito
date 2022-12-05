#ifndef TABLE_HPP_
#define TABLE_HPP_

#include <string>
#include <unordered_map>
using namespace std;

struct TableID {
    unsigned short type : 16;
    unsigned short id : 16;
};

struct TableType {
    TableType(string name, unsigned short capacity, unsigned short id)
        :name(name), capacity(capacity), id(id) { }
    string name;
    unsigned short capacity;
    unsigned short id;
};

class Table {
public:
    Table(unsigned short type_id, unsigned short id);
    string get_type_name();
    string get_id_str();
    unsigned short get_capacity();
    void set_type(unsigned short type_id);
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