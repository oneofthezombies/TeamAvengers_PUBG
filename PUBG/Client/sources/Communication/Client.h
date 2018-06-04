#pragma once

#include "Message.h"

class Client
{
private:
    boost::asio::io_context& m_ioContext;
    tcp::socket m_socket;
    Message m_readMsg;
    Message m_writeMsg;

    void Connect(const tcp::resolver::results_type& endpoints);
    void ReadRequest();
    void ReadDescriptionSizeInfo();

public:
    Client(boost::asio::io_context& ioContext, const tcp::resolver::results_type& endpoints);

    void Close();
};