#pragma once

#include "Message.h"

class Participant
{
public:
    Participant() = default;
    virtual ~Participant() = default;
};

class Room
{
private:
    unordered_set<shared_ptr<Participant>> m_participants;
    unordered_map<string, int>             m_nicknameIDs;
    int                                    m_participantID;
    unordered_map<int, string>             m_positions;

public:
    Room();
    ~Room() = default;

    void Join(shared_ptr<Participant> participant);
    void Leave(shared_ptr<Participant> participant);
    
    int GetID(const string& nickname);
};

class Session
    : public Participant
    , public enable_shared_from_this<Session>
{
private:
    tcp::socket m_socket;
    Room* m_room;
    Message m_readMsg;
    Message m_writeMsg;

    void ReadRequest();
    void ReadDescriptionSizeInfo();
    void ReadDescription();
    void Write();

    void Parse();

public:
    Session(tcp::socket socket, Room* room);
    virtual ~Session() = default;

    void Start();
};

class Server
{
private:
    tcp::acceptor m_acceptor;
    Room          m_room;

public:
    Server(boost::asio::io_context& ioContext, const tcp::endpoint& endpoint);
    ~Server() = default;

    void Accept();
};