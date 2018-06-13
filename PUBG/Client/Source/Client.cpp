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
                }
                break;
            }

            ReadHeader();
        }
        else
        {
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
            m_socket.close();
        }
    });
}

Client::Client(boost::asio::io_context* ioContext, const tcp::resolver::results_type& endpoints)
    : m_pIoContext(ioContext)
    , m_socket(*ioContext)
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
