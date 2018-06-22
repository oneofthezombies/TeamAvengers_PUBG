#pragma once
#include "TagRequest.h"

class Message
{
public:
    static const int HEADER_LENGTH = 4;
    static const int MAX_BODY_LENGTH = 512;
    static const int REQUEST_LENGTH = 2;

private:
    char   m_Data[HEADER_LENGTH + MAX_BODY_LENGTH];
    size_t m_BodyLength;

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

    static Message Create(const TAG_REQUEST request, const string& description);
    static pair<int, string> ParseDescription(const string& description);
};

ostream& operator<<(ostream& os, const Message& msg);

struct GameInfo
{
    struct PlayerInfo
    {
        int            m_ID;
        string         m_Nickname;
        D3DXVECTOR3    m_Position;
        D3DXQUATERNION m_Rotation;
        int            m_AnimationIndex;
        float          m_AnimationTime;

        PlayerInfo();
    };

    struct Event
    {
    };

    struct MyInfo
    {
        int m_ID;
        string m_Nickname;

        MyInfo();
    };

    static const int NUM_PLAYERS = 4;

    struct RoomInfo
    {
        array<PlayerInfo, NUM_PLAYERS> m_PlayerInfos;

        RoomInfo();
    };
};