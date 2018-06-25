#pragma once
#include "Protocol.h"

class Room;

class Participant : public enable_shared_from_this<Participant>
{
public:
    GameInfo::MyInfo m_MyInfo;

private:
    tcp::socket m_Socket;
    Message     m_ReadMsg;
    Message     m_WriteMsg;
    
    Room*       pRoom;

    void ReadHeader();
    void ReadBody();
    void Write();

    void ReceiveMessage(const TAG_REQUEST tag, const string& description);

public:
    Participant(tcp::socket socket, Room* pRoom);
    ~Participant() = default;

    void Start();
    void Write(const Message& msg);
};

class Room
{
public:
    GameInfo::RoomInfo m_RoomInfo;

private:
    unordered_set<shared_ptr<Participant>> m_Participants;
    int m_ParticipantID;

    unordered_map<string, int> m_NicknameIDs;

public:
    Room();
    ~Room() = default;

    void Join(shared_ptr<Participant> participant);
    void Leave(shared_ptr<Participant> participant);
    int GetID(const string& nickname);
    vector<int> GetOthersID(const int myID);
    void Echo(const int id, const Message& msg);
};

class Server
{
private:
    tcp::acceptor m_Acceptor;
    Room m_Room;

    void Accept();

public:
    Server(boost::asio::io_context* pIOContext, const tcp::endpoint& endpoint);
    ~Server();
};