#include "stdafx.h"
#include "Server.h"
#include "Protocol.h"

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

void Session::ReadRequest()
{
    boost::asio::async_read(m_socket, 
                            boost::asio::buffer(m_readMsg.GetRequestData(), Message::REQUEST_SIZE), 
                            [this](boost::system::error_code ec, size_t) 
    {
        if (ec)
        {
            m_room->Leave(shared_from_this());
        }
        else
        {
            ReadDescriptionSizeInfo();
        }
    });
}

void Session::ReadDescriptionSizeInfo()
{
    boost::asio::async_read(m_socket, 
                            boost::asio::buffer(m_readMsg.GetDescriptionSizeInfoData(), Message::DESCRIPTION_SIZE_INFO_SIZE), 
                            [this](boost::system::error_code ec, size_t) 
    {
        if (ec || !m_readMsg.IsValidDescriptionSize())
        {
            m_room->Leave(shared_from_this());
        }
        else
        {
            ReadDescription();
        }
    });
}

void Session::ReadDescription()
{
    boost::asio::async_read(m_socket,
                            boost::asio::buffer(m_readMsg.GetDescriptionData(), m_readMsg.GetDescriptionSize()), 
                            [this](boost::system::error_code ec, size_t) 
    {
        if (ec)
        {
            m_room->Leave(shared_from_this());
        }
        else
        {
            Parse();
            ReadRequest();
        }
    });
}

void Session::Write()
{
    boost::asio::async_write(m_socket, 
                             boost::asio::buffer(m_writeMsg.GetData(), m_writeMsg.GetSize()), 
                             [this](boost::system::error_code ec, size_t) 
    {
        if (ec)
            m_room->Leave(shared_from_this());
    });
}

void Session::Parse()
{
    const auto request = m_readMsg.GetRequest();
    switch (request)
    {
    case REQUEST::TestConnection:
        {

        }
        break;
    case REQUEST::MyID:
        {
            const auto description = m_readMsg.GetDescription();
            if (description.size() == 0) return;

            const int id = m_room->GetID(description);
            const string idStr = to_string(id);

            // set writeMsg
            stringstream ss;
            ss << setw(2) << setfill('0') << static_cast<int>(request);
            ss << setw(4) << setfill('0') << idStr.size();
            ss << idStr;

            m_writeMsg.SetDescriptionSize(idStr.size());
            m_writeMsg.SetMessage(ss.str());

            Write();
        }
        break;
    case REQUEST::OthersPosition:
        {

        }
        break;
    default:
        {
            // something error
        }
        break;
    }
}

Session::Session(tcp::socket socket, Room* room)
    : m_socket(move(socket))
    , m_room(room)
{
}

void Session::Start()
{
    m_room->Join(shared_from_this());
    ReadRequest();
}

Server::Server(boost::asio::io_context& ioContext, const tcp::endpoint& endpoint)
    : m_acceptor(ioContext, endpoint)
{
    Accept();
}

void Server::Accept()
{
    m_acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket)
    {
        if (!ec)
            make_shared<Session>(move(socket), &m_room)->Start();

        Accept();
    });
}
