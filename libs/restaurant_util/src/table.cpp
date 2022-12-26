#include <table.hpp>

string Table::get_type_name() {
    return type_.name;
}

string Table::get_id_str() {
    return to_string(table_id_.type) + "-" + to_string(table_id_.id);
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
