#include "stdafx.h"
#include "Client.h"

void Client::Connect(const tcp::resolver::results_type& endpoints)
{
    boost::asio::async_connect(m_socket, 
                               endpoints, 
                               [this](boost::system::error_code ec, tcp::endpoint) 
    {
        if (!ec)
            ReadRequest();
    });
}

void Client::ReadRequest()
{
    boost::asio::async_read(m_socket, 
                            boost::asio::buffer(m_readMsg.GetRequestData(), Message::REQUEST_SIZE), 
                            [this](boost::system::error_code ec, size_t) 
    {
        if (!ec)
        {
            ReadDescriptionSizeInfo();
        }
        else
        {
            m_socket.close();
        }
    });
}

void Client::ReadDescriptionSizeInfo()
{
    boost::asio::async_read(m_socket, 
                            boost::asio::buffer(m_readMsg.GetDescriptionSizeInfoData(), Message::DESCRIPTION_SIZE_INFO_SIZE), 
                            [this](boost::system::error_code ec, size_t) 
    {
        if (!ec)
        {
            // parse

            ReadRequest();
        }
        else
        {
            m_socket.close();
        }
    });
}

Client::Client(boost::asio::io_context& ioContext, const tcp::resolver::results_type& endpoints)
    : m_ioContext(ioContext)
    , m_socket(ioContext)
{
}

void Client::Close()
{
    boost::asio::post(m_ioContext, [this]() { m_socket.close(); });
}
