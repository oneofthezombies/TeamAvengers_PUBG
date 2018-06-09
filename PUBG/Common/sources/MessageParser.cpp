#include "stdafx.h"
#include "MessageParser.h"

MessageParser::MessageParser(const Message& msg)
    : m_msg(msg)
{
}

MessageParser::~MessageParser()
{
}

REQUEST MessageParser::GetRequest()
{
    string requestStr(m_msg.GetBodyData(), 2);
    return static_cast<REQUEST>(std::stoi(requestStr));
}

string MessageParser::GetDescription()
{
    return string(m_msg.GetBodyData() + 2, m_msg.GetBodyLength() - 2);
}

void MessageParser::SetMessage(Message* OutMsg, const REQUEST request, const string& description)
{
    assert(OutMsg && "MessageParser::SetMessage(), OutMsg is NULL.");

    stringstream ss;
    ss << setw(2) << setfill(' ') << static_cast<int>(request);
    ss << description;
    const string str(ss.str());

    memcpy_s(OutMsg->GetBodyData(), Message::MAX_BODY_LENGTH, str.data(), str.size());
    OutMsg->SetBodyLength(str.size());
    OutMsg->EncodeHeader();
}