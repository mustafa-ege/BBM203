#include "PhysicalLayerPacket.h"

PhysicalLayerPacket::PhysicalLayerPacket(int _layer_ID, const string& _sender_MAC, const string& _receiver_MAC)
        : Packet(_layer_ID) {
    sender_MAC_address = _sender_MAC;
    receiver_MAC_address = _receiver_MAC;
    cout << "Sender MAC address: " << sender_MAC_address << ", Receiver MAC address: " << receiver_MAC_address << endl;
}


void PhysicalLayerPacket::print() {
    // TODO: Override the virtual print function from Packet class to additionally print layer-specific properties.
    cout << "Sender MAC address: " << sender_MAC_address << ", Receiver MAC address: " << receiver_MAC_address << endl;
}
void PhysicalLayerPacket::print_with_layer_info() {
    // TODO: Override the virtual print function from Packet class to additionally print layer-specific properties.
    cout << "Layer 3 info: ";
    print();
    cout << "Number of hops so far: " << number_of_hops << endl;
}

PhysicalLayerPacket::~PhysicalLayerPacket() {
    // TODO: Free any dynamically allocated memory if necessary.
}