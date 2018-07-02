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
            if (pCommunicationManager)
                pCommunicationManager->ReceiveMessage(m_ReadMsg.GetRequest(),
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
    const tcp::resolver::results_type& endpoints, 
    CommunicationManager* pCommunicationManager)
    : m_Socket(*ioContext)
    , pIOContext(ioContext)
    , pCommunicationManager(pCommunicationManager)
{
    Connect(endpoints);
}

void Client::Write(const Message& msg)
{
    boost::asio::post(*pIOContext, [this, msg]()
    {
        m_WriteMsg = msg;
        Write();
    });
}

void Client::Close()
{
    boost::asio::post(*pIOContext, [this]() { m_Socket.close(); });
}

CommunicationManager::CommunicationManager()
    : Singleton<CommunicationManager>()
    , m_pClient(nullptr)
    , m_pThread(nullptr)
    , m_IOContext()
    , m_Resolver(m_IOContext)
{
}

CommunicationManager::~CommunicationManager()
{
}

void CommunicationManager::CheckConnection()
{
    assert(m_pClient && m_pThread && "client or thread is null.");
    assert((m_MyInfo.m_ID > -1) && 
        (m_MyInfo.m_ID < static_cast<int>(m_RoomInfo.m_PlayerInfos.size())) &&
        "my id is wrong.");
}

void CommunicationManager::Destroy()
{
    if (m_pClient)
        m_pClient->Close();

    if (m_pThread)
        m_pThread->join();

    SAFE_DELETE(m_pClient);
    SAFE_DELETE(m_pThread);
}

void CommunicationManager::Print()
{
}

void CommunicationManager::Connect(const string& host, const string& port,
    const string& nickname)
{
    auto endpoints = m_Resolver.resolve(host, port);

    m_pClient = new Client(&m_IOContext, endpoints, this);
    m_pThread = new std::thread([this]() { m_IOContext.run(); });

    Sleep(500);

    m_MyInfo.m_Nickname = nickname;

    m_pClient->Write(Message::Create(TAG_REQUEST::RECEIVE_MY_ID, nickname));
}

void CommunicationManager::ReceiveMessage(const TAG_REQUEST tag, const string& description)
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
    case TAG_REQUEST::SEND_EVENT_FIRE_BULLET:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& eventFireBulletStr = parsedDesc.second;

            stringstream ss(eventFireBulletStr);
            D3DXVECTOR3 pos;
            D3DXQUATERNION rot;
            float speed;
            float damage;
            int tag;
            ss >> pos.x >> pos.y >> pos.z 
               >> rot.x >> rot.y >> rot.z >> rot.w
               >> speed >> damage >> tag;
            BulletPool()()->Fire(pos, rot, speed, damage, 
                static_cast<TAG_COLLISION>(tag));
        }
        break;
    case TAG_REQUEST::SEND_EVENT_SOUND:
        {
            
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& eventSoundStr = parsedDesc.second;

            stringstream ss(eventSoundStr);
            //TAG_SOUND tagSound;
            int tagSound;
            D3DXVECTOR3 pos;

            ss >> pos.x >> pos.y >> pos.z
                >> tagSound;
            Sound()()->Play(static_cast<TAG_SOUND>(tagSound), pos, 0.3f, FMOD_3D);
        }
        break;
    }
}

void CommunicationManager::ReceiveID(const int id)
{
    m_MyInfo.m_ID = id;
    cout << "Received ID : " << id << '\n';
}

void CommunicationManager::SendID(const int id)
{
    m_RoomInfo.m_PlayerInfos[id].m_ID = id;

    m_pClient->Write(Message::Create(TAG_REQUEST::SEND_ID, to_string(id)));
}

void CommunicationManager::SendNickname(const string& nickname)
{
    m_RoomInfo.m_PlayerInfos[m_MyInfo.m_ID].m_Nickname = nickname;

    stringstream ss;
    ss << m_MyInfo.m_ID << nickname;
    m_pClient->Write(Message::Create(TAG_REQUEST::SEND_NICKNAME, ss.str()));
}

void CommunicationManager::SendPosition(const D3DXVECTOR3& pos)
{
    m_RoomInfo.m_PlayerInfos[m_MyInfo.m_ID].m_Position = pos;

    stringstream ss;
    ss << m_MyInfo.m_ID << pos.x << ' ' << pos.y << ' ' << pos.z;
    m_pClient->Write(Message::Create(TAG_REQUEST::SEND_POSITION, ss.str()));
}

void CommunicationManager::SendAnimationIndex(const int index)
{
    m_RoomInfo.m_PlayerInfos[m_MyInfo.m_ID].m_AnimationIndex = index;

    stringstream ss;
    ss << m_MyInfo.m_ID << index;
    m_pClient->Write(Message::Create(TAG_REQUEST::SEND_ANIMATION_INDEX, 
        ss.str()));
}

void CommunicationManager::SendAnimationTime(const float time)
{
    m_RoomInfo.m_PlayerInfos[m_MyInfo.m_ID].m_AnimationTime = time;

    stringstream ss;
    ss << m_MyInfo.m_ID << time;
    m_pClient->Write(Message::Create(TAG_REQUEST::SEND_ANIMATION_TIME,
        ss.str()));
}

void CommunicationManager::SendEventFireBullet(Bullet* pBullet)
{
    assert(pBullet && 
        "Communicator::SendEventFireBullet() failed. bullet is null.");

    auto tr = pBullet->GetTransform();
    auto pos = tr->GetPosition();
    auto rot = tr->GetRotation();

    stringstream ss;
    ss << m_MyInfo.m_ID << pos.x << ' ' << pos.y << ' ' << pos.z << ' '
       << rot.x << ' ' << rot.y << ' ' << rot.z << ' ' << rot.w << ' '
       << pBullet->GetSpeed() << ' ' << pBullet->GetDamage() << ' ' 
       << static_cast<int>(pBullet->GetTagCollision());

    m_pClient->Write(
        Message::Create(TAG_REQUEST::SEND_EVENT_FIRE_BULLET, ss.str()));
}

void CommunicationManager::SendEventSound(const TAG_SOUND tag, const D3DXVECTOR3& pos)
{
    
    int tagSound = static_cast<int>(tag);
    stringstream ss;
    ss << m_MyInfo.m_ID << pos.x << ' ' << pos.y << ' ' << pos.z << ' ' << tagSound;

    m_pClient->Write(Message::Create(TAG_REQUEST::SEND_EVENT_SOUND, ss.str()));
}
