#include "stdafx.h"
#include "Client.h"
#include "ComponentTransform.h"
#include "ScenePlay.h"
#include "IObject.h"
#include "Item.h"
#include "DeathDropBox.h"
#include "SceneLobby.h"

void Client::Connect(const tcp::resolver::results_type& endpoints)
{
    boost::asio::async_connect(m_socket, endpoints, [this](auto ec, auto) 
    {
        if (!ec)
        {
            ReadHeader();
        }
        else
        {
            cout << "Client::Connect() failed.\n";
        }
    });
}

void Client::ReadHeader()
{
    boost::asio::async_read(m_socket, 
        boost::asio::buffer(m_readMsg.GetData(), Message::HEADER_LENGTH), 
        [this](auto ec, auto) 
    {
        if (!ec && m_readMsg.DecodeHeader())
        {
            ReadBody();
        }
        else
        {
            cout << "Client::ReadHeader() failed.\n";
            m_socket.close();
        }
    });
}

void Client::ReadBody()
{
    boost::asio::async_read(m_socket, 
        boost::asio::buffer(m_readMsg.GetBodyData(), 
            m_readMsg.GetBodyLength()), [this](auto ec, auto) 
    {
        if (!ec)
        {
            if (pCommunicationManager)
                pCommunicationManager->ReceiveMessage(
                    m_readMsg.GetRequest(),
                    m_readMsg.GetDescription());
            
            ReadHeader();
        }
        else
        {
            cout << "Client::ReadBody() failed.\n";
            m_socket.close();
        }
    });
}

void Client::Write()
{
    boost::asio::async_write(m_socket, 
        boost::asio::buffer(m_writeMsg.GetData(), m_writeMsg.GetLength()), 
        [this](auto ec, auto) 
    {
        if (!ec)
        {
            // do nothing
        }
        else
        {
            cout << "Client::Write() failed.\n";
            m_socket.close();
        }
    });
}

Client::Client(
          boost::asio::io_context* pIOContext, 
    const tcp::resolver::results_type& endpoints, 
          Communication::Manager* pCommunicationManager)
    : m_socket(*pIOContext)
    , pIOContext(pIOContext)
    , pCommunicationManager(pCommunicationManager)
{
    Connect(endpoints);
}

void Client::Write(const Message& msg)
{
    boost::asio::post(*pIOContext, [this, msg]()
    {
        m_writeMsg = msg;
        Write();
    });
}

void Client::Close()
{
    boost::asio::post(*pIOContext, [this]() { m_socket.close(); });
}

Communication::Manager::Manager()
    : Singleton<Communication::Manager>()
    , m_pClient(nullptr)
    , m_pThread(nullptr)
    , m_IOContext()
    , m_resolver(m_IOContext)
    , m_playMode(PlayMode::WITH_OTHERS)
{
    for (auto c : m_roomInfo.playerInfos)
    {
        c.upperAnimState = 140;
        c.lowerAnimState = 140;
    }
}

Communication::Manager::~Manager()
{
}

void Communication::Manager::CheckConnection()
{
    assert(m_pClient && m_pThread && "client or thread is null.");
    assert(
        (m_myInfo.ID > -1) && 
        (m_myInfo.ID < static_cast<int>(m_roomInfo.playerInfos.size())) &&
        "my ID is wrong.");
}

void Communication::Manager::Destroy()
{
    if (m_pClient)
        m_pClient->Close();

    if (m_pThread)
        m_pThread->join();

    SAFE_DELETE(m_pClient);
    SAFE_DELETE(m_pThread);
}

void Communication::Manager::Print()
{
}

void Communication::Manager::SetPlayMode(const PlayMode playMode)
{
    m_playMode = playMode;
}

Communication::PlayMode Communication::Manager::GetPlayMode() const
{
    return m_playMode;
}

void Communication::Manager::Connect(
    const std::string& host, 
    const std::string& port,
    const std::string& nickname)
{
    if (m_playMode == PlayMode::ALONE) return;

    auto endpoints = m_resolver.resolve(host, port);

    m_pClient = new Client(&m_IOContext, endpoints, this);
    m_pThread = new std::thread([this]() { m_IOContext.run(); });

    Sleep(2000);

    m_myInfo.nickname = nickname;

    m_pClient->Write(Message::Create(TAG_REQUEST::RECEIVE_MY_ID, nickname));

    Sleep(2000);
}

void Communication::Manager::ReceiveMessage(
    const TAG_REQUEST tag, 
    const string& description)
{
    if (m_playMode == PlayMode::ALONE) return;

    switch (tag)
    {
    case TAG_REQUEST::RECEIVE_MY_ID:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;

            ReceiveID(id);
            
            m_roomInfo.playerInfos[id].ID = id;
            m_roomInfo.playerInfos[m_myInfo.ID].nickname = m_myInfo.nickname;

            SendNickname(m_myInfo.nickname);
            //SendID(m_myInfo.ID);
        }
        break;
    case TAG_REQUEST::SEND_ID:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;

            m_roomInfo.playerInfos[id].ID = id;
        }
        break;
    case TAG_REQUEST::SEND_NICKNAME:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& nickname = parsedDesc.second;

            GameInfo::PlayerInfo& pi = m_roomInfo.playerInfos[id];
            pi.nickname = nickname;
        }
        break;
    case TAG_REQUEST::SEND_POSITION_AND_ROTATION:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& positionRotationStr = parsedDesc.second;

            std::stringstream ss(positionRotationStr);
            D3DXVECTOR3 pos;
            D3DXQUATERNION rot;
            ss >> pos.x >> pos.y >> pos.z >> rot.x >> rot.y >> rot.z >> rot.w;

            GameInfo::PlayerInfo& pi = m_roomInfo.playerInfos[id];
            pi.position = pos;
            pi.rotation = rot;
            //pi.isApplied = false;
        }
        break;
    case TAG_REQUEST::SEND_HEAD_ANGLE:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& headAngleStr = parsedDesc.second;

            std::stringstream ss(headAngleStr);
            float headAngle = 0.0f;
            ss >> headAngle;

            m_roomInfo.playerInfos[id].headAngle = headAngle;
        }
        break;
    case TAG_REQUEST::SEND_UPPER_ANIMATION_INDEX:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& upperIndexStr = parsedDesc.second;

            m_roomInfo.playerInfos[id].upperAnimState = 
                std::stoi(upperIndexStr);
        }
        break;
    case TAG_REQUEST::SEND_LOWER_ANIMATION_INDEX:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& lowerIndexStr = parsedDesc.second;

            m_roomInfo.playerInfos[id].lowerAnimState =
                std::stoi(lowerIndexStr);
        }
        break;
    case TAG_REQUEST::SEND_IS_DEAD:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& isDeadStr = parsedDesc.second;

            std::stringstream ss(isDeadStr);
            int isDeadID;
            int isDeadInt;

            ss >> isDeadID >> isDeadInt;
            m_roomInfo.playerInfos[isDeadID].isDead = isDeadInt ? true : false;
        }
        break;
    case TAG_REQUEST::SEND_IS_READY:
        {
            std::pair<int, std::string> parsedDesc = Message::ParseDescription(description);
            int id = parsedDesc.first;
            std::string& isReadyStr = parsedDesc.second;

            std::stringstream ss(isReadyStr);
            int isReadyID;
            ss >> isReadyID;

            m_roomInfo.playerInfos[isReadyID].isReady = true;
        }
        break;
    case TAG_REQUEST::SEND_IS_NOT_READY:
        {
            std::pair<int, std::string> parsedDesc = Message::ParseDescription(description);
            int id = parsedDesc.first;
            std::string& isNotReadyStr = parsedDesc.second;

            std::stringstream ss(isNotReadyStr);
            int isNotReadyID;
            ss >> isNotReadyID;

            m_roomInfo.playerInfos[isNotReadyID].isReady = false;
        }
        break;
    //case TAG_REQUEST::SEND_EVENT_FIRE_BULLET:
    //    {
    //        auto parsedDesc = Message::ParseDescription(description);
    //        auto& id = parsedDesc.first;
    //        auto& eventFireBulletStr = parsedDesc.second;

    //        std::stringstream ss(eventFireBulletStr);
    //        D3DXVECTOR3 pos;
    //        //D3DXQUATERNION rot;
    //        D3DXVECTOR3 rot;
    //        float speed;
    //        float damage;
    //        int tag;
    //        ss >> pos.x >> pos.y >> pos.z 
    //           >> rot.x >> rot.y >> rot.z /*>> rot.w*/
    //           >> speed >> damage >> tag;

    //        BulletPool()()->Fire(Communication()()->m_myInfo,pos, rot, speed, damage,
    //            static_cast<TAG_COLLISION>(tag));
    //    }
    //    break;
    case TAG_REQUEST::SEND_EVENT_SOUND:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& eventSoundStr = parsedDesc.second;

            std::stringstream ss(eventSoundStr);
            int tagSound;
            D3DXVECTOR3 pos;

            ss >> pos.x >> pos.y >> pos.z
               >> tagSound;
            Sound()()->Play(static_cast<TAG_SOUND>(tagSound), pos, 0.3f, FMOD_3D);
        }
        break;
    case TAG_REQUEST::SEND_EVENT_MINUS_DAMAGE:
        {
            auto parsedDesc = Message::ParseDescription(description);
            auto& id = parsedDesc.first;
            auto& eventMinusDamageStr = parsedDesc.second;

            std::stringstream ss(eventMinusDamageStr);
            int damageID;
            float damage;
            
            ss >> damageID >> damage;
            m_roomInfo.playerInfos[damageID].health -= damage;
            if (m_roomInfo.playerInfos[damageID].health < 0.0f)
                m_roomInfo.playerInfos[damageID].health = 0.0f;
        }
        break;
    case TAG_REQUEST::SEND_EVENT_DESTROY_ITEM:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventDestroyItemStr = parsedDesc.second;

            std::stringstream ss(eventDestroyItemStr);
            std::string       destoryedItemName;
            std::getline(ss >> std::ws, destoryedItemName);

            ScenePlay* pScenePlay = static_cast<ScenePlay*>(CurrentScene()());
            Item* pItem = pScenePlay->FindItemWithName(destoryedItemName);
            const std::size_t cellIndex = pScenePlay->GetCellIndex(pItem->GetTransform()->GetPosition());
            pScenePlay->ItemIntoInventory(cellIndex, pItem);
            pScenePlay->Destroy(pItem);
        }
        break;
    case TAG_REQUEST::SEND_EVENT_MOVE_ITEM_FIELD_TO_HEAD:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventMoveItemStr = parsedDesc.second;

            std::stringstream ss(eventMoveItemStr);
            int pickerID;
            ss >> pickerID;
            std::string itemName;
            std::getline(ss >> std::ws, itemName);

            ScenePlay* pScenePlay = static_cast<ScenePlay*>(CurrentScene()());
            Item* pItem = pScenePlay->FindItemWithName(itemName);

            const std::size_t cellIndex = pScenePlay->GetCellIndex(
                pItem->GetTransform()->GetPosition());
            pScenePlay->ItemIntoInventory(cellIndex, pItem);
            pScenePlay->RemoveObject(pItem);

            const std::vector<Character*> characters = pScenePlay->GetCharacters();
            for (auto p : characters)
            {
                if (p->GetIndex() == pickerID)
                {
                    p->MoveItemFieldToHead(pItem);
                }
            }
        }
        break;
    case TAG_REQUEST::SEND_EVENT_MOVE_ITEM_FIELD_TO_ARMOR:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventMoveItemStr = parsedDesc.second;

            std::stringstream ss(eventMoveItemStr);
            int pickerID;
            ss >> pickerID;
            std::string itemName;
            std::getline(ss >> std::ws, itemName);

            ScenePlay* pScenePlay = static_cast<ScenePlay*>(CurrentScene()());
            Item* pItem = pScenePlay->FindItemWithName(itemName);

            const std::size_t cellIndex = pScenePlay->GetCellIndex(
                pItem->GetTransform()->GetPosition());
            pScenePlay->ItemIntoInventory(cellIndex, pItem);
            pScenePlay->RemoveObject(pItem);

            const std::vector<Character*> characters = pScenePlay->GetCharacters();
            for (auto p : characters)
            {
                if (p->GetIndex() == pickerID)
                {
                    p->MoveItemFieldToArmor(pItem);
                }
            }
        }
        break;
    case TAG_REQUEST::SEND_EVENT_MOVE_ITEM_FIELD_TO_BACK:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventMoveItemStr = parsedDesc.second;

            std::stringstream ss(eventMoveItemStr);
            int pickerID;
            ss >> pickerID;
            std::string itemName;
            std::getline(ss >> std::ws, itemName);

            ScenePlay* pScenePlay = static_cast<ScenePlay*>(CurrentScene()());
            Item* pItem = pScenePlay->FindItemWithName(itemName);

            const std::size_t cellIndex = pScenePlay->GetCellIndex(
                pItem->GetTransform()->GetPosition());
            pScenePlay->ItemIntoInventory(cellIndex, pItem);
            pScenePlay->RemoveObject(pItem);

            const std::vector<Character*> characters = pScenePlay->GetCharacters();
            for (auto p : characters)
            {
                if (p->GetIndex() == pickerID)
                {
                    p->MoveItemFieldToBack(pItem);
                }
            }
        }
        break;
    case TAG_REQUEST::SEND_EVENT_MOVE_ITEM_FIELD_TO_PRIMARY:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventMoveItemStr = parsedDesc.second;

            std::stringstream ss(eventMoveItemStr);
            int pickerID;
            ss >> pickerID;
            std::string itemName;
            std::getline(ss >> std::ws, itemName);

            ScenePlay* pScenePlay = static_cast<ScenePlay*>(CurrentScene()());
            Item* pItem = pScenePlay->FindItemWithName(itemName);

            const std::size_t cellIndex = pScenePlay->GetCellIndex(
                pItem->GetTransform()->GetPosition());
            pScenePlay->ItemIntoInventory(cellIndex, pItem);
            pScenePlay->RemoveObject(pItem);

            const std::vector<Character*> characters = pScenePlay->GetCharacters();
            for (auto p : characters)
            {
                if (p->GetIndex() == pickerID)
                {
                    p->MoveItemFieldToPrimary(pItem);
                }
            }
        }
        break;
    case TAG_REQUEST::SEND_EVENT_MOVE_ITEM_FIELD_TO_SECONDARY:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventMoveItemStr = parsedDesc.second;

            std::stringstream ss(eventMoveItemStr);
            int pickerID;
            ss >> pickerID;
            std::string itemName;
            std::getline(ss >> std::ws, itemName);

            ScenePlay* pScenePlay = static_cast<ScenePlay*>(CurrentScene()());
            Item* pItem = pScenePlay->FindItemWithName(itemName);

            const std::size_t cellIndex = pScenePlay->GetCellIndex(
                pItem->GetTransform()->GetPosition());
            pScenePlay->ItemIntoInventory(cellIndex, pItem);
            pScenePlay->RemoveObject(pItem);

            const std::vector<Character*> characters = pScenePlay->GetCharacters();
            for (auto p : characters)
            {
                if (p->GetIndex() == pickerID)
                {
                    p->MoveItemFieldToSecondary(pItem);
                }
            }
        }
        break;
    case TAG_REQUEST::SEND_EVENT_MOVE_ITEM_FIELD_TO_INVENTORY:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventMoveItemStr = parsedDesc.second;

            std::stringstream ss(eventMoveItemStr);
            int pickerID;
            ss >> pickerID;
            std::string itemName;
            std::getline(ss >> std::ws, itemName);

            ScenePlay* pScenePlay = static_cast<ScenePlay*>(CurrentScene()());
            Item* pItem = pScenePlay->FindItemWithName(itemName);

            const std::size_t cellIndex = pScenePlay->GetCellIndex(
                pItem->GetTransform()->GetPosition());
            pScenePlay->ItemIntoInventory(cellIndex, pItem);
            pScenePlay->RemoveObject(pItem);

            const std::vector<Character*> characters = pScenePlay->GetCharacters();
            for (auto p : characters)
            {
                if (p->GetIndex() == pickerID)
                {
                    p->MoveItemFieldToInventory(pItem);
                    return;
                }
            }
        }
        break;
    case TAG_REQUEST::SEND_EVENT_MOVE_ITEM_HEAD_TO_FIELD:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventMoveItemStr = parsedDesc.second;

            std::stringstream ss(eventMoveItemStr);

            int dropperID;
            ss >> dropperID;

            std::string itemName;
            std::getline(ss >> std::ws, itemName);

            ScenePlay* pScenePlay = static_cast<ScenePlay*>(CurrentScene()());
            Item* pItem = pScenePlay->FindItemWithName(itemName);
            const std::vector<Character*> characters = 
                pScenePlay->GetCharacters();
            for (auto p : characters)
            {
                if (p->GetIndex() == dropperID)
                {
                    Character::TotalInventory& inven = p->GetTotalInventory();
                    inven.DropItem(&inven.m_pEquipHead);
                    return;
                }
            }
        }
        break;
    case TAG_REQUEST::SEND_EVENT_MOVE_ITEM_ARMOR_TO_FIELD:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventMoveItemStr = parsedDesc.second;

            std::stringstream ss(eventMoveItemStr);

            int dropperID;
            ss >> dropperID;

            std::string itemName;
            std::getline(ss >> std::ws, itemName);

            ScenePlay* pScenePlay = static_cast<ScenePlay*>(CurrentScene()());
            Item* pItem = pScenePlay->FindItemWithName(itemName);
            const std::vector<Character*> characters =
                pScenePlay->GetCharacters();
            for (auto p : characters)
            {
                if (p->GetIndex() == dropperID)
                {
                    Character::TotalInventory& inven = p->GetTotalInventory();
                    inven.DropItem(&inven.m_pEquipArmor);
                    return;
                }
            }
        }
        break;
    case TAG_REQUEST::SEND_EVENT_MOVE_ITEM_BACK_TO_FIELD:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventMoveItemStr = parsedDesc.second;

            std::stringstream ss(eventMoveItemStr);

            int dropperID;
            ss >> dropperID;

            std::string itemName;
            std::getline(ss >> std::ws, itemName);

            ScenePlay* pScenePlay = static_cast<ScenePlay*>(CurrentScene()());
            Item* pItem = pScenePlay->FindItemWithName(itemName);
            const std::vector<Character*> characters =
                pScenePlay->GetCharacters();
            for (auto p : characters)
            {
                if (p->GetIndex() == dropperID)
                {
                    Character::TotalInventory& inven = p->GetTotalInventory();
                    inven.DropItem(&inven.m_pEquipBack);
                    return;
                }
            }
        }
        break;
    case TAG_REQUEST::SEND_EVENT_MOVE_ITEM_PRIMARY_TO_FIELD:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventMoveItemStr = parsedDesc.second;

            std::stringstream ss(eventMoveItemStr);

            int dropperID;
            ss >> dropperID;

            std::string itemName;
            std::getline(ss >> std::ws, itemName);

            ScenePlay* pScenePlay = static_cast<ScenePlay*>(CurrentScene()());
            Item* pItem = pScenePlay->FindItemWithName(itemName);
            const std::vector<Character*> characters =
                pScenePlay->GetCharacters();
            for (auto p : characters)
            {
                if (p->GetIndex() == dropperID)
                {
                    Character::TotalInventory& inven = p->GetTotalInventory();
                    inven.DropItem(&inven.m_pWeaponPrimary);
                    return;
                }
            }
        }
        break;
    case TAG_REQUEST::SEND_EVENT_MOVE_ITEM_SECONDARY_TO_FIELD:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventMoveItemStr = parsedDesc.second;

            std::stringstream ss(eventMoveItemStr);

            int dropperID;
            ss >> dropperID;

            std::string itemName;
            std::getline(ss >> std::ws, itemName);

            ScenePlay* pScenePlay = static_cast<ScenePlay*>(CurrentScene()());
            Item* pItem = pScenePlay->FindItemWithName(itemName);
            const std::vector<Character*> characters =
                pScenePlay->GetCharacters();
            for (auto p : characters)
            {
                if (p->GetIndex() == dropperID)
                {
                    Character::TotalInventory& inven = p->GetTotalInventory();
                    inven.DropItem(&inven.m_pWeaponSecondary);
                    return;
                }
            }
        }
        break;
    case TAG_REQUEST::SEND_EVENT_MOVE_ITEM_BULLETS_TO_FIELD:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventMoveItemStr = parsedDesc.second;

            std::stringstream ss(eventMoveItemStr);

            int dropperID;
            ss >> dropperID;

            std::string itemName;
            std::getline(ss >> std::ws, itemName);

            std::string bulletCountStr;
            std::getline(ss >> std::ws, bulletCountStr);
            const int bulletCount = std::stoi(bulletCountStr);

            ScenePlay* pScenePlay = static_cast<ScenePlay*>(CurrentScene()());
            const std::vector<Character*> characters =
                pScenePlay->GetCharacters();
            for (auto p : characters)
            {
                if (p->GetIndex() == dropperID)
                {
                    Character::TotalInventory& inven = p->GetTotalInventory();
                    auto& mapInven = inven.m_mapInventory;
                    bool isDropped = false;
                    for (auto it = mapInven.begin(); it != mapInven.end(); ++it)
                    {
                        auto& items = it->second;
                        for (auto it2 = items.begin(); it2 != items.end();)
                        {
                            if ((*it2)->GetName() == itemName)
                            {
                                auto index = std::distance(items.begin(), it2);
                                Item* pItem = items[index];
                                pItem->SetCount(bulletCount);
                                pItem->SetPosition(p->GetTransform()->GetPosition());
                                inven.DropItem(&items[index]);
                                it2 = items.erase(it2);
                                isDropped = true;
                            }
                            else
                            {
                                ++it2;
                            }
                        }
                    }

                    if (!isDropped)
                    {
                        auto& empties = inven.m_empties;
                        for (auto it = empties.begin(); it != empties.end(); ++it)
                        {
                            auto& items = it->second;
                            for (auto it2 = items.begin(); it2 != items.end();)
                            {
                                if ((*it2)->GetName() == itemName)
                                {
                                    auto index = std::distance(items.begin(), it2);
                                    Item* pItem = items[index];
                                    pItem->SetCount(bulletCount);
                                    pItem->SetPosition(p->GetTransform()->GetPosition());
                                    inven.DropItem(&items[index]);
                                    it2 = items.erase(it2);
                                    isDropped = true;
                                }
                                else
                                {
                                    ++it2;
                                }
                            }
                        }
                    }

                    return;
                }
            }
        }
        break;
    case TAG_REQUEST::SEND_EVENT_MOVE_ITEM_PRIMARY_TO_HAND:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventMoveItemStr = parsedDesc.second;

            std::stringstream ss(eventMoveItemStr);
            int moveItemID;
            ss >> moveItemID;

            const std::vector<Character*> characters = 
                static_cast<ScenePlay*>(CurrentScene()())->GetCharacters();

            for (auto c : characters)
            {
                if (c->GetIndex() == moveItemID)
                    c->MoveItemPrimaryToHand();
            }
        }
        break;
    case TAG_REQUEST::SEND_EVENT_MOVE_ITEM_SECONDARY_TO_HAND:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventMoveItemStr = parsedDesc.second;

            std::stringstream ss(eventMoveItemStr);

            int moveItemID;
            ss >> moveItemID;

            const std::vector<Character*> characters =
                static_cast<ScenePlay*>(CurrentScene()())->GetCharacters();

            for (auto c : characters)
            {
                if (c->GetIndex() == moveItemID)
                    c->MoveItemSecondaryToHand();
            }
        }
        break;
    case TAG_REQUEST::SEND_EVENT_MOVE_ITEM_HAND_TO_PRIMARY:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventMoveItemStr = parsedDesc.second;

            std::stringstream ss(eventMoveItemStr);

            int moveItemID;
            ss >> moveItemID;

            const std::vector<Character*> characters =
                static_cast<ScenePlay*>(CurrentScene()())->GetCharacters();

            for (auto c : characters)
            {
                if (c->GetIndex() == moveItemID)
                    c->MoveItemHandToPrimary();
            }
        }
        break;
    case TAG_REQUEST::SEND_EVENT_MOVE_ITEM_HAND_TO_SECONDARY:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventMoveItemStr = parsedDesc.second;

            std::stringstream ss(eventMoveItemStr);

            int moveItemID;
            ss >> moveItemID;

            const std::vector<Character*> characters =
                static_cast<ScenePlay*>(CurrentScene()())->GetCharacters();

            for (auto c : characters)
            {
                if (c->GetIndex() == moveItemID)
                    c->MoveItemHandToSecondary();
            }
        }
        break;
    case TAG_REQUEST::SEND_EVENT_CREATE_DEATH_DROP_BOX:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventCreateDeathDropBoxStr = parsedDesc.second;

            std::stringstream ss(eventCreateDeathDropBoxStr);

            int deathID;
            ss >> deathID;

            std::vector<std::pair<std::string, int>> consumes;
            std::string buf;
            while (std::getline(ss >> std::ws, buf))
            {
                std::string name = buf;

                std::getline(ss >> std::ws, buf);
                int count = std::stoi(buf);

                consumes.emplace_back(std::make_pair(name, count));
            }

            ScenePlay* pScenePlay = static_cast<ScenePlay*>(CurrentScene()());
            Character* pDeath = nullptr;
            for (auto pCharacter : pScenePlay->GetCharacters())
            {
                if (pCharacter->GetIndex() == deathID)
                {
                    pDeath = pCharacter;
                    break;
                }
            }

            // item 수량 변경
            for (auto& kv : pDeath->GetTotalInventory().m_mapInventory)
            {
                for (auto& item : kv.second)
                {
                    for (auto it = consumes.begin(); it != consumes.end();)
                    {
                        std::string name = it->first;
                        int count = it->second;

                        if (item->GetName() == name)
                        {
                            item->SetCount(count);
                            it = consumes.erase(it);
                        }
                        else
                        {
                            ++it;
                        }
                    }
                }
            }

            pDeath->CreateDeathDropBox();
        }
        break;
    case TAG_REQUEST::SEND_EVENT_MOVE_ITEM_BOX_TO_INVENTORY:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventMoveItemStr = parsedDesc.second;

            std::stringstream ss(eventMoveItemStr);
            int pickerID;
            ss >> pickerID;
            int boxID;
            ss >> boxID;
            std::string itemName;
            std::getline(ss >> std::ws, itemName);

            ScenePlay* pScenePlay = static_cast<ScenePlay*>(CurrentScene()());
            DeathDropBox* pBox = pScenePlay->GetDeathDropBox(boxID);
            Item* pItem = pBox->FindItem(itemName);

            if (pItem->IsInDeathDropBox())
                pItem->DeleteThisInDeathDropBox();

            const std::vector<Character*>& characters = 
                pScenePlay->GetCharacters();

            for (auto p : characters)
            {
                if (p->GetIndex() == pickerID)
                {
                    p->MoveItemFieldToInventory(pItem);
                    return;
                }
            }
        }
        break;
    case TAG_REQUEST::SEND_EVENT_MOVE_ITEM_BOX_TO_PRIMARY:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventMoveItemStr = parsedDesc.second;

            std::stringstream ss(eventMoveItemStr);
            int pickerID;
            ss >> pickerID;
            int boxID;
            ss >> boxID;
            std::string itemName;
            std::getline(ss >> std::ws, itemName);

            ScenePlay* pScenePlay = static_cast<ScenePlay*>(CurrentScene()());
            DeathDropBox* pBox = pScenePlay->GetDeathDropBox(boxID);
            Item* pItem = pBox->FindItem(itemName);

            if (pItem->IsInDeathDropBox())
                pItem->DeleteThisInDeathDropBox();

            const std::vector<Character*>& characters =
                pScenePlay->GetCharacters();

            for (auto p : characters)
            {
                if (p->GetIndex() == pickerID)
                {
                    p->MoveItemFieldToPrimary(pItem);
                    return;
                }
            }
        }
        break;
    case TAG_REQUEST::SEND_EVENT_MOVE_ITEM_BOX_TO_SECONDARY:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventMoveItemStr = parsedDesc.second;

            std::stringstream ss(eventMoveItemStr);
            int pickerID;
            ss >> pickerID;
            int boxID;
            ss >> boxID;
            std::string itemName;
            std::getline(ss >> std::ws, itemName);

            ScenePlay* pScenePlay = static_cast<ScenePlay*>(CurrentScene()());
            DeathDropBox* pBox = pScenePlay->GetDeathDropBox(boxID);
            Item* pItem = pBox->FindItem(itemName);

            if (pItem->IsInDeathDropBox())
                pItem->DeleteThisInDeathDropBox();

            const std::vector<Character*>& characters =
                pScenePlay->GetCharacters();

            for (auto p : characters)
            {
                if (p->GetIndex() == pickerID)
                {
                    p->MoveItemFieldToSecondary(pItem);
                    return;
                }
            }
        }
        break;
    case TAG_REQUEST::SEND_EVENT_MOVE_ITEM_BOX_TO_HEAD:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventMoveItemStr = parsedDesc.second;

            std::stringstream ss(eventMoveItemStr);
            int pickerID;
            ss >> pickerID;
            int boxID;
            ss >> boxID;
            std::string itemName;
            std::getline(ss >> std::ws, itemName);

            ScenePlay* pScenePlay = static_cast<ScenePlay*>(CurrentScene()());
            DeathDropBox* pBox = pScenePlay->GetDeathDropBox(boxID);
            Item* pItem = pBox->FindItem(itemName);

            if (pItem->IsInDeathDropBox())
                pItem->DeleteThisInDeathDropBox();

            const std::vector<Character*>& characters =
                pScenePlay->GetCharacters();

            for (auto p : characters)
            {
                if (p->GetIndex() == pickerID)
                {
                    p->MoveItemFieldToHead(pItem);
                    return;
                }
            }
        }
        break;
    case TAG_REQUEST::SEND_EVENT_MOVE_ITEM_BOX_TO_ARMOR:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventMoveItemStr = parsedDesc.second;

            std::stringstream ss(eventMoveItemStr);
            int pickerID;
            ss >> pickerID;
            int boxID;
            ss >> boxID;
            std::string itemName;
            std::getline(ss >> std::ws, itemName);

            ScenePlay* pScenePlay = static_cast<ScenePlay*>(CurrentScene()());
            DeathDropBox* pBox = pScenePlay->GetDeathDropBox(boxID);
            Item* pItem = pBox->FindItem(itemName);

            if (pItem->IsInDeathDropBox())
                pItem->DeleteThisInDeathDropBox();

            const std::vector<Character*>& characters =
                pScenePlay->GetCharacters();

            for (auto p : characters)
            {
                if (p->GetIndex() == pickerID)
                {
                    p->MoveItemFieldToArmor(pItem);
                    return;
                }
            }
        }
        break;
    case TAG_REQUEST::SEND_EVENT_MOVE_ITEM_BOX_TO_BACK:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventMoveItemStr = parsedDesc.second;

            std::stringstream ss(eventMoveItemStr);
            int pickerID;
            ss >> pickerID;
            int boxID;
            ss >> boxID;
            std::string itemName;
            std::getline(ss >> std::ws, itemName);

            ScenePlay* pScenePlay = static_cast<ScenePlay*>(CurrentScene()());
            DeathDropBox* pBox = pScenePlay->GetDeathDropBox(boxID);
            Item* pItem = pBox->FindItem(itemName);

            if (pItem->IsInDeathDropBox())
                pItem->DeleteThisInDeathDropBox();

            const std::vector<Character*>& characters =
                pScenePlay->GetCharacters();

            for (auto p : characters)
            {
                if (p->GetIndex() == pickerID)
                {
                    p->MoveItemFieldToBack(pItem);
                    return;
                }
            }
        }
        break;
    case TAG_REQUEST::SEND_EVENT_DESTROY_ITEM_IN_BOX:
        {
            auto parsedDesc = Message::ParseDescription(description);

            int& id = parsedDesc.first;
            std::string& eventMoveItemStr = parsedDesc.second;

            std::stringstream ss(eventMoveItemStr);
            int pickerID;
            ss >> pickerID;
            int boxID;
            ss >> boxID;
            std::string itemName;
            std::getline(ss >> std::ws, itemName);

            ScenePlay* pScenePlay = static_cast<ScenePlay*>(CurrentScene()());
            DeathDropBox* pBox = pScenePlay->GetDeathDropBox(boxID);
            Item* pItem = pBox->FindItem(itemName);

            if (pItem->IsInDeathDropBox())
                pItem->DeleteThisInDeathDropBox();

            SAFE_DELETE(pItem);
        }
        break;
    case TAG_REQUEST::RECEIVE_EVENT_START_PLAY:
        {
            SceneLobby* pSceneLobby = static_cast<SceneLobby*>(CurrentScene()());
            pSceneLobby->StartPlay();
        }
        break;
    case TAG_REQUEST::RECEIVE_EVENT_PLAYER_INFOS:
        {
            std::pair<int, std::string> parsedDesc = 
                Message::ParseDescription(description);

            int id = parsedDesc.first;
            std::string& eventPlayerInfosStr = parsedDesc.second;

            GameInfo::PlayerInfo& p0 = m_roomInfo.playerInfos[0];
            GameInfo::PlayerInfo& p1 = m_roomInfo.playerInfos[1];
            GameInfo::PlayerInfo& p2 = m_roomInfo.playerInfos[2];
            GameInfo::PlayerInfo& p3 = m_roomInfo.playerInfos[3];
            
            std::array<int, GameInfo::NUM_PLAYERS> isReadys;

            std::stringstream ss(eventPlayerInfosStr);
            ss >> p0.ID >> p0.nickname >> isReadys[0]; 
            isReadys[0] ? p0.isReady = true : p0.isReady = false;

            ss >> p1.ID >> p1.nickname >> isReadys[1];
            isReadys[1] ? p1.isReady = true : p1.isReady = false;

            ss >> p2.ID >> p2.nickname >> isReadys[2];
            isReadys[2] ? p2.isReady = true : p2.isReady = false;

            ss >> p3.ID >> p3.nickname >> isReadys[3];
            isReadys[3] ? p3.isReady = true : p3.isReady = false;
        }
        break;
    }
}

void Communication::Manager::ReceiveID(const int id)
{
    if (m_playMode == PlayMode::ALONE) return;

    m_myInfo.ID = id;
    cout << "Received ID : " << id << '\n';
}

void Communication::Manager::SendID(const int id)
{
    if (m_playMode == PlayMode::ALONE) return;

    m_pClient->Write(Message::Create(TAG_REQUEST::SEND_ID, to_string(id)));
}

void Communication::Manager::SendNickname(const string& nickname)
{
    if (m_playMode == PlayMode::ALONE) return;

    stringstream ss;
    ss << m_myInfo.ID << nickname;
    m_pClient->Write(Message::Create(TAG_REQUEST::SEND_NICKNAME, ss.str()));
}

void Communication::Manager::SendPositionAndRotation(
    const D3DXVECTOR3& p, 
    const D3DXQUATERNION& r)
{
    if (m_playMode == PlayMode::ALONE) return;

    m_roomInfo.playerInfos[m_myInfo.ID].position = p;
    m_roomInfo.playerInfos[m_myInfo.ID].rotation = r;

    std::stringstream ss;
    ss << m_myInfo.ID 
       << p.x << ' ' << p.y << ' ' << p.z << ' ' 
       << r.x << ' ' << r.y << ' ' << r.z << ' ' << r.w;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_POSITION_AND_ROTATION, 
            ss.str()));
}

void Communication::Manager::SendHeadAngle(const float angle)
{
    if (m_playMode == PlayMode::ALONE) return;

    m_roomInfo.playerInfos[m_myInfo.ID].headAngle = angle;

    std::stringstream ss;
    ss << m_myInfo.ID
       << angle;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_HEAD_ANGLE, 
            ss.str()));
}

void Communication::Manager::SendUpperAnimationIndex(
    const TAG_ANIM_CHARACTER tag)
{
    if (m_playMode == PlayMode::ALONE) return;

    const int index = static_cast<int>(tag);
    m_roomInfo.playerInfos[m_myInfo.ID].upperAnimState = index;

    std::stringstream ss;
    ss << m_myInfo.ID << index;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_UPPER_ANIMATION_INDEX, 
            ss.str()));
}

void Communication::Manager::SendLowerAnimationIndex(
    const TAG_ANIM_CHARACTER tag)
{
    if (m_playMode == PlayMode::ALONE) return;

    const int index = static_cast<int>(tag);
    m_roomInfo.playerInfos[m_myInfo.ID].lowerAnimState = index;

    std::stringstream ss;
    ss << m_myInfo.ID << index;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_LOWER_ANIMATION_INDEX, 
            ss.str()));
}

//void Communication::Manager::SendEventFireBullet(Bullet* pBullet)
//{
//    if (m_playMode == PlayMode::ALONE) return;
//
//    assert(pBullet && 
//        "Communicator::SendEventFireBullet() failed. bullet is null.");
//
//    auto tr = pBullet->GetTransform();
//    auto pos = tr->GetPosition();
//    auto rot = tr->GetRotation();
//
//    stringstream ss;
//    ss << m_myInfo.ID 
//       << pos.x << ' ' << pos.y << ' ' << pos.z << ' '
//       << rot.x << ' ' << rot.y << ' ' << rot.z << ' ' << rot.w << ' '
//       << pBullet->GetSpeed() << ' ' << pBullet->GetDamage() << ' ' 
//       /*<< static_cast<int>(pBullet->GetTagCollision())*/;
//
//    m_pClient->Write(
//        Message::Create(TAG_REQUEST::SEND_EVENT_FIRE_BULLET, ss.str()));
//}

void Communication::Manager::SendEventSound(
    const TAG_SOUND tag, 
    const D3DXVECTOR3& p)
{
    if (m_playMode == PlayMode::ALONE) return;

    const int tagSound = static_cast<int>(tag);
    stringstream ss;
    ss << m_myInfo.ID << p.x << ' ' << p.y << ' ' << p.z << ' ' << tagSound;

    m_pClient->Write(Message::Create(TAG_REQUEST::SEND_EVENT_SOUND, ss.str()));
}

void Communication::Manager::SendEventMinusDamage(
    const int id, 
    const float damage)
{
    if (m_playMode == PlayMode::ALONE) return;

    m_roomInfo.playerInfos[id].health -= damage;

    std::stringstream ss;
    ss << m_myInfo.ID << id << ' ' << damage;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_MINUS_DAMAGE, 
            ss.str()));
}

void Communication::Manager::SendEventDestroyItem(const std::string& itemName)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << itemName;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_DESTROY_ITEM, 
            ss.str()));
}

void Communication::Manager::SendEventMoveItemFieldToPrimary(
    const int id, 
    const std::string& itemName)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << id << ' ' << itemName;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_MOVE_ITEM_FIELD_TO_PRIMARY,
            ss.str()));
}

void Communication::Manager::SendEventMoveItemFieldToSecondary(
    const int id, 
    const std::string& itemName)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << id << ' ' << itemName;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_MOVE_ITEM_FIELD_TO_SECONDARY,
            ss.str()));
}

void Communication::Manager::SendEventMoveItemFieldToHead(
    const int id, 
    const std::string& itemName)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << id << ' ' << itemName;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_MOVE_ITEM_FIELD_TO_HEAD,
            ss.str()));
}

void Communication::Manager::SendEventMoveItemFieldToArmor(
    const int id, 
    const std::string& itemName)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << id << ' ' << itemName;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_MOVE_ITEM_FIELD_TO_ARMOR,
            ss.str()));
}

void Communication::Manager::SendEventMoveItemFieldToBack(
    const int id, 
    const std::string& itemName)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << id << ' ' << itemName;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_MOVE_ITEM_FIELD_TO_BACK,
            ss.str()));
}

void Communication::Manager::SendEventMoveItemFieldToInventory(
    const int id, 
    const std::string& itemName)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << id << ' ' << itemName;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_MOVE_ITEM_FIELD_TO_INVENTORY,
            ss.str()));
}

void Communication::Manager::SendEventMoveItemPrimaryToField(
    const int id, 
    const std::string& itemName)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << id << ' ' << itemName;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_MOVE_ITEM_PRIMARY_TO_FIELD,
            ss.str()));
}

void Communication::Manager::SendEventMoveItemSecondaryToField(
    const int id, 
    const std::string& itemName)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << id << ' ' << itemName;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_MOVE_ITEM_SECONDARY_TO_FIELD,
            ss.str()));
}

void Communication::Manager::SendEventMoveItemHeadToField(
    const int id, 
    const std::string& itemName)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << id << ' ' << itemName;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_MOVE_ITEM_HEAD_TO_FIELD,
            ss.str()));
}

void Communication::Manager::SendEventMoveItemArmorToField(
    const int id, 
    const std::string& itemName)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << id << ' ' << itemName;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_MOVE_ITEM_ARMOR_TO_FIELD,
            ss.str()));
}

void Communication::Manager::SendEventMoveItemBackToField(
    const int id, 
    const std::string& itemName)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << id << ' ' << itemName;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_MOVE_ITEM_BACK_TO_FIELD,
            ss.str()));
}

void Communication::Manager::SendEventMoveItemBulletsToField(
    const int id, 
    const std::string& itemName, 
    const int count)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << id << ' ' << itemName << '\n' << count;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_MOVE_ITEM_BULLETS_TO_FIELD,
            ss.str()));
}

void Communication::Manager::SendEventMoveItemPrimaryToHand(const int id)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << id;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_MOVE_ITEM_PRIMARY_TO_HAND,
            ss.str()));
}

void Communication::Manager::SendEventMoveItemSecondaryToHand(const int id)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << id;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_MOVE_ITEM_SECONDARY_TO_HAND,
            ss.str()));
}

void Communication::Manager::SendEventMoveItemHandToPrimary(const int id)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << id;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_MOVE_ITEM_HAND_TO_PRIMARY,
            ss.str()));
}

void Communication::Manager::SendEventMoveItemHandToSecondary(const int id)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << id;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_MOVE_ITEM_HAND_TO_SECONDARY,
            ss.str()));
}

void Communication::Manager::SendEventCreateDeathDropBox(
    const int id, 
    const std::vector<std::pair<std::string, int>>& consumes)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << id << ' ';

    for (auto& kv : consumes)
    {
        std::string itemName  = kv.first;
        int         itemCount = kv.second;

        ss << itemName << '\n' << std::to_string(itemCount) << '\n';
    }

    std::string s(ss.str());

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_CREATE_DEATH_DROP_BOX,
            ss.str()));
}

void Communication::Manager::SendEventMoveItemBoxToInventory(
    const int CharacterID, 
    const int boxID, 
    const std::string& itemName)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << CharacterID << ' ' << boxID << ' ' << itemName;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_MOVE_ITEM_BOX_TO_INVENTORY,
            ss.str()));
}

void Communication::Manager::SendEventMoveItemBoxToPrimary(
    const int CharacterID, 
    const int boxID, 
    const std::string& itemName)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << CharacterID << ' ' << boxID << ' ' << itemName;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_MOVE_ITEM_BOX_TO_PRIMARY,
            ss.str()));
}

void Communication::Manager::SendEventMoveItemBoxToSecondary(
    const int CharacterID, 
    const int boxID, 
    const std::string& itemName)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << CharacterID << ' ' << boxID << ' ' << itemName;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_MOVE_ITEM_BOX_TO_SECONDARY,
            ss.str()));
}

void Communication::Manager::SendEventMoveItemBoxToHead(
    const int CharacterID, 
    const int boxID, 
    const std::string& itemName)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << CharacterID << ' ' << boxID << ' ' << itemName;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_MOVE_ITEM_BOX_TO_HEAD,
            ss.str()));
}

void Communication::Manager::SendEventMoveItemBoxToArmor(
    const int CharacterID, 
    const int boxID, 
    const std::string& itemName)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << CharacterID << ' ' << boxID << ' ' << itemName;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_MOVE_ITEM_BOX_TO_ARMOR,
            ss.str()));
}

void Communication::Manager::SendEventMoveItemBoxToBack(
    const int CharacterID, 
    const int boxID, 
    const std::string& itemName)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << CharacterID << ' ' << boxID << ' ' << itemName;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_MOVE_ITEM_BOX_TO_BACK,
            ss.str()));
}

void Communication::Manager::SendEventDestroyItemInBox(
    const int CharacterID, 
    const int boxID, 
    const std::string& itemName)
{
    if (m_playMode == PlayMode::ALONE) return;

    std::stringstream ss;
    ss << m_myInfo.ID << CharacterID << ' ' << boxID << ' ' << itemName;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_EVENT_DESTROY_ITEM_IN_BOX,
            ss.str()));
}

void Communication::Manager::SendIsDead(
    const int id, 
    bool isDead)
{
    if (m_playMode == PlayMode::ALONE) return;

    m_roomInfo.playerInfos[id].isDead = isDead;
    
    std::stringstream ss;
    ss << m_myInfo.ID << id << ' ' << isDead;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_IS_DEAD,
            ss.str()));
}

void Communication::Manager::SendIsReady(const int characterID)
{
    if (m_playMode == PlayMode::ALONE) return;

    m_roomInfo.playerInfos[characterID].isReady = true;

    std::stringstream ss;
    ss << m_myInfo.ID << characterID;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_IS_READY,
            ss.str()));
}

void Communication::Manager::SendIsNotReady(const int characterID)
{
    if (m_playMode == PlayMode::ALONE) return;

    m_roomInfo.playerInfos[characterID].isReady = false;

    std::stringstream ss;
    ss << m_myInfo.ID << characterID;

    m_pClient->Write(
        Message::Create(
            TAG_REQUEST::SEND_IS_NOT_READY,
            ss.str()));
}

Communication::Manager* Communication::operator()()
{
    return Manager::GetInstance();
}
