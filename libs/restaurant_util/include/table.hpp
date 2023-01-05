/**
 * @file table.hpp
 * @brief Header of Table, TableID, TableType, TableLocation and TableContainer classes.
 * @details
 * @version
 */

#ifndef TABLE_HPP_
#define TABLE_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cereal/types/string.hpp>
#include <cereal/types/map.hpp>
using namespace std;

class Table;

/**
 * @brief ID of table.
 */
struct TableID {
    bool operator< (const TableID& other) const {
        if (type < other.type) return true;
        else if (type > other.type) return false;
        else return id < other.id;
    }
    /**
     * @brief Overloading operator>> : typeid , id
     */
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

    //
    unsigned short type;
    unsigned short id;
};

/**
 * @brief Type of table.
 */
struct TableType {
    /**
     * Construct a TableType object with name of empty string, capacity and ID set to 0.
     * @brief Default constructor.
     * 
     * @see TableType(string name, unsigned short capacity, unsigned short id)
     */
    TableType()
        :name(" "), capacity(0), id(0) { }
    /**
     * Construct a TableType object from name, capacity and ID.
     * @brief Constructor.
     * 
     * @param name Name of the type.
     * @param capacity Capacity of the type.
     * @param id ID of the type.
     * 
     * @see TableType()
     */
    TableType(string name, unsigned short capacity, unsigned short id)
        :name(name), capacity(capacity), id(id) { }
    
    bool operator< (const TableType& other) const {
        return id < other.id;
    }
    /**
     * @brief Overloading operator>> : name , capacity , id
     */
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

    //
    string name;
    unsigned short capacity;
    unsigned short id;
};

const TableType NULL_TABLE_TYPE;

/**
 * @brief Location of table.
 */
struct TableLocation {
    bool operator< (const TableLocation& other) const {
        if (col < other.col) return true;
        else if (col > other.col) return false;
        else return row < other.row;
    }
    /**
     * @brief Overloading operator>> : column , row
     */
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
    
    //
    unsigned short col;
    unsigned short row;
};

/**
 * @brief Table.
 */
class Table {
public:
    /**
     * Construct a Table object of NULL_TABLE_TYPE.
     * @brief Default constructor.
     * 
     * @see Table(TableType type, TableID id, TableLocation location)
     */
    Table()
        :type_(NULL_TABLE_TYPE) { }
    /**
     * Construct a Table object from type, id and location.
     * @brief Constructor.
     * 
     * @param type Type of the table.
     * @param id ID of the table.
     * @param location Location of the table.
     */
    Table(TableType type, TableID id, TableLocation location)
        :type_(type), table_id_(id), location_(location) { }
    /**
     * @brief Get the name of the type of the table.
     * @return The name of the type of the table.
     */
    string get_type_name();
    /**
     * @brief Get the ID in string.
     * 
     * @return ID "typeid-id"
     */
    string get_id_str();
    /**
     * @brief Get the location in string.
     * 
     * @return Location "(column,row)"
     */
    string get_location_str();
    /**
     * @brief Get the ID of the table.
     * 
     * @return ID of the table.
     */
    TableID get_id() { return table_id_; }
    /**
     * @brief Get the type of the table.
     * 
     * @return Reference to the type of the table.
     */
    TableType& get_type() { return type_; }
    /**
     * @brief Get the capacity of the table.
     * 
     * @return Capacity of the table.
     */
    unsigned short get_capacity();
    /**
     * @brief Set the type of the table.
     * 
     * @param type Type of the table.
     */
    void set_type(TableType& type_id);
    /**
     * @brief Get the location of the table.
     * 
     * @return Location of the table.
     */
    TableLocation get_location();
    /**
     * @brief Overloading operator>> : TableID , TableType , TableLocation
     */
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

/**
 * @brief Container that holds TableType, Table and Location in three maps.
 */
struct TableContainer {
    /**
     * @brief Default constructor.
     */
    TableContainer()
        :types(), tables(), locations() { }
    /**
     * Construct a TableContainer object from a input file stream.
     * @brief Constructor.
     * 
     * @param file Input file stream. Format: typeid , id , typename , capacity , typeid , location_col , location_row\n
     */
    TableContainer (ifstream& file) {
        Table tmp;
        while (file >> tmp) {
            types.emplace(tmp.get_type().id, tmp.get_type());
            tables.emplace(tmp.get_id(),tmp);
            locations.emplace(tmp.get_location(),tmp.get_id());
        }
    }
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(types,tables,locations);
    }
    map<unsigned short, TableType> types;
    map<TableID, Table> tables;
    map<TableLocation, TableID> locations;
};

#endif