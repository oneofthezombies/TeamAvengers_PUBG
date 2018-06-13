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

Participant::Participant(tcp::socket socket, Room* pRoom)
    : m_socket(std::move(socket))
    , m_pRoom(pRoom)
{
}

void Participant::Start()
{
    m_pRoom->Join(shared_from_this());
    ReadHeader();
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

                    Write();
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

void Server::Accept()
{
    m_acceptor.async_accept([this](auto ec, auto socket) 
    {
        if (!ec)
        {
            make_shared<Participant>(move(socket), &m_room)->Start();
        }

        Accept();
    });
}
