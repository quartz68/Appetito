#ifndef CUSTOMER_NOTIF_HPP_
#define CUSTOMER_NOTIF_HPP_

#include <array>
#include <string>
#include <protocol.hpp>
using namespace std;

class Notif {
public:
    Notif(std::string& notif);
    void print_notif();
private:
    string message;
};

#endif