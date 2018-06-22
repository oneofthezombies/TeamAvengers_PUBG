#include "stdafx.h"
#include "Server.h"

Room::Room()
    : m_ParticipantID(0)
{
}

void Room::Join(shared_ptr<Participant> participant)
{
    m_Participants.emplace(participant);
}

void Room::Leave(shared_ptr<Participant> participant)
{
    m_Participants.erase(participant);
}

int Room::GetID(const string& nickname)
{
    const auto search = m_NicknameIDs.find(nickname);
    if (search == m_NicknameIDs.end())
    {
        m_NicknameIDs.emplace(nickname, m_ParticipantID);
        ++m_ParticipantID;
    }
    return m_NicknameIDs[nickname];
}

vector<int> Room::GetOthersID(const int myID)
{
    vector<int> ids;
    for (auto& ni : m_NicknameIDs)
    {
        if (ni.second == myID) continue;

        ids.emplace_back(ni.second);
    }
    return ids;
}

void Room::Echo(const int id, const Message& msg)
{
    for (auto& p : m_Participants)
    {
        if (id == p->m_MyInfo.m_ID) continue;

        p->Write(msg);
    }
}

Participant::Participant(tcp::socket socket, Room* pRoom)
    : m_Socket(std::move(socket))
    , m_pRoom(pRoom)
    , m_MyInfo()
{
}

void Participant::Start()
{
    cout << "Particiapnt::Start()\n";

    m_pRoom->Join(shared_from_this());
    ReadHeader();
}

void Participant::Write(const Message& msg)
{
    m_WriteMsg = msg;
    Write();
}

void Participant::ReadHeader()
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
            m_pRoom->Leave(shared_from_this());
            cout << "Participant::ReadHeader() failed.\n";
        }
    });
}

void Participant::ReadBody()
{
    boost::asio::async_read(m_Socket, boost::asio::buffer(
        m_ReadMsg.GetBodyData(), m_ReadMsg.GetBodyLength()), 
        [this](auto ec, auto) 
    {
        if (!ec)
        {
            ReceiveMessage(m_ReadMsg.GetRequest(), m_ReadMsg.GetDescription());

            ReadHeader();
        }
        else
        {
            m_pRoom->Leave(shared_from_this());
            cout << "particiapnt::ReadBody() failed.\n";
        }
    });
}

void Participant::Write()
{
    boost::asio::async_write(m_Socket, boost::asio::buffer(
        m_WriteMsg.GetData(), m_WriteMsg.GetLength()), 
        [this](auto ec, auto) 
    {
        if (!ec)
        {
            // do nothing
        }
        else
        {
            m_pRoom->Leave(shared_from_this());
            cout << "Participant::Write() failed.\n";
        }
    });
}

void Participant::ReceiveMessage(const TAG_REQUEST tag, 
    const string& description)
{
    switch (tag)
    {
    case TAG_REQUEST::RECEIVE_MY_ID:
        {
            const string nickname(m_ReadMsg.GetDescription());
            const int id(m_pRoom->GetID(nickname));

            m_MyInfo.m_Nickname = nickname;
            m_MyInfo.m_ID = id;

            Write(Message::Create(TAG_REQUEST::RECEIVE_MY_ID, to_string(id)));

            cout << "nickname : " << nickname << " -> id : " << id << '\n';
        }
        break;
    case TAG_REQUEST::SEND_ID:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;

            m_pRoom->m_RoomInfo.m_PlayerInfos[id].m_ID = id;

            m_pRoom->Echo(id, Message::Create(TAG_REQUEST::SEND_ID, 
                description));
        }
        break;
    case TAG_REQUEST::SEND_NICKNAME:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& nickname = parsedDesc.second;

            m_pRoom->m_RoomInfo.m_PlayerInfos[id].m_Nickname = nickname;

            m_pRoom->Echo(id, Message::Create(TAG_REQUEST::SEND_NICKNAME, 
                description));
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
            m_pRoom->m_RoomInfo.m_PlayerInfos[id].m_Position = pos;

            m_pRoom->Echo(id, Message::Create(TAG_REQUEST::SEND_POSITION, 
                description));
        }
        break;
    }
}

Server::Server(boost::asio::io_context* ioContext, 
    const tcp::endpoint& endpoint)
    : m_Acceptor(*ioContext, endpoint)
{
    cout << "Server is Running...\n";

    Accept();
}

Server::~Server()
{
    cout << "Shut down Server.\n";
}

void Server::Accept()
{
    m_Acceptor.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) 
    {
        if (!ec)
        {
            make_shared<Participant>(move(socket), &m_Room)->Start();
        }

        Accept();
    });
}
