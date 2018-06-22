#include "stdafx.h"
#include "Client.h"

void Client::Connect(const tcp::resolver::results_type& endpoints)
{
    boost::asio::async_connect(m_Socket, endpoints, [this](auto ec, auto) 
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
    boost::asio::async_read(m_Socket, 
        boost::asio::buffer(m_ReadMsg.GetData(), Message::HEADER_LENGTH), 
        [this](auto ec, auto) 
    {
        if (!ec && m_ReadMsg.DecodeHeader())
        {
            ReadBody();
        }
        else
        {
            cout << "Client::ReadHeader() failed.\n";
            m_Socket.close();
        }
    });
}

void Client::ReadBody()
{
    boost::asio::async_read(m_Socket, 
        boost::asio::buffer(m_ReadMsg.GetBodyData(), 
            m_ReadMsg.GetBodyLength()), [this](auto ec, auto) 
    {
        if (!ec)
        {
            if (m_pCommunicator)
                m_pCommunicator->ReceiveMessage(m_ReadMsg.GetRequest(), 
                    m_ReadMsg.GetDescription());
            
            ReadHeader();
        }
        else
        {
            cout << "Client::ReadBody() failed.\n";
            m_Socket.close();
        }
    });
}

void Client::Write()
{
    boost::asio::async_write(m_Socket, 
        boost::asio::buffer(m_WriteMsg.GetData(), m_WriteMsg.GetLength()), 
        [this](auto ec, auto) 
    {
        if (!ec)
        {
            // do nothing
        }
        else
        {
            cout << "Client::Write() failed.\n";
            m_Socket.close();
        }
    });
}

Client::Client(boost::asio::io_context* ioContext, 
    const tcp::resolver::results_type& endpoints, Communicator* pCommunicator)
    : m_pIOContext(ioContext)
    , m_Socket(*ioContext)
    , m_pCommunicator(pCommunicator)
{
    Connect(endpoints);
}

void Client::Write(const Message& msg)
{
    boost::asio::post(*m_pIOContext, [this, msg]()
    {
        m_WriteMsg = msg;
        Write();
    });
}

void Client::Close()
{
    boost::asio::post(*m_pIOContext, [this]() { m_Socket.close(); });
}

Communicator::Communicator()
    : Singleton<Communicator>()
    , m_pClient(nullptr)
    , m_pThread(nullptr)
    , m_IOContext()
    , m_Resolver(m_IOContext)
{
}

Communicator::~Communicator()
{
}

void Communicator::CheckConnection()
{
    assert(m_pClient && m_pThread && "client or thread is null.");
    assert((m_MyInfo.m_ID > -1) && 
        (m_MyInfo.m_ID < static_cast<int>(m_RoomInfo.m_PlayerInfos.size())) &&
        "my id is wrong.");
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
    //Debug << "My ID : " << m_
    //Debug << "myID : " << info.myID << '\n';
    //Debug << "myNickname : " << info.myNickname << '\n';
    //for (auto& i : info.ID)
    //    Debug << "ID : " << i << ", ";
    //Debug << '\n';
    //for (auto& n : info.nickname)
    //    Debug << "nickname : " << n << ", ";
    //Debug << '\n';
}

void Communicator::Connect(const string& host, const string& port, 
    const string& nickname)
{
    auto endpoints = m_Resolver.resolve(host, port);

    m_pClient = new Client(&m_IOContext, endpoints, this);
    m_pThread = new std::thread([this]() { m_IOContext.run(); });

    Sleep(500);

    m_MyInfo.m_Nickname = nickname;

    m_pClient->Write(Message::Create(TAG_REQUEST::RECEIVE_MY_ID, nickname));
}

void Communicator::ReceiveMessage(const TAG_REQUEST tag, const string& description)
{
    switch (tag)
    {
    case TAG_REQUEST::RECEIVE_MY_ID:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;

            ReceiveID(id);
            
            SendID(m_MyInfo.m_ID);
            SendNickname(m_MyInfo.m_Nickname);
        }
        break;
    case TAG_REQUEST::SEND_ID:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;

            m_RoomInfo.m_PlayerInfos[id].m_ID = id;
        }
        break;
    case TAG_REQUEST::SEND_NICKNAME:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& nickname = parsedDesc.second;

            m_RoomInfo.m_PlayerInfos[id].m_Nickname = nickname;
        }
        break;
    case TAG_REQUEST::SEND_POSITION:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& positionStr = parsedDesc.second;

            stringstream ss(positionStr);
            D3DXVECTOR3 pos;
            ss >> pos.x >> pos.y >> pos.z;
            m_RoomInfo.m_PlayerInfos[id].m_Position = pos;
        }
        break;
    case TAG_REQUEST::SEND_ANIMATION_INDEX:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& indexStr = parsedDesc.second;

            const int index = std::stoi(indexStr);
            m_RoomInfo.m_PlayerInfos[id].m_AnimationIndex = index;
        }
        break;
    case TAG_REQUEST::SEND_ANIMATION_TIME:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& timeStr = parsedDesc.second;

            stringstream ss(timeStr);
            float time;
            ss >> time;
            m_RoomInfo.m_PlayerInfos[id].m_AnimationTime = time;
        }
        break;
    }
}

void Communicator::ReceiveID(const int id)
{
    m_MyInfo.m_ID = id;
    cout << "Received ID : " << id << '\n';
}

void Communicator::SendID(const int id)
{
    m_RoomInfo.m_PlayerInfos[id].m_ID = id;

    m_pClient->Write(Message::Create(TAG_REQUEST::SEND_ID, to_string(id)));
}

void Communicator::SendNickname(const string& nickname)
{
    m_RoomInfo.m_PlayerInfos[m_MyInfo.m_ID].m_Nickname = nickname;

    stringstream ss;
    ss << m_MyInfo.m_ID << nickname;
    m_pClient->Write(Message::Create(TAG_REQUEST::SEND_NICKNAME, ss.str()));
}

void Communicator::SendPosition(const D3DXVECTOR3& pos)
{
    m_RoomInfo.m_PlayerInfos[m_MyInfo.m_ID].m_Position = pos;

    stringstream ss;
    ss << m_MyInfo.m_ID << pos.x << ' ' << pos.y << ' ' << pos.z;
    m_pClient->Write(Message::Create(TAG_REQUEST::SEND_POSITION, ss.str()));
}

void Communicator::SendAnimationIndex(const int index)
{
    m_RoomInfo.m_PlayerInfos[m_MyInfo.m_ID].m_AnimationIndex = index;

    stringstream ss;
    ss << m_MyInfo.m_ID << index;
    m_pClient->Write(Message::Create(TAG_REQUEST::SEND_ANIMATION_INDEX, 
        ss.str()));
}

void Communicator::SendAnimationTime(const float time)
{
    m_RoomInfo.m_PlayerInfos[m_MyInfo.m_ID].m_AnimationTime = time;

    stringstream ss;
    ss << m_MyInfo.m_ID << time;
    m_pClient->Write(Message::Create(TAG_REQUEST::SEND_ANIMATION_TIME,
        ss.str()));
}
