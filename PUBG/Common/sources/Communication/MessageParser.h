#pragma once

#include "Message.h"
#include "Protocol.h"

class MessageParser
{
private:
    Message m_msg;

public:
    MessageParser(const Message& msg);
    ~MessageParser();

    REQUEST GetRequest();
    string GetDescription();
    void SetMessage(Message* OutMsg, const REQUEST request, const string& description);
};