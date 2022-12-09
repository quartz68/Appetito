#include <table.hpp>

string Table::get_type_name() {
    return type_.name;
}

string Table::get_id_str() {
    return to_string(table_id_.type) + "_" + to_string(table_id_.id);
}

unsigned short Table::get_capacity() {
    return type_.capacity;
}

void Table::set_type(unsigned short type_id) {
    type_ = container_.types[type_id];
}

TableLocation Table::get_location() {
    return location_;
}
