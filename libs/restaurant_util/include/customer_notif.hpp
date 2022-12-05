#ifndef CUSTOMER_NOTIF_HPP_
#define CUSTOMER_NOTIF_HPP_

#include <array>
#include <string>
#include <protocol.hpp>
using namespace std;
using Pack = array<char, MAX_PACK_SIZE>;

class Notif {
public:
    Notif(Pack pack);
    //void print_notif();
private:
    string message;
};

#endif