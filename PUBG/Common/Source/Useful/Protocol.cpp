#include "stdafx.h"
#include "Protocol.h"

Message::Message()
    : m_bodyLength(0)
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

std::size_t Message::GetLength() const
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

void Message::SetBodyLength(const std::size_t length)
{
    m_bodyLength = length;
    if (m_bodyLength > MAX_BODY_LENGTH)
        m_bodyLength = MAX_BODY_LENGTH;
}

std::size_t Message::GetBodyLength() const
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
        m_bodyLength = 0;
        return false;
    }

    return true;
}

void Message::EncodeHeader()
{
    std::stringstream ss;
    ss << setw(4) << setfill(' ') << static_cast<int>(m_bodyLength);
    const std::string str(ss.str());
    memcpy_s(m_data, HEADER_LENGTH, str.data(), str.size());
}

TAG_REQUEST Message::GetRequest()
{
    return static_cast<TAG_REQUEST>(
        std::stoi(std::string(GetBodyData(), REQUEST_LENGTH)));
}

std::string Message::GetDescription()
{
    return std::string(GetBodyData() + REQUEST_LENGTH, 
        GetBodyLength() - REQUEST_LENGTH);
}

Message Message::Create(
    const TAG_REQUEST request, 
    const std::string& description)
{
    std::stringstream ss;
    ss << setw(2) << setfill(' ') << static_cast<int>(request);
    ss << description;
    const std::string s(ss.str());

    Message m;
    memcpy_s(m.GetBodyData(), Message::MAX_BODY_LENGTH, s.data(), s.size());
    m.SetBodyLength(s.size());
    m.EncodeHeader();
    return m;
}

std::pair<int, std::string> Message::ParseDescription(
    const std::string& description)
{
    return std::make_pair(
        std::stoi(std::string(description.begin(), description.begin() + 1)),
        std::string(description.begin() + 1, description.end()));
}

std::ostream& operator<<(std::ostream& os, const Message& msg)
{
    os << std::string(msg.GetData(), msg.GetLength());
    return os;
}

GameInfo::PlayerInfo::PlayerInfo()
    : ID(-1)
    , nickname("Unknown")
    , position(Vector3::ZERO)
    , rotation(Quaternion::IDENTITY)
    , upperAnimState(135)
    , lowerAnimState(135)
{
}

GameInfo::MyInfo::MyInfo()
    : ID(-1)
{
}

GameInfo::RoomInfo::RoomInfo()
    : playerInfos()
{
}
