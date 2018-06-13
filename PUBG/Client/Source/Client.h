#pragma once
#include "Singleton.h"
#include "Message.h"

class Client
{
private:
    boost::asio::io_context* m_pIoContext;
    tcp::socket m_socket;
    Message m_readMsg;
    Message m_writeMsg;

    void Connect(const tcp::resolver::results_type& endpoints);
    void ReadHeader();
    void ReadBody();
    void Write();

public:
    Client(boost::asio::io_context* ioContext, const tcp::resolver::results_type& endpoints);

    void Write(const Message& msg);
    void Close();
};