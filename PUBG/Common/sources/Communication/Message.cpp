#include "stdafx.h"
#include "Message.h"

size_t Message::GetSize() const
{
    return REQUEST_SIZE + DESCRIPTION_SIZE_INFO_SIZE + m_descriptionSize;
}

char* Message::GetData()
{
    return m_data.data();
}

const char* Message::GetData() const
{
    return m_data.data();
}

char* Message::GetRequestData()
{
    return m_data.data();
}

const char* Message::GetRequestData() const
{
    return m_data.data();
}

char* Message::GetDescriptionSizeInfoData()
{
    return GetRequestData() + REQUEST_SIZE;
}

const char* Message::GetDescriptionSizeInfoData() const
{
    return GetRequestData() + REQUEST_SIZE;
}

char* Message::GetDescriptionData()
{
    return GetDescriptionSizeInfoData() + DESCRIPTION_SIZE_INFO_SIZE;
}

const char* Message::GetDescriptionData() const
{
    return GetDescriptionSizeInfoData() + DESCRIPTION_SIZE_INFO_SIZE;
}

bool Message::IsValidDescriptionSize()
{
    const string descSizeInfo(GetDescriptionSizeInfoData(), Message::DESCRIPTION_SIZE_INFO_SIZE);
    m_descriptionSize = std::stoi(descSizeInfo);
    if (m_descriptionSize > MAX_DESCRIPTION_SIZE)
    {
        m_descriptionSize = 0u;
        return false;
    }

    return true;
}

void Message::SetDescriptionSize(const size_t size)
{
    m_descriptionSize = size;
    if (m_descriptionSize > MAX_DESCRIPTION_SIZE)
        m_descriptionSize = MAX_DESCRIPTION_SIZE;
}

size_t Message::GetDescriptionSize() const
{
    return m_descriptionSize;
}

REQUEST Message::GetRequest() const
{
    if (m_data.size() < REQUEST_SIZE)
        return REQUEST::TestConnection;

    auto begin = m_data.begin();
    auto end = begin + REQUEST_SIZE;
    return static_cast<REQUEST>(std::stoi(string(begin, end)));
}

string Message::GetDescription() const
{
    if (m_data.size() < REQUEST_SIZE + DESCRIPTION_SIZE_INFO_SIZE)
        return string();

    auto begin = m_data.begin() + REQUEST_SIZE + DESCRIPTION_SIZE_INFO_SIZE;
    auto end = m_data.end();
    return string(begin, end);
}

void Message::SetMessage(const string& val)
{
    memcpy_s(m_data.data(), GetSize(), val.c_str(), val.size());
}