#include "stdafx.h"
#include "Client.h"
#include "ComponentTransform.h"

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
    boost::asio::async_read(m_socket, 
        boost::asio::buffer(m_readMsg.GetData(), Message::HEADER_LENGTH), 
        [this](auto ec, auto) 
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
    boost::asio::async_read(m_socket, 
        boost::asio::buffer(m_readMsg.GetBodyData(), 
            m_readMsg.GetBodyLength()), [this](auto ec, auto) 
    {
        if (!ec)
        {
            if (pCommunicationManager)
                pCommunicationManager->ReceiveMessage(
                    m_readMsg.GetRequest(),
                    m_readMsg.GetDescription());
            
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
    boost::asio::async_write(m_socket, 
        boost::asio::buffer(m_writeMsg.GetData(), m_writeMsg.GetLength()), 
        [this](auto ec, auto) 
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

Client::Client(
          boost::asio::io_context* pIOContext, 
    const tcp::resolver::results_type& endpoints, 
          CommunicationManager* pCommunicationManager)
    : m_socket(*pIOContext)
    , pIOContext(pIOContext)
    , pCommunicationManager(pCommunicationManager)
{
    Connect(endpoints);
}

void Client::Write(const Message& msg)
{
    boost::asio::post(*pIOContext, [this, msg]()
    {
        m_writeMsg = msg;
        Write();
    });
}

void Client::Close()
{
    boost::asio::post(*pIOContext, [this]() { m_socket.close(); });
}

CommunicationManager::CommunicationManager()
    : Singleton<CommunicationManager>()
    , m_pClient(nullptr)
    , m_pThread(nullptr)
    , m_IOContext()
    , m_resolver(m_IOContext)
{
}

CommunicationManager::~CommunicationManager()
{
}

void CommunicationManager::CheckConnection()
{
    assert(m_pClient && m_pThread && "client or thread is null.");
    assert(
        (m_myInfo.ID > -1) && 
        (m_myInfo.ID < static_cast<int>(m_roomInfo.playerInfos.size())) &&
        "my ID is wrong.");
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

void CommunicationManager::Connect(
    const std::string& host, 
    const std::string& port,
    const std::string& nickname)
{
    auto endpoints = m_resolver.resolve(host, port);

    m_pClient = new Client(&m_IOContext, endpoints, this);
    m_pThread = new std::thread([this]() { m_IOContext.run(); });

    Sleep(1000);

    m_myInfo.nickname = nickname;

    m_pClient->Write(Message::Create(TAG_REQUEST::RECEIVE_MY_ID, nickname));
}

void CommunicationManager::ReceiveMessage(
    const TAG_REQUEST tag, 
    const string& description)
{
    switch (tag)
    {
    case TAG_REQUEST::RECEIVE_MY_ID:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;

            ReceiveID(id);
            
            SendID(m_myInfo.ID);
            SendNickname(m_myInfo.nickname);
        }
        break;
    case TAG_REQUEST::SEND_ID:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;

            m_roomInfo.playerInfos[id].ID = id;
        }
        break;
    case TAG_REQUEST::SEND_NICKNAME:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& nickname = parsedDesc.second;

            m_roomInfo.playerInfos[id].nickname = nickname;
        }
        break;
    case TAG_REQUEST::SEND_POSITION_AND_ROTATION:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& positionRotationStr = parsedDesc.second;

            std::stringstream ss(positionRotationStr);
            D3DXVECTOR3 pos;
            D3DXQUATERNION rot;
            ss >> pos.x >> pos.y >> pos.z >> rot.x >> rot.y >> rot.z >> rot.w;

            m_roomInfo.playerInfos[id].position = pos;
            m_roomInfo.playerInfos[id].rotation = rot;
        }
        break;
    case TAG_REQUEST::SEND_UPPER_ANIMATION_INDEX:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& upperIndexStr = parsedDesc.second;

            m_roomInfo.playerInfos[id].upperAnimState = 
                std::stoi(upperIndexStr);
        }
        break;
    case TAG_REQUEST::SEND_LOWER_ANIMATION_INDEX:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& lowerIndexStr = parsedDesc.second;

            m_roomInfo.playerInfos[id].lowerAnimState =
                std::stoi(lowerIndexStr);
        }
        break;
    case TAG_REQUEST::SEND_EVENT_FIRE_BULLET:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& eventFireBulletStr = parsedDesc.second;

            std::stringstream ss(eventFireBulletStr);
            D3DXVECTOR3 pos;
            //D3DXQUATERNION rot;
            D3DXVECTOR3 rot;
            float speed;
            float damage;
            int tag;
            ss >> pos.x >> pos.y >> pos.z 
               >> rot.x >> rot.y >> rot.z /*>> rot.w*/
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

            std::stringstream ss(eventSoundStr);
            int tagSound;
            D3DXVECTOR3 pos;

            ss >> pos.x >> pos.y >> pos.z
               >> tagSound;
            Sound()()->Play(static_cast<TAG_SOUND>(tagSound), pos, 0.3f, FMOD_3D);
        }
        break;
    case TAG_REQUEST::SEND_EVENT_MINUS_DAMAGE:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& eventMinusDamageStr = parsedDesc.second;

            std::stringstream ss(eventMinusDamageStr);
            int id;
            float damage;
            
            ss >> id >> damage;
            m_roomInfo.playerInfos[id].health -= damage;
        }
        break;
    }
}

void CommunicationManager::ReceiveID(const int id)
{
    m_myInfo.ID = id;
    cout << "Received ID : " << id << '\n';
}

void CommunicationManager::SendID(const int id)
{
    m_roomInfo.playerInfos[id].ID = id;

    m_pClient->Write(Message::Create(TAG_REQUEST::SEND_ID, to_string(id)));
}

void CommunicationManager::SendNickname(const string& nickname)
{
    m_roomInfo.playerInfos[m_myInfo.ID].nickname = nickname;

    stringstream ss;
    ss << m_myInfo.ID << nickname;
    m_pClient->Write(Message::Create(TAG_REQUEST::SEND_NICKNAME, ss.str()));
}

void CommunicationManager::SendPositionAndRotation(
    const D3DXVECTOR3& p, 
    const D3DXQUATERNION& r)
{
    m_roomInfo.playerInfos[m_myInfo.ID].position = p;
    m_roomInfo.playerInfos[m_myInfo.ID].rotation = r;

    std::stringstream ss;
    ss << m_myInfo.ID 
       << p.x << ' ' << p.y << ' ' << p.z << ' ' 
       << r.x << ' ' << r.y << ' ' << r.z << ' ' << r.w;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_POSITION_AND_ROTATION, 
            ss.str()));
}

void CommunicationManager::SendUpperAnimationIndex(
    const TAG_ANIM_CHARACTER tag)
{
    const int index = static_cast<int>(tag);
    m_roomInfo.playerInfos[m_myInfo.ID].upperAnimState = index;

    std::stringstream ss;
    ss << m_myInfo.ID << index;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_UPPER_ANIMATION_INDEX, 
            ss.str()));
}

void CommunicationManager::SendLowerAnimationIndex(
    const TAG_ANIM_CHARACTER tag)
{
    const int index = static_cast<int>(tag);
    m_roomInfo.playerInfos[m_myInfo.ID].lowerAnimState = index;

    std::stringstream ss;
    ss << m_myInfo.ID << index;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_LOWER_ANIMATION_INDEX, 
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
    ss << m_myInfo.ID 
       << pos.x << ' ' << pos.y << ' ' << pos.z << ' '
       << rot.x << ' ' << rot.y << ' ' << rot.z << ' ' << rot.w << ' '
       << pBullet->GetSpeed() << ' ' << pBullet->GetDamage() << ' ' 
       << static_cast<int>(pBullet->GetTagCollision());

    m_pClient->Write(
        Message::Create(TAG_REQUEST::SEND_EVENT_FIRE_BULLET, ss.str()));
}

void CommunicationManager::SendEventSound(
    const TAG_SOUND tag, 
    const D3DXVECTOR3& p)
{
    const int tagSound = static_cast<int>(tag);
    stringstream ss;
    ss << m_myInfo.ID << p.x << ' ' << p.y << ' ' << p.z << ' ' << tagSound;

    m_pClient->Write(Message::Create(TAG_REQUEST::SEND_EVENT_SOUND, ss.str()));
}

void CommunicationManager::SendEventMinusDamage(
    const int id, 
    const float damage)
{
    std::stringstream ss;
    ss << m_myInfo.ID << id << ' ' << damage;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_MINUS_DAMAGE, 
            ss.str()));
}
