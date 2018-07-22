#pragma once
#include "Protocol.h"

class Room;

class Participant : public enable_shared_from_this<Participant>
{
public:
    GameInfo::MyInfo m_myInfo;

private:
    tcp::socket m_socket;
    Message     m_readMsg;
    Message     m_writeMsg;
    
    Room*       pRoom;

    void ReadHeader();
    void ReadBody();
    void Write();

    void ReceiveMessage(const TAG_REQUEST tag, const std::string& description);

public:
     Participant(tcp::socket socket, Room* pRoom);
    ~Participant() = default;

    void Start();
    void Write(const Message& msg);
};

class Room
{
public:
    GameInfo::RoomInfo m_roomInfo;

private:
    std::set<std::shared_ptr<Participant>> m_participants;
    int                                    m_participantID;
    std::map<std::string, int>             m_nicknameIDs;

public:
    Room();
    ~Room() = default;

    void Join(std::shared_ptr<Participant> participant);
    void Leave(std::shared_ptr<Participant> participant);
    int GetID(const std::string& nickname);
    std::vector<int> GetOthersID(const int myID);
    void Echo(const int id, const Message& msg);
    bool IsAllReady();
    void StartPlay();
    void SendPlayerInfos(const int receiveID);
};

class Server
{
private:
    tcp::acceptor m_acceptor;
    Room          m_room;

    void Accept();

public:
    Server(boost::asio::io_context* pIOContext, const tcp::endpoint& endpoint);
    ~Server();
};