#pragma once

#include "Message.h"

class Room;

class Participant : public enable_shared_from_this<Participant>
{
private:
    tcp::socket m_socket;
    Room* m_pRoom;
    Message m_readMsg;
    Message m_writeMsg;

    void ReadHeader();
    void ReadBody();
    void Write();

public:
    Participant(tcp::socket socket, Room* pRoom);
    ~Participant() = default;

    void Start();
};

class Room
{
private:
    unordered_set<shared_ptr<Participant>> m_participants;
    int m_participantID;
    unordered_map<string, int> m_nicknameIDs;

public:
    Room();
    ~Room() = default;

    void Join(shared_ptr<Participant> participant);
    void Leave(shared_ptr<Participant> participant);
    int GetID(const string& nickname);
};

class Server
{
private:
    tcp::acceptor m_acceptor;
    Room m_room;

    void Accept();

public:
    Server(boost::asio::io_context* ioContext, const tcp::endpoint& endpoint);
};