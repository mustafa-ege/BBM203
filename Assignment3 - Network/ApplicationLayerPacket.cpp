#include "ApplicationLayerPacket.h"

ApplicationLayerPacket::ApplicationLayerPacket(int _layer_ID, const string &_sender_ID, const string &_receiver_ID, const string& _message_data) : Packet(_layer_ID) {
    sender_ID = _sender_ID;
    receiver_ID = _receiver_ID;
    message_data = _message_data;
    cout << "Sender ID: " << sender_ID << ", Receiver ID: " << receiver_ID << endl;
    cout << "Message chunk carried: " << "\"" << message_data << "\"" << endl;
    cout << "Number of hops so far: " << number_of_hops<< endl;
    cout << "--------" << endl;
}

void ApplicationLayerPacket::print() {
    // TODO: Override the virtual print function from Packet class to additionally print layer-specific properties.

    cout << "Sender ID: " << sender_ID << ", Receiver ID: " << receiver_ID << endl;
    cout << "Message chunk carried: \"" << message_data << "\"" << endl;
    cout << "Number of hops so far: " << number_of_hops << endl;

}

void ApplicationLayerPacket::print_with_layer_info() {
    // TODO: Override the virtual print function from Packet class to additionally print layer-specific properties.
    cout << "Carried Message: \"" << message_data << "\"" << endl;
    cout << "Layer 0 info: ";
    cout << "Sender ID: " << sender_ID << ", Receiver ID: " << receiver_ID << endl;
}

ApplicationLayerPacket::~ApplicationLayerPacket() {
    // TODO: Free any dynamically allocated memory if necessary.
}
