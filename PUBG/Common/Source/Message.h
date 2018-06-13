#pragma once
#include "TagRequest.h"

class Message
{
public:
    static const int HEADER_LENGTH = 4;
    static const int MAX_BODY_LENGTH = 512;

private:
    char   m_data[HEADER_LENGTH + MAX_BODY_LENGTH];
    size_t m_bodyLength;

public:
    Message();
    ~Message() = default;

    char*       GetData();
    const char* GetData() const;

    size_t GetLength() const;

    char*       GetBodyData();
    const char* GetBodyData() const;

    void   SetBodyLength(size_t length);
    size_t GetBodyLength() const;

    bool DecodeHeader();
    void EncodeHeader();

    TAG_REQUEST GetRequest();
    string      GetDescription();

    static void Create(Message* OutMsg, const TAG_REQUEST request, const string& description);
};

ostream& operator<<(ostream& os, const Message& msg);