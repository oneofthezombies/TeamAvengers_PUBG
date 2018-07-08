#pragma once
#include "Singleton.h"
#include "Protocol.h"

#define g_pCommunication Communication()()

class CommunicationManager;
class Bullet;

class Client
{
private:
    tcp::socket m_socket;
    Message     m_readMsg;
    Message     m_writeMsg;

    boost::asio::io_context* pIOContext;
    CommunicationManager*    pCommunicationManager;

    void Connect(const tcp::resolver::results_type& endpoints);
    void ReadHeader();
    void ReadBody();
    void Write();

public:
    Client(
        boost::asio::io_context* pIOContext, 
        const tcp::resolver::results_type& endpoints, 
        CommunicationManager* pCommunicationManager);

    void Write(const Message& msg);
    void Close();
};

class CommunicationManager : public Singleton<CommunicationManager>
{
public:
    GameInfo::RoomInfo m_roomInfo;
    GameInfo::MyInfo   m_myInfo;

private:
    boost::asio::io_context m_IOContext;
    tcp::resolver           m_resolver;
    Client*                 m_pClient;
    std::thread*            m_pThread;

             CommunicationManager();
    virtual ~CommunicationManager();

    void CheckConnection();

public:
    void Destroy();
    void Print();

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
    void SendUpperAnimationIndex(const TAG_ANIM_CHARACTER tag);
    void SendLowerAnimationIndex(const TAG_ANIM_CHARACTER tag);

    void SendEventFireBullet(Bullet* pBullet);
    void SendEventSound(const TAG_SOUND tag, const D3DXVECTOR3& p);

    friend Singleton<CommunicationManager>;
};

struct Communication
{
    CommunicationManager* operator()()
    {
        return CommunicationManager::GetInstance();
    }
};