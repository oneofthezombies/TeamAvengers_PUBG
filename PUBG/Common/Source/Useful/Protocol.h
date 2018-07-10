#pragma once
#include "TagCommon.h"

class Message
{
public:
    static const int HEADER_LENGTH = 4;
    static const int MAX_BODY_LENGTH = 512;
    static const int REQUEST_LENGTH = 2;

private:
    char        m_data[HEADER_LENGTH + MAX_BODY_LENGTH];
    std::size_t m_bodyLength;

public:
     Message();
    ~Message() = default;

    char*       GetData();
    const char* GetData() const;

    std::size_t GetLength() const;

    char*       GetBodyData();
    const char* GetBodyData() const;

    void        SetBodyLength(const std::size_t length);
    std::size_t GetBodyLength() const;

    bool DecodeHeader();
    void EncodeHeader();

    TAG_REQUEST GetRequest();
    std::string GetDescription();

    static Message Create(
        const TAG_REQUEST request, 
        const std::string& description);
    static std::pair<int, std::string> 
        ParseDescription(const string& description);
};

std::ostream& operator<<(std::ostream& os, const Message& msg);

struct GameInfo
{
    struct PlayerInfo
    {
        int            ID;
        std::string    nickname;
        D3DXVECTOR3    position;
        D3DXQUATERNION rotation;
        float          headAngle;
        int            upperAnimState;
        int            lowerAnimState;
        float          health;
        bool           isDead;


        // this is not sending.
        bool isApplied;

        PlayerInfo();
    };

    struct MyInfo
    {
        int         ID;
        std::string nickname;

        MyInfo();
    };

    static const int NUM_PLAYERS = 4;

    struct RoomInfo
    {
        std::array<PlayerInfo, NUM_PLAYERS> playerInfos;

        RoomInfo();
    };
};