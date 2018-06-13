#include "stdafx.h"
#include "Message.h"

Message::Message()
    : m_bodyLength(0u)
{
}

char* Message::GetData()
{
    return m_data;
}

const char* Message::GetData() const
{
    return m_data;
}

size_t Message::GetLength() const
{
    return HEADER_LENGTH + m_bodyLength;
}

char* Message::GetBodyData()
{
    return m_data + HEADER_LENGTH;
}

const char* Message::GetBodyData() const
{
    return m_data + HEADER_LENGTH;
}

void Message::SetBodyLength(size_t length)
{
    m_bodyLength = length;
    if (m_bodyLength > MAX_BODY_LENGTH)
        m_bodyLength = MAX_BODY_LENGTH;
}

size_t Message::GetBodyLength() const
{
    return m_bodyLength;
}

bool Message::DecodeHeader()
{
    char header[HEADER_LENGTH + 1] = "";
    memcpy_s(header, HEADER_LENGTH + 1, m_data, HEADER_LENGTH);
    m_bodyLength = std::atoi(header);
    if (m_bodyLength > MAX_BODY_LENGTH)
    {
        m_bodyLength = 0u;
        return false;
    }

    return true;
}

void Message::EncodeHeader()
{
    stringstream ss;
    ss << setw(4) << setfill(' ') << static_cast<int>(m_bodyLength);
    const string str(ss.str());
    memcpy_s(m_data, HEADER_LENGTH, str.data(), str.size());
}

TAG_REQUEST Message::GetRequest()
{
    return static_cast<TAG_REQUEST>(std::stoi(string(GetBodyData(), 2)));
}

string Message::GetDescription()
{
    return string(GetBodyData() + 2, GetBodyLength() - 2);
}

void Message::Create(Message* OutMsg, const TAG_REQUEST request, const string& description)
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

ostream& operator<<(ostream& os, const Message& msg)
{
    os << string(msg.GetData(), msg.GetLength());
    return os;
}