#include "stdafx.h"
#include "Server.h"

Room::Room()
    : m_participantID(0)
{
}

void Room::Join(std::shared_ptr<Participant> participant)
{
    m_participants.emplace(participant);
}

void Room::Leave(std::shared_ptr<Participant> participant)
{
    m_participants.erase(participant);
}

int Room::GetID(const std::string& nickname)
{
    const auto search = m_nicknameIDs.find(nickname);
    if (search == m_nicknameIDs.end())
    {
        m_nicknameIDs.emplace(nickname, m_participantID);
        ++m_participantID;
    }
    return m_nicknameIDs[nickname];
}

std::vector<int> Room::GetOthersID(const int myID)
{
    std::vector<int> ids;
    for (auto& ni : m_nicknameIDs)
    {
        if (ni.second == myID) continue;

        ids.emplace_back(ni.second);
    }
    return ids;
}

void Room::Echo(const int id, const Message& msg)
{
    for (auto& p : m_participants)
    {
        if (id == p->m_myInfo.ID) continue;

        p->Write(msg);
    }
}

Participant::Participant(tcp::socket socket, Room* pRoom)
    : m_socket(std::move(socket))
    , m_myInfo()

    , pRoom(pRoom)
{
    assert(pRoom && "Participant::Constructor() failed. room is null.");
}

void Participant::Start()
{
    cout << "Particiapnt::Start()\n";

    pRoom->Join(shared_from_this());
    ReadHeader();
}

void Participant::Write(const Message& msg)
{
    m_writeMsg = msg;
    Write();
}

void Participant::ReadHeader()
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
            pRoom->Leave(shared_from_this());
            cout << "Participant::ReadHeader() failed.\n";
        }
    });
}

void Participant::ReadBody()
{
    boost::asio::async_read(m_socket, boost::asio::buffer(
        m_readMsg.GetBodyData(), m_readMsg.GetBodyLength()), 
        [this](auto ec, auto) 
    {
        if (!ec)
        {
            ReceiveMessage(m_readMsg.GetRequest(), m_readMsg.GetDescription());

            ReadHeader();
        }
        else
        {
            pRoom->Leave(shared_from_this());
            cout << "particiapnt::ReadBody() failed.\n";
        }
    });
}

void Participant::Write()
{
    boost::asio::async_write(m_socket, boost::asio::buffer(
        m_writeMsg.GetData(), m_writeMsg.GetLength()), 
        [this](auto ec, auto) 
    {
        if (!ec)
        {
            // do nothing
        }
        else
        {
            pRoom->Leave(shared_from_this());
            cout << "Participant::Write() failed.\n";
        }
    });
}

void Participant::ReceiveMessage(const TAG_REQUEST tag, 
    const std::string& description)
{
    switch (tag)
    {
    case TAG_REQUEST::RECEIVE_MY_ID:
        {
            const std::string nickname(m_readMsg.GetDescription());
            const int id(pRoom->GetID(nickname));

            m_myInfo.nickname = nickname;
            m_myInfo.ID = id;

            Write(
                Message::Create(
                    TAG_REQUEST::RECEIVE_MY_ID, 
                    std::to_string(id)));

            cout << "nickname : " << nickname << " -> id : " << id << '\n';
        }
        break;
    case TAG_REQUEST::SEND_ID:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;

            pRoom->m_roomInfo.playerInfos[id].ID = id;

            pRoom->Echo(
                id, 
                Message::Create(
                    TAG_REQUEST::SEND_ID, 
                    description));
        }
        break;
    case TAG_REQUEST::SEND_NICKNAME:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& nickname = parsedDesc.second;

            pRoom->m_roomInfo.playerInfos[id].nickname = nickname;

            pRoom->Echo(
                id, 
                Message::Create(
                    TAG_REQUEST::SEND_NICKNAME, 
                    description));
        }
        break;
    case TAG_REQUEST::SEND_POSITION_AND_ROTATION:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& positionStr = parsedDesc.second;

            std::stringstream ss(positionStr);
            D3DXVECTOR3 pos;
            D3DXQUATERNION rot;
            ss >> pos.x >> pos.y >> pos.z >> rot.x >> rot.y >> rot.z >> rot.w;
            pRoom->m_roomInfo.playerInfos[id].position = pos;
            pRoom->m_roomInfo.playerInfos[id].rotation = rot;

            pRoom->Echo(
                id, 
                Message::Create(
                    TAG_REQUEST::SEND_POSITION_AND_ROTATION,
                    description));
        }
        break;
    case TAG_REQUEST::SEND_UPPER_ANIMATION_INDEX:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& upperIndexStr = parsedDesc.second;

            pRoom->m_roomInfo.playerInfos[id].upperAnimState = 
                std::stoi(upperIndexStr);

            pRoom->Echo(
                id, 
                Message::Create(
                    TAG_REQUEST::SEND_UPPER_ANIMATION_INDEX, 
                    description));
        }
        break;
    case TAG_REQUEST::SEND_LOWER_ANIMATION_INDEX:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& lowerIndexStr = parsedDesc.second;

            pRoom->m_roomInfo.playerInfos[id].lowerAnimState = 
                std::stoi(lowerIndexStr);

            pRoom->Echo(
                id, 
                Message::Create(
                    TAG_REQUEST::SEND_LOWER_ANIMATION_INDEX, 
                    description));
        }
        break;
    case TAG_REQUEST::SEND_EVENT_FIRE_BULLET:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& eventFireBulletStr = parsedDesc.second;

            pRoom->Echo(id, Message::Create(
                TAG_REQUEST::SEND_EVENT_FIRE_BULLET, description));
        }
    case TAG_REQUEST::SEND_EVENT_SOUND:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& eventSoundStr = parsedDesc.second;

            pRoom->Echo(id, Message::Create(
                TAG_REQUEST::SEND_EVENT_SOUND, description));
        }
        break;
    }
}

Server::Server(boost::asio::io_context* pIOContext, 
    const tcp::endpoint& endpoint)
    : m_acceptor(*pIOContext, endpoint)
{
    assert(pIOContext && "Server::Constructor() failed. io context is null.");
    cout << "Server is Running...\n";

    Accept();
}

Server::~Server()
{
    cout << "Shut down Server.\n";
}

void Server::Accept()
{
    m_acceptor.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) 
    {
        if (!ec)
        {
            make_shared<Participant>(move(socket), &m_room)->Start();
        }

        Accept();
    });
}
