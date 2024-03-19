#ifndef PACKET_H
#define PACKET_H

#include <string>
#include <iostream>

using namespace std;

class Packet {
public:
    Packet(int layerID);
    virtual ~Packet();

    int layer_ID;

    int number_of_hops = 0 ;

    friend ostream &operator<<(ostream &os, const Packet &packet);
    virtual void print() {};
    virtual void print_with_layer_info() {};
};

#endif // PACKET_H
