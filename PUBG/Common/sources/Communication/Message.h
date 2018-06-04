#pragma once

#include "Protocol.h"

class Message
{
public:
    static const int REQUEST_SIZE = 2;
    static const int DESCRIPTION_SIZE_INFO_SIZE = 4;
    static const int MAX_DESCRIPTION_SIZE = 256;

private:
    array<char, REQUEST_SIZE + DESCRIPTION_SIZE_INFO_SIZE + MAX_DESCRIPTION_SIZE> m_data;
    size_t m_descriptionSize;

public:
    Message() = default;
    ~Message() = default;

    char*       GetData();
    const char* GetData() const;

    char*       GetRequestData();
    const char* GetRequestData() const;

    char*       GetDescriptionSizeInfoData();
    const char* GetDescriptionSizeInfoData() const;

    char*       GetDescriptionData();
    const char* GetDescriptionData() const;

    size_t GetSize() const;

    bool IsValidDescriptionSize();

    void   SetDescriptionSize(const size_t size);
    size_t GetDescriptionSize() const;

    REQUEST GetRequest() const;
    string GetDescription() const;

    void SetMessage(const string& val);
};