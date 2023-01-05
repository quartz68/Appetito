/**
 * @file protocol.hpp
 * @brief Defines some console output.
 * @details
 * @version
 */

#ifndef PROTOCOL_HPP_
#define PROTOCOL_HPP_

#include <iomanip>

#define SMALL_FIELD_WIDTH 10
#define LARGE_FIELD_WIDTH 20

inline std::ostream& small_field(std::ostream& o)
{
    return o << std::setw(SMALL_FIELD_WIDTH) << std::left;
}

inline std::ostream& large_field(std::ostream& o)
{
    return o << std::setw(LARGE_FIELD_WIDTH) << std::left;
}

// General Formatting
#define BOTTOM               "----------------------------------------\n"
#define DIVIDE               "----------------------------------------"
// Customer Client Formatting
#define SELECT_TABLE_HEADER  "\n------------Select---Table--------------"
#define SELECT_FOOD_HEADER   "\n------------Select---Food---------------"
#define PREVIEW_ORDER_HEADER "\n------------Preview---Order-------------"
#define DEAL_NUMBER_HEADER   "\n-------------Deal---Number--------------"
#define NOTIFICATION_HEADER  "\n-------------NOTIFICATION---------------"
#define DEAL_HEADER          "\n-----------------Deal-------------------"
#define CURRENT_DEAL_HEADER  "\n------------Current---Deal--------------"
// Kitchen Client Formatting
#define NEW_ITEM_HEADER      "\n--------------New---Item----------------"

#endif