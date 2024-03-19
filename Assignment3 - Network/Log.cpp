//
// Created by alperen on 2.10.2023.
//

#include "Log.h"

Log::Log(const string &_timestamp, const string &_message, int _number_of_frames, int _number_of_hops, const string &_sender_id,
         const string &_receiver_id, bool _success, ActivityType _type) {
    timestamp = _timestamp;
    message_content = _message;
    number_of_frames = _number_of_frames;
    number_of_hops = _number_of_hops;
    sender_id = _sender_id;
    receiver_id = _receiver_id;
    success_status = _success;
    activity_type = _type;
}

void Log::print() {
    string activity;
    if (activity_type == ActivityType::MESSAGE_SENT)
        activity = "Message Sent";
    else if (activity_type == ActivityType::MESSAGE_DROPPED)
        activity = "Message Dropped";
    else if (activity_type == ActivityType::MESSAGE_FORWARDED)
        activity = "Message Forwarded";
    else if (activity_type == ActivityType::MESSAGE_RECEIVED)
        activity = "Message Received";
    string success;
    if (success_status)
        success = "Yes";
    else if (!success_status)
        success = "No";

    cout << "Activity: " << activity << "\n"
            "Timestamp: " << timestamp << "\n"
            "Number of frames: " << number_of_frames << "\n"
            "Number of hops: " << number_of_hops << "\n"
            "Sender ID: " << sender_id << "\n"
            "Receiver ID: " << receiver_id << "\n"
            "Success: " << success << endl ;
    if (activity_type == ActivityType::MESSAGE_SENT || activity_type == ActivityType::MESSAGE_RECEIVED)
        cout << "Message: \""<< message_content << "\"" << endl;

}

Log::~Log() {
    // TODO: Free any dynamically allocated memory if necessary.
}