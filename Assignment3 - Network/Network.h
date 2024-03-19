#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <iostream>
#include "Packet.h"
#include "Client.h"


using namespace std;

class Network {
public:
    Network();
    ~Network();
    stack<Packet*> dynamic_packets;


    int number_of_clients = 0;
    unordered_map<string, vector<Log>> logs;

    void message_creator(vector<Client> &clients, string& sender_ID, string& receiver_ID, string& message,
                         const string &sender_port, const string &receiver_port, int& message_limit);
    vector<string> slice_message(const string& message, size_t max_length);

    void show_frame_info(Client& client, string& in_or_out, string& frame_number);
    void show_q_info(Client& client, string& in_or_out);

    void send(vector<Client> &clients);
    void receive(vector<Client> &clients);
    void print_log(vector<Client> &clients, string& log_id);


    stack<Packet*> stack_returner(queue<stack<Packet*>> original_queue, int no);
    Packet* packet_returner(stack<Packet*> original_stack, int no);
    Client& client_returner(vector<Client>& clients, string id);
    Client& client_returner_by_MAC_adress(vector<Client>& clients, string mac_adress);
    void frame_printer(stack<Packet*> frame);


    // Executes commands given as a vector of strings while utilizing the remaining arguments.
    void process_commands(vector<Client> &clients, vector<string> &commands, int message_limit, const string &sender_port,
                     const string &receiver_port);

    // Initialize the network from the input files.
    vector<Client> read_clients(string const &filename);
    void read_routing_tables(vector<Client> & clients, string const &filename);
    vector<string> read_commands(const string &filename);
};

#endif  // NETWORK_H
