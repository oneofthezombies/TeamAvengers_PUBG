#include "stdafx.h"
#include "Server.h"

Room::Room()
    : m_participantID(0)
{
}

void Room::Join(shared_ptr<Participant> participant)
{
    m_participants.emplace(participant);
}

void Room::Leave(shared_ptr<Participant> participant)
{
    m_participants.erase(participant);
}

int Room::GetID(const string& nickname)
{
    const auto search = m_nicknameIDs.find(nickname);
    if (search == m_nicknameIDs.end())
    {
        m_nicknameIDs.emplace(nickname, m_participantID);
        ++m_participantID;
    }
    return m_nicknameIDs[nickname];
}

vector<int> Room::GetOthersID(const int myID)
{
    vector<int> ids;
    for (auto& ni : m_nicknameIDs)
    {
        if (ni.second == myID) continue;

        ids.emplace_back(ni.second);
    }
    return ids;
}

void Room::Echo(const Message& msg)
{
    for (auto& p : m_participants)
        p->Echo(msg);
}

Participant::Participant(tcp::socket socket, Room* pRoom)
    : m_socket(std::move(socket))
    , m_pRoom(pRoom)
{
}

void Participant::Start()
{
    cout << "Joined.\n";
    m_pRoom->Join(shared_from_this());
    ReadHeader();
}

void Participant::Echo(const Message& msg)
{
    m_writeMsg = msg;
    Write();
}

void Participant::ReadHeader()
{
    boost::asio::async_read(m_socket, boost::asio::buffer(m_readMsg.GetData(), Message::HEADER_LENGTH), [this](auto ec, auto) 
    {
        if (!ec && m_readMsg.DecodeHeader())
        {
            ReadBody();
        }
        else
        {
            m_pRoom->Leave(shared_from_this());
        }
    });
}

void Participant::ReadBody()
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
                    const string nickname(m_readMsg.GetDescription());
                    const int id(m_pRoom->GetID(nickname));
                    Message::Create(&m_writeMsg, TAG_REQUEST::MY_ID, to_string(id));

                    cout << "nickname : " << nickname << " -> id : " << id << '\n';
                    m_pRoom->info.ID[id] = id;
                    m_pRoom->info.nickname[id] = nickname;

                    Write();
                }
                break;
            case TAG_REQUEST::ID:
                {
                    const string desc = m_readMsg.GetDescription();
                    const int id(std::stoi(desc));
                    m_pRoom->info.ID[id] = id;

                    Message::Create(&m_writeMsg, TAG_REQUEST::ID, desc);
                    m_pRoom->Echo(m_writeMsg);
                }
                break;
            case TAG_REQUEST::NICKNAME:
                {
                    const string desc = m_readMsg.GetDescription();
                    const int id(std::stoi(string(desc.begin(), desc.begin() + 1)));
                    const string nickname(string(desc.begin() + 1, desc.end()));
                    m_pRoom->info.ID[id] = id;
                    m_pRoom->info.nickname[id] = nickname;

                    Message::Create(&m_writeMsg, TAG_REQUEST::NICKNAME, desc);
                    m_pRoom->Echo(m_writeMsg);
                }
                break;
            case TAG_REQUEST::POSITION:
                {
                    const string desc = m_readMsg.GetDescription();
                    const int id(std::stoi(string(desc.begin(), desc.begin() + 1)));
                    stringstream ss(string(desc.begin() + 1, desc.end()));
                    D3DXVECTOR3 pos;
                    ss >> pos.x >> pos.y >> pos.z;
                    m_pRoom->info.position[id] = pos;

                    Message::Create(&m_writeMsg, TAG_REQUEST::POSITION, desc);
                    m_pRoom->Echo(m_writeMsg);
                }
                break;
            }

            ReadHeader();
        }
        else
        {
            m_pRoom->Leave(shared_from_this());
        }
    });
}

void Participant::Write()
{
    boost::asio::async_write(m_socket, boost::asio::buffer(m_writeMsg.GetData(), m_writeMsg.GetLength()), [this](auto ec, auto) 
    {
        if (!ec)
        {
            // do nothing
        }
        else
        {
            m_pRoom->Leave(shared_from_this());
        }
    });
}

Server::Server(boost::asio::io_context* ioContext, const tcp::endpoint& endpoint)
    : m_acceptor(*ioContext, endpoint)
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
    m_acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket) 
    {
        if (!ec)
        {
            make_shared<Participant>(move(socket), &m_room)->Start();
        }

        Accept();
    });
}

Room::Info::Info()
{
    for (auto& i : ID)
        i = -1;

    for (auto& n : nickname)
        n = "Unknown";
}