#pragma once
#include "Singleton.h"
#include "Message.h"

#define g_pCommunicator Communicator::GetInstance()

class Communicator;

class Client
{
private:
    boost::asio::io_context* m_pIoContext;
    tcp::socket m_socket;
    Message m_readMsg;
    Message m_writeMsg;
    Communicator* m_pCommunicator;

    void Connect(const tcp::resolver::results_type& endpoints);
    void ReadHeader();
    void ReadBody();
    void Write();

public:
    Client(boost::asio::io_context* ioContext, const tcp::resolver::results_type& endpoints, Communicator* pCommunicator);

    void Write(const Message& msg);
    void Close();
};

class Communicator : public Singleton<Communicator>
{
public:
    struct Info
    {
        int                   myID;
        string                myNickname;
        array<int, 2>         ID;
        array<string, 2>      nickname;
        array<D3DXVECTOR3, 2> position;

        Info();
    };

    Info info;

private:
    boost::asio::io_context m_ioContext;
    tcp::resolver           m_resolver;
    Client*                 m_pClient;
    std::thread*            m_pThread;
    char m_host[32];
    char m_port[32];

    Communicator();
    virtual ~Communicator();

public:
    void Destroy();
    void Logging();

    bool Connect(const string& host, const string& port, const string& nickname);
    bool UpdatePosition(const D3DXVECTOR3& pos);

    friend Singleton<Communicator>;
};