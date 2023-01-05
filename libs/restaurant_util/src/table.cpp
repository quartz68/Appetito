/**
 * @file table.cpp
 * @brief Implementation of Table, TableID, TableType, TableLocation and TableContainer classes.
 * @details
 * @version
 */

#include <table.hpp>

string Table::get_type_name() {
    return type_.name;
}

string Table::get_id_str() {
    return to_string(table_id_.type) + "-" + to_string(table_id_.id);
}

string Table::get_location_str() {
    return "(" + to_string(location_.col) + "," + to_string(location_.row) + ")";
}

unsigned short Table::get_capacity() {
    return type_.capacity;
}

void Table::set_type(TableType& type) {
    type_ = type;
}

TableLocation Table::get_location() {
    return location_;
}
