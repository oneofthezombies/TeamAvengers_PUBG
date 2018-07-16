#pragma once
#include "Singleton.h"
#include "Protocol.h"

#define g_pCommunication Communication()()

class Bullet;
class Client;

struct Communication
{
    enum class PlayMode
    {
        // for local test
        ALONE,

        WITH_OTHERS
    };

    class Manager : public Singleton<Manager>
    {
    public:
        GameInfo::RoomInfo m_roomInfo;
        GameInfo::MyInfo   m_myInfo;

    private:
        boost::asio::io_context m_IOContext;
        tcp::resolver           m_resolver;
        Client*                 m_pClient;
        std::thread*            m_pThread;
        PlayMode                m_playMode;

    private:
        Manager();
        virtual ~Manager();

        void CheckConnection();

    public:
        void Destroy();
        void Print();

        void     SetPlayMode(const PlayMode playMode);
        PlayMode GetPlayMode() const;

        void Connect(
            const std::string& host,
            const std::string& port,
            const std::string& nickname);

        void ReceiveMessage(const TAG_REQUEST tag, const std::string& description);

        void ReceiveID(const int id);
        void SendID(const int id);
        void SendNickname(const std::string& nickname);

        void SendPositionAndRotation(
            const D3DXVECTOR3& p,
            const D3DXQUATERNION& r);
        void SendHeadAngle(const float angle);
        void SendUpperAnimationIndex(const TAG_ANIM_CHARACTER tag);
        void SendLowerAnimationIndex(const TAG_ANIM_CHARACTER tag);
        void SendIsDead(const int id, bool isDead);

        //void SendEventFireBullet(Bullet* pBullet);
        void SendEventSound(const TAG_SOUND tag, const D3DXVECTOR3& p);
        void SendEventMinusDamage(const int id, const float damage);
        void SendEventMoveItemFromFieldToCharacter(
            const int id, 
            const std::string& itemName);
        void SendEventDestroyItem(const std::string& itemName);

        friend Singleton<Manager>;
    };

    Manager* operator()();
};

class Client
{
private:
    tcp::socket m_socket;
    Message     m_readMsg;
    Message     m_writeMsg;

    boost::asio::io_context* pIOContext;
    Communication::Manager*  pCommunicationManager;

    void Connect(const tcp::resolver::results_type& endpoints);
    void ReadHeader();
    void ReadBody();
    void Write();

public:
    Client(
        boost::asio::io_context* pIOContext,
        const tcp::resolver::results_type& endpoints,
        Communication::Manager* pCommunicationManager);

    void Write(const Message& msg);
    void Close();
};