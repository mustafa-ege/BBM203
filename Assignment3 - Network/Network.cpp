#include "Network.h"
#include <fstream>
#include <sstream>

Network::Network() {

}

void Network::process_commands(vector<Client> &clients, vector<string> &commands, int message_limit,
                      const string &sender_port, const string &receiver_port) {
    // TODO: Execute the commands given as a vector of strings while utilizing the remaining arguments.
    /* Don't use any static variables, assume this method will be called over and over during testing.
     Don't forget to update the necessary member variables after processing each command. For example,
     after the MESSAGE command, the outgoing queue of the sender must have the expected frames ready to send. */
    int number_of_commands = stoi(commands[0]);
    commands.erase(commands.begin());
    vector<vector<string>> all_commands_divided;
    for (string command_line_string : commands) {
        vector<string> command_line;
        string word;
        stringstream line(command_line_string);
        while (line >> word) {
            if (word[0] == '#'){
//                word.erase(0,1);
                command_line.push_back(word);
                while (line >> word && word[word.size()] != '#') {
                    command_line.back() += " " + word;
                }
            } else {
                command_line.push_back(word);
            }
        }
        all_commands_divided.push_back(command_line);
    }
    for (vector<string> command_line : all_commands_divided) {
        try {
            string all_line;
            for (string i: command_line) {
                all_line += i;
                all_line += " ";
            }
            all_line.pop_back();
            cout << string(all_line.size() + 9, '-') << endl << "Command: " << all_line << endl;
            cout << string(all_line.size() + 9, '-') << endl;
            if (command_line[0] == "MESSAGE") {
                string sender_ID = command_line[1];
                string receiver_ID = command_line[2];
                string message = command_line[3];
                message.erase(0, 1);
                message.pop_back();
                message_creator(clients, sender_ID, receiver_ID, message, sender_port, receiver_port, message_limit);
            } else if (command_line[0] == "SEND") {
                send(clients);
            } else if (command_line[0] == "RECEIVE") {
                receive(clients);
            } else if (command_line[0] == "SHOW_FRAME_INFO") {
                string client_id = command_line[1];
                string in_or_out = command_line[2];
                string frame_number = command_line[3];
                Client *client = &client_returner(clients, client_id);
                show_frame_info(*client, in_or_out, frame_number);
            } else if (command_line[0] == "SHOW_Q_INFO") {
                string client_id = command_line[1];
                string in_or_out = command_line[2];
                Client *client = &client_returner(clients, client_id);
                show_q_info(*client, in_or_out);
            } else if (command_line[0] == "PRINT_LOG") {
                print_log(clients, command_line[1]);
            } else {
                cout << "Invalid command." << endl;
            }
        } catch (runtime_error) {
            cout << "Invalid command." << endl;
        }
    }
}

void Network::print_log(vector<Client> &clients, string& log_id) {
    int log_entry = 0;
    Client client = client_returner(clients, log_id);
    for (Log log: client.log_entries){
        if (log_entry == 0)
            cout << "Client "<< log_id << " Logs:\n" << "--------------\n";
        cout << "Log Entry #" << ++log_entry << ":"<< endl;
        log.print();
        if (log_entry < client.log_entries.size())
            cout << "--------------" << endl;

    }
}

void Network::send(vector<Client> &clients) {
    for (Client& client: clients) {
        int number_of_frames_to_be_sent = client.outgoing_queue.size();
        for (int i = 0; i < number_of_frames_to_be_sent; i++) {
            stack<Packet*> frame_in_queue = stack_returner(client.outgoing_queue, 0);
            PhysicalLayerPacket* packet_of_frame = dynamic_cast<PhysicalLayerPacket *>(packet_returner(
                    frame_in_queue, 0));
            string receiver_mac = packet_of_frame->receiver_MAC_address;
            Client* receiver_client = &client_returner_by_MAC_adress(clients, receiver_mac);

            cout << "Client " << client.client_id << " sending frame #" << packet_of_frame->frame_number
                    << " to client " << receiver_client->client_id << endl;

            for (int i = 0; i <4; i++) {
                packet_returner(frame_in_queue, i)->number_of_hops ++;
            }
            receiver_client->incoming_queue.push(frame_in_queue);
            client.outgoing_queue.pop();
            frame_printer(frame_in_queue);
        }
    }
}
void Network::receive(vector<Client> &clients) {
    string message_received;
    for (Client& client: clients) {
        int number_of_frames_to_be_received = client.incoming_queue.size();
        for (int i = 0; i < number_of_frames_to_be_received; i++) {
            stack<Packet*> frame_in_queue = stack_returner(client.incoming_queue, 0);
            ApplicationLayerPacket* app_packet_of_frame = dynamic_cast<ApplicationLayerPacket *>(packet_returner(
                    frame_in_queue, 3));
            PhysicalLayerPacket* physical_packet_of_frame = dynamic_cast<PhysicalLayerPacket *>(packet_returner(
                    frame_in_queue, 0));
            string next_receiver_id = client.routing_table[app_packet_of_frame->receiver_ID];
            string from_client_id = client_returner_by_MAC_adress(clients, physical_packet_of_frame->sender_MAC_address).client_id;
            Client* next_client;
            //// transfer
            if (app_packet_of_frame->receiver_ID != client.client_id) {
                //// check if next receiver exist
                try {
                    next_client = &client_returner(clients, next_receiver_id);
                } catch (runtime_error e){
                    cout << "Client " << client.client_id << " receiving frame #" <<physical_packet_of_frame->frame_number << " from client " << from_client_id <<
                         ", but intended for client " <<  app_packet_of_frame->receiver_ID << ". Forwarding... " << endl;
                    cout << "Error: Unreachable destination. Packets are dropped after " << physical_packet_of_frame->number_of_hops << " hops!"<< endl;
                    client.incoming_queue.pop();
                    string msg = app_packet_of_frame->message_data;
                    const char* ending_chars = ".!?";
                    if (msg.length() >= 1) {
                        char lastChar = msg.back();
                        for (int i = 0; ending_chars[i] != '\0'; ++i) {
                            if (lastChar == ending_chars[i]){
                                time_t currentTime = time(nullptr);
                                char timeString[100];
                                strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));
                                Log log(timeString, message_received, physical_packet_of_frame->frame_number, app_packet_of_frame->number_of_hops,
                                        from_client_id, app_packet_of_frame->receiver_ID,false, ActivityType::MESSAGE_DROPPED);
                                client.log_entries.push_back(log);
                                std::cout << "--------" << std::endl;
                            }
                        }
                    }
                    continue;
                }
                if (physical_packet_of_frame->frame_number == 1)
                    cout << "Client " << client.client_id << " receiving a message from client " << from_client_id <<
                         ", but intended for client " <<  app_packet_of_frame->receiver_ID << ". Forwarding... " << endl;
                string new_MAC_adress = next_client->client_mac;
                physical_packet_of_frame->sender_MAC_address = client.client_mac;
                physical_packet_of_frame->receiver_MAC_address = new_MAC_adress;
                client.outgoing_queue.push(frame_in_queue);
                client.incoming_queue.pop();
                cout << "Frame #" << physical_packet_of_frame->frame_number << " MAC address change: New sender MAC "
                     << physical_packet_of_frame->sender_MAC_address << ", new receiver MAC " << physical_packet_of_frame->receiver_MAC_address<< "" << endl;
                string msg = app_packet_of_frame->message_data;
                const char* ending_chars = ".!?";
                if (msg.length() >= 1) {
                    char lastChar = msg.back();
                    for (int i = 0; ending_chars[i] != '\0'; ++i) {
                        if (lastChar == ending_chars[i]){
                            time_t currentTime = time(nullptr);
                            char timeString[100];
                            strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));
                            Log log(timeString, message_received,physical_packet_of_frame->frame_number,app_packet_of_frame->number_of_hops,
                                    app_packet_of_frame->sender_ID, app_packet_of_frame->receiver_ID,true, ActivityType::MESSAGE_FORWARDED);
                            client.log_entries.push_back(log);
                            std::cout << "--------" << std::endl;
                        }
                    }
                }
            } //// receive message
            else {
                cout << "Client " << client.client_id << " receiving frame #"<< physical_packet_of_frame->frame_number<<" from client "
                     << from_client_id << ", originating from client " << app_packet_of_frame->sender_ID << endl;
                if (physical_packet_of_frame->frame_number == 1)
                    message_received = "";
                frame_printer(frame_in_queue);
                message_received += app_packet_of_frame->message_data;
                client.incoming_queue.pop();
                string msg = app_packet_of_frame->message_data;
                const char* ending_chars = ".!?";
                if (msg.length() >= 1) {
                    char lastChar = msg.back();
                    for (int i = 0; ending_chars[i] != '\0'; ++i) {
                        if (lastChar == ending_chars[i]){
                            time_t currentTime = time(nullptr);
                            char timeString[100];
                            strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));
                            Log log(timeString, message_received,physical_packet_of_frame->frame_number,app_packet_of_frame->number_of_hops,
                                    app_packet_of_frame->sender_ID, app_packet_of_frame->receiver_ID,true, ActivityType::MESSAGE_RECEIVED);
                            client.log_entries.push_back(log);
                            std::cout << "Client "<< client.client_id <<" received the message \"" << message_received
                                      << "\" from client " << app_packet_of_frame->sender_ID << "."<< std::endl << "--------\n";
                        }
                    }
                }
            }
        }
    }
}
void Network::frame_printer(stack<Packet*> frame) {
    for (int i = 0; i <4; i++) {
        Packet* current_packet = packet_returner(frame, i);
        current_packet->print();
    }
    cout << "--------" << endl;

}
Client& Network::client_returner(vector<Client>& clients, string id) {
    for (Client& client : clients) {
        if (id == client.client_id)
            return client;
    }
    throw std::runtime_error("Client with ID '" + id + "' not found");
}
Client& Network::client_returner_by_MAC_adress(vector<Client>& clients, string mac_adress) {
    for (Client& client : clients) {
        if (mac_adress == client.client_mac)
            return client;
    }
    throw std::runtime_error("Client with MAC '" + mac_adress + "' not found");
}

void Network::message_creator(vector<Client> &clients, string& sender_ID, string& receiver_ID, string& message, const string &sender_port, const string &receiver_port, int& message_limit){
    cout << "Message to be sent: " << "\"" << message << "\"" << endl<< endl;
    Client* sender = &client_returner(clients, sender_ID);
    Client* receiver = &client_returner(clients, receiver_ID);
    string next_receiver_id = (*sender).routing_table[(*receiver).client_id];
    Client* next_hop = &client_returner(clients, next_receiver_id);
    int frame_no = 0;
    stack<Packet*> frame;
    for (string allowed_message : slice_message(message, message_limit)) {
        cout << "Frame #" << ++frame_no << endl;
        PhysicalLayerPacket *physicalLayerPacket = new PhysicalLayerPacket(
                PHYSICAL_LAYER_ID, (*sender).client_mac, (*next_hop).client_mac);
        NetworkLayerPacket *networkLayerPacket = new NetworkLayerPacket(
                NETWORK_LAYER_ID, (*sender).client_ip, (*receiver).client_ip);
        TransportLayerPacket *transportLayerPacket = new TransportLayerPacket(
                TRANSPORT_LAYER_ID, sender_port, receiver_port);
        ApplicationLayerPacket *applicationLayerPacket = new ApplicationLayerPacket(
                APPLICATION_LAYER_ID, (*sender).client_id, (*receiver).client_id, allowed_message);
        physicalLayerPacket->frame_number = frame_no;

        frame.push(applicationLayerPacket);
        frame.push(transportLayerPacket);
        frame.push(networkLayerPacket);
        frame.push(physicalLayerPacket);
        (*sender).outgoing_queue.push(frame);

        dynamic_packets.push(applicationLayerPacket);
        dynamic_packets.push(transportLayerPacket);
        dynamic_packets.push(networkLayerPacket);
        dynamic_packets.push(physicalLayerPacket);
    }
    time_t currentTime = time(nullptr);
    char timeString[100];
    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));
    Log log(timeString, message, frame_no, packet_returner(frame,0)->number_of_hops, sender_ID, receiver_ID,
              true, ActivityType::MESSAGE_SENT);
    (*sender).log_entries.push_back(log);
}

void Network::show_frame_info(Client& client, string& in_or_out, string& frame_number) {
    queue<stack<Packet*>> temp_queue;
    if (in_or_out == "in") {
        temp_queue = client.incoming_queue;
        in_or_out = "incoming";
    } else if (in_or_out == "out") {
        temp_queue = client.outgoing_queue;
        in_or_out = "outgoing";
    }
    int frame_no = stoi(frame_number);
    if (temp_queue.size() < frame_no){
        cout << "No such frame." << endl;
    } else {
        stack<Packet*> frame = stack_returner(temp_queue, frame_no);
        cout << "Current Frame #" << frame_no << " on the " << in_or_out << " queue of client " << client.client_id << endl;
        for (int i = 3; i >=0; i--) {
            Packet* current_packet = packet_returner(frame, i);
            current_packet->print_with_layer_info();
        }
    }
}
void Network::show_q_info(Client& client, string& in_or_out) {
    queue<stack<Packet*>> temp_queue;
    if (in_or_out == "in") {
        temp_queue = client.incoming_queue;
        in_or_out = "Incoming";
    } else if (in_or_out == "out"){
        temp_queue = client.outgoing_queue;
        in_or_out = "Outgoing";
    }
    cout << "Client " << client.client_id << " " << in_or_out<< " Queue Status\n"
            "Current total number of frames: " << temp_queue.size() << endl;
}

stack<Packet*> Network::stack_returner(queue<stack<Packet*>> original_queue, int no){
    for (int i = 0; i < no-1; ++i) {
        original_queue.pop();
    }
    return original_queue.front();
}
Packet* Network::packet_returner(stack<Packet*> original_stack, int no) {
    for (int i = 0; i < no; ++i) {
        original_stack.pop();
    }
    return original_stack.top();
}

vector<string> Network::slice_message(const string& message, size_t max_length) {
    vector<string> slices;
    for (size_t i = 0; i < message.length(); i += max_length) {
        slices.push_back(message.substr(i, max_length));
    }
    return slices;
}

vector<Client> Network::read_clients(const string &filename) {
    vector<Client> clients;
    // TODO: Read clients from the given input file and return a vector of Client instances.
    std::ifstream clients_file(filename);
    if (!clients_file.is_open()) {
        std::cerr << "error with input file";
        exit(-1);
    }
    string line;
    while (std::getline(clients_file, line)) {
        if (number_of_clients == 0) {
            number_of_clients = stoi(line);
            continue;
        }
        stringstream line_stream(line);
        string id, ip, mac;
        line_stream >> id >> ip >> mac;

        Client newClient(id, ip, mac);
        clients.push_back(newClient);
    }
    return clients;
}
void Network::read_routing_tables(vector<Client> &clients, const string &filename) {
    // TODO: Read the routing tables from the given input file and populate the clients' routing_table member variable.
    std::ifstream routing_file(filename);
    if (!routing_file.is_open()) {
        std::cerr << "error with input file";
        exit(-1);
    }

    string line;
    int current_client_num = 0;

    while (std::getline(routing_file, line)) {
        if (line == "-") {
            current_client_num++;
        } else {
            stringstream routes(line);
            string destination, neighbor;
            routes >> destination >> neighbor;
            clients[current_client_num].routing_table.insert({destination, neighbor});
        }
    }
}
// Returns a list of token lists for each command
vector<string> Network::read_commands(const string &filename) {
    vector<string> commands;
    // TODO: Read commands from the given input file and return them as a vector of strings.

    std::ifstream command_file(filename);
    if (!command_file.is_open()) {
        std::cerr << "error with input file";
        exit(-1);
    }
    string line;
    while (std::getline(command_file, line))
        commands.push_back(line);
    return commands;
}

Network::~Network() {
    // Free any dynamically allocated memory if necessary.
        while (!dynamic_packets.empty()) {
            Packet* packet = dynamic_packets.top();
            dynamic_packets.pop();
            delete packet;
        }
}



