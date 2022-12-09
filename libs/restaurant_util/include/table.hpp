#ifndef TABLE_HPP_
#define TABLE_HPP_

#include <string>
#include <unordered_map>
#include <cereal/types/string.hpp>
using namespace std;

class Table;

struct TableID {
    unsigned short type;
    unsigned short id;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(type,id);
    }
};


struct TableType {
    TableType()
        :name(0), capacity(0), id(0) { }
    TableType(string name, unsigned short capacity, unsigned short id)
        :name(name), capacity(capacity), id(id) { }
    string name;
    unsigned short capacity;
    unsigned short id;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(name,capacity,id);
    }
};

struct TableLocation {
    unsigned short row;
    unsigned short col;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(row,col);
    }
};

struct TableContainer {
    unordered_map<unsigned short, TableType> types;
    unordered_map<TableID, Table> tables;
    unordered_map<TableLocation, TableID> locations;
};

class Table {
public:
    Table(TableContainer& container, unsigned short type_id, unsigned short id, TableLocation location)
        :container_(container), type_(container_.types[type_id]), table_id_{type_.id, id}, location_(location) { }
    string get_type_name();
    string get_id_str();
    unsigned short get_capacity();
    void set_type(unsigned short type_id);
    TableLocation get_location();
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(type_,table_id_,location_);
    }
private:
    TableType& type_;
    TableID table_id_;
    TableLocation location_;
    TableContainer& container_;
};




#endif