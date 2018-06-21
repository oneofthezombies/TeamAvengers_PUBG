#include "stdafx.h"
#include "Client.h"

void Client::Connect(const tcp::resolver::results_type& endpoints)
{
    boost::asio::async_connect(m_socket, endpoints, [this](auto ec, auto) 
    {
        if (!ec)
        {
            ReadHeader();
        }
        else
        {
            cout << "Client::Connect() failed.\n";
        }
    });
}

void Client::ReadHeader()
{
    boost::asio::async_read(m_socket, boost::asio::buffer(m_readMsg.GetData(), Message::HEADER_LENGTH), [this](auto ec, auto) 
    {
        if (!ec && m_readMsg.DecodeHeader())
        {
            ReadBody();
        }
        else
        {
            cout << "Client::ReadHeader() failed.\n";
            m_socket.close();
        }
    });
}

void Client::ReadBody()
{
    boost::asio::async_read(m_socket, boost::asio::buffer(m_readMsg.GetBodyData(), m_readMsg.GetBodyLength()), [this](auto ec, auto) 
    {
        if (!ec)
        {
            TAG_REQUEST request(m_readMsg.GetRequest());
            switch (request)
            {
            case TAG_REQUEST::MY_ID:
                {
                    const int id(std::stoi(m_readMsg.GetDescription()));
                    cout << "Received ID : " << id << '\n';
                    m_pCommunicator->info.myID = id;
                    m_pCommunicator->info.ID[id] = id;

                    Message::Create(&m_writeMsg, TAG_REQUEST::ID, to_string(id));
                    Write();

                    Message::Create(&m_writeMsg, TAG_REQUEST::NICKNAME, to_string(id) + m_pCommunicator->info.myNickname);
                    Write();
                }
                break;
            case TAG_REQUEST::ID:
                {
                    const int id(std::stoi(m_readMsg.GetDescription()));
                    cout << "Echo ID : " << id << '\n';
                    m_pCommunicator->info.ID[id] = id;
                }
                break;
            case TAG_REQUEST::NICKNAME:
                {
                    const string desc = m_readMsg.GetDescription();
                    const int id(std::stoi(string(desc.begin(), desc.begin() + 1)));
                    if (id == m_pCommunicator->info.myID) break;

                    const string nickname(string(desc.begin() + 1, desc.end()));
                    m_pCommunicator->info.nickname[id] = nickname;
                }
                break;
            case TAG_REQUEST::POSITION:
                {
                    const string desc = m_readMsg.GetDescription();
                    const int id(std::stoi(string(desc.begin(), desc.begin() + 1)));
                    if (id == m_pCommunicator->info.myID) break;

                    stringstream ss(string(desc.begin() + 1, desc.end()));
                    D3DXVECTOR3 pos;
                    ss >> pos.x >> pos.y >> pos.z;
                    m_pCommunicator->info.ID[id] = id;
                    m_pCommunicator->info.position[id] = pos;
                }
                break;
            }

            ReadHeader();
        }
        else
        {
            cout << "Client::ReadBody() failed.\n";
            m_socket.close();
        }
    });
}

void Client::Write()
{
    boost::asio::async_write(m_socket, boost::asio::buffer(m_writeMsg.GetData(), m_writeMsg.GetLength()), [this](auto ec, auto) 
    {
        if (!ec)
        {
            // do nothing
        }
        else
        {
            cout << "Client::Write() failed.\n";
            m_socket.close();
        }
    });
}

Client::Client(boost::asio::io_context* ioContext, const tcp::resolver::results_type& endpoints, Communicator* pCommunicator)
    : m_pIoContext(ioContext)
    , m_socket(*ioContext)
    , m_pCommunicator(pCommunicator)
{
    Connect(endpoints);
}

void Client::Write(const Message& msg)
{
    boost::asio::post(*m_pIoContext, [this, msg]()
    {
        m_writeMsg = msg;
        Write();
    });
}

void Client::Close()
{
    boost::asio::post(*m_pIoContext, [this]() { m_socket.close(); });
}

Communicator::Communicator()
    : Singleton<Communicator>()
    , m_pClient(nullptr)
    , m_pThread(nullptr)
    , m_ioContext()
    , m_resolver(m_ioContext)
{
    memset(m_host, 0, 32);
    memset(m_port, 0, 32);
}

Communicator::~Communicator()
{
}

void Communicator::Destroy()
{
    if (m_pClient)
        m_pClient->Close();

    if (m_pThread)
        m_pThread->join();

    SAFE_DELETE(m_pClient);
    SAFE_DELETE(m_pThread);
}

void Communicator::Logging()
{
    Debug << "myID : " << info.myID << '\n';
    Debug << "myNickname : " << info.myNickname << '\n';
    for (auto& i : info.ID)
        Debug << "ID : " << i << ", ";
    Debug << '\n';
    for (auto& n : info.nickname)
        Debug << "nickname : " << n << ", ";
    Debug << '\n';
}

bool Communicator::Connect(const string& host, const string& port, const string& nickname)
{
    memcpy_s(m_host, 32, host.c_str(), host.size());
    m_host[host.size()] = 0;
    memcpy_s(m_port, 32, port.c_str(), port.size());
    m_port[port.size()] = 0;
    cout << m_host << " " << m_port << endl;

    auto endpoints = m_resolver.resolve(m_host, m_port);
    
    m_pClient = new Client(&m_ioContext, endpoints, this);
    m_pThread = new std::thread([this]() { m_ioContext.run(); });

    Sleep(500);

    Message msg;
    Message::Create(&msg, TAG_REQUEST::MY_ID, nickname);
    m_pClient->Write(msg);
    return true;
}

bool Communicator::UpdatePosition(const D3DXVECTOR3& pos)
{
    if (!m_pClient || !m_pThread) false;

    info.position[info.myID] = pos;
    Message msg;
    stringstream ss;
    ss << info.myID;
    ss << pos.x << ' ' << pos.y << ' ' << pos.z;
    Message::Create(&msg, TAG_REQUEST::POSITION, ss.str());
    m_pClient->Write(msg);

    return true;
}

Communicator::Info::Info()
    : myID(-1)
    , myNickname("Unknown")
{
    for (auto& i : ID)
        i = -1;

    for (auto& n : nickname)
        n = "Unknown";
}
