#ifndef TABLE_HPP_
#define TABLE_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cereal/types/string.hpp>
using namespace std;

class Table;

struct TableID {
    bool operator< (const TableID& other) const {
        if (type < other.type) return true;
        else if (type > other.type) return false;
        else return id < other.id;
    }
    friend istream& operator>>(istream& is, TableID& id) {
        string comma;
        is >> id.type;
        is >> comma;
        is >> id.id;
        return is;
    }
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(type,id);
    }
    unsigned short type;
    unsigned short id;
};


struct TableType {
    TableType()
        :name(" "), capacity(0), id(0) { }
    TableType(string name, unsigned short capacity, unsigned short id)
        :name(name), capacity(capacity), id(id) { }
    
    bool operator< (const TableType& other) const {
        return id < other.id;
    }
    friend istream& operator>>(istream& is, TableType& tabletype) {
        string comma;
        is >> tabletype.name;
        is >> comma;
        is >> tabletype.capacity;
        is >> comma;
        is >> tabletype.id;
        return is;
    }
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(name,capacity,id);
    }

    string name;
    unsigned short capacity;
    unsigned short id;
};

const TableType NULL_TABLE_TYPE;

struct TableLocation {

    bool operator< (const TableLocation& other) const {
        if (col < other.col) return true;
        else if (col > other.col) return false;
        else return row < other.row;
    }
    friend istream& operator>>(istream& is, TableLocation& location) {
        char comma;
        is >> location.col;
        is >> comma;
        is >> location.row;
        return is;
    }
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(col,row);
    }
    
    unsigned short col;
    unsigned short row;
};



class Table {
public:
    Table()
        :type_(NULL_TABLE_TYPE) { }
    Table(TableType type, TableID id, TableLocation location)
        :type_(type), table_id_(id), location_(location) { }
    string get_type_name();
    string get_id_str();
    TableID get_id() { return table_id_; }
    TableType& get_type() { return type_; }
    unsigned short get_capacity();
    void set_type(TableType& type_id);
    TableLocation get_location();

    friend istream& operator>>(istream& is, Table& table) {
        string comma;
        is >> table.table_id_;
        is >> comma;
        is >> table.type_;
        is >> comma;
        is >> table.location_;
        return is;
    }
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(type_,table_id_,location_);
    }
private:
    TableID table_id_;
    TableType type_;
    TableLocation location_;
};


struct TableContainer {
    TableContainer (ifstream& file) {
        Table tmp;
        while (file >> tmp) {
            types.emplace(tmp.get_type().id, tmp.get_type());
            tables.emplace(tmp.get_id(),tmp);
            locations.emplace(tmp.get_location(),tmp.get_id());
        }
    }
    map<unsigned short, TableType> types;
    map<TableID, Table> tables;
    map<TableLocation, TableID> locations;
};

#endif