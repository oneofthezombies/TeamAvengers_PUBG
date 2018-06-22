#pragma once
#include "Singleton.h"
#include "Protocol.h"

#define g_pCommunicator Communicator::GetInstance()

class Communicator;

class Client
{
private:
    boost::asio::io_context* m_pIOContext;
    tcp::socket m_Socket;
    Message m_ReadMsg;
    Message m_WriteMsg;
    Communicator* m_pCommunicator;

    void Connect(const tcp::resolver::results_type& endpoints);
    void ReadHeader();
    void ReadBody();
    void Write();

public:
    Client(boost::asio::io_context* ioContext, 
        const tcp::resolver::results_type& endpoints, 
        Communicator* pCommunicator);

    void Write(const Message& msg);
    void Close();
};

class Communicator : public Singleton<Communicator>
{
public:
    GameInfo::RoomInfo m_RoomInfo;
    GameInfo::MyInfo   m_MyInfo;

private:
    boost::asio::io_context m_IOContext;
    tcp::resolver           m_Resolver;
    Client*                 m_pClient;
    std::thread*            m_pThread;

    Communicator();
    virtual ~Communicator();

    void CheckConnection();

public:
    void Destroy();
    void Logging();

    void Connect(const string& host, const string& port, 
        const string& nickname);

    void ReceiveMessage(const TAG_REQUEST tag, const string& description);
    
    void ReceiveID(const int id);
    void SendID(const int id);
    void SendNickname(const string& nickname);
    void SendPosition(const D3DXVECTOR3& pos);
    void SendAnimationIndex(const int index);
    void SendAnimationTime(const float time);

    friend Singleton<Communicator>;
};