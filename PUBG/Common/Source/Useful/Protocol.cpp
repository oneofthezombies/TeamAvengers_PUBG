#include "stdafx.h"
#include "Protocol.h"

Message::Message()
    : m_BodyLength(0u)
{
}

char* Message::GetData()
{
    return m_Data;
}

const char* Message::GetData() const
{
    return m_Data;
}

size_t Message::GetLength() const
{
    return HEADER_LENGTH + m_BodyLength;
}

char* Message::GetBodyData()
{
    return m_Data + HEADER_LENGTH;
}

const char* Message::GetBodyData() const
{
    return m_Data + HEADER_LENGTH;
}

void Message::SetBodyLength(size_t length)
{
    m_BodyLength = length;
    if (m_BodyLength > MAX_BODY_LENGTH)
        m_BodyLength = MAX_BODY_LENGTH;
}

size_t Message::GetBodyLength() const
{
    return m_BodyLength;
}

bool Message::DecodeHeader()
{
    char header[HEADER_LENGTH + 1] = "";
    memcpy_s(header, HEADER_LENGTH + 1, m_Data, HEADER_LENGTH);
    m_BodyLength = std::atoi(header);
    if (m_BodyLength > MAX_BODY_LENGTH)
    {
        m_BodyLength = 0u;
        return false;
    }

    return true;
}

void Message::EncodeHeader()
{
    stringstream ss;
    ss << setw(4) << setfill(' ') << static_cast<int>(m_BodyLength);
    const string str(ss.str());
    memcpy_s(m_Data, HEADER_LENGTH, str.data(), str.size());
}

TAG_REQUEST Message::GetRequest()
{
    return static_cast<TAG_REQUEST>(
        std::stoi(string(GetBodyData(), REQUEST_LENGTH)));
}

string Message::GetDescription()
{
    return string(GetBodyData() + REQUEST_LENGTH, 
        GetBodyLength() - REQUEST_LENGTH);
}

Message Message::Create(const TAG_REQUEST request, const string& description)
{
    stringstream ss;
    ss << setw(2) << setfill(' ') << static_cast<int>(request);
    ss << description;
    const string s(ss.str());

    Message m;
    memcpy_s(m.GetBodyData(), Message::MAX_BODY_LENGTH, s.data(), s.size());
    m.SetBodyLength(s.size());
    m.EncodeHeader();
    return m;
}

pair<int, string> Message::ParseDescription(const string& description)
{
    return make_pair(
        std::stoi(string(description.begin(), description.begin() + 1)),
        string(description.begin() + 1, description.end()));
}

ostream& operator<<(ostream& os, const Message& msg)
{
    os << string(msg.GetData(), msg.GetLength());
    return os;
}

GameInfo::PlayerInfo::PlayerInfo()
    : m_ID(-1)
    , m_nickname("Unknown")
    , m_upperAnimState(135)
    , m_lowerAnimState(135)
{
    memset(&m_position, 0, sizeof m_position);
    memset(&m_rotation, 0, sizeof m_rotation);
}

GameInfo::MyInfo::MyInfo()
    : m_ID(-1)
{
}

GameInfo::RoomInfo::RoomInfo()
    : m_PlayerInfos()
{
}
