#include "stdafx.h"
#include "Character.h"
#include "Item.h"
#include "ItemInfo.h"
#include "ComponentTransform.h"
#include "UIImage.h"
#include "UIText.h"
#include "TagUIPosition.h"
#include "UIButton.h"
#include "ScenePlay.h"

using Event = UIButtonWithItem::Event;
using MouseButton = UIButtonWithItem::MouseButton;

void Character::onMouse(
    const Event event,
    const MouseButton button,
    UIButtonWithItem* pUIButtonWithItem)
{
    assert(
        pUIButtonWithItem && 
        "Character::onMouse() button is null.");

    Character::TotalInventory& inven = m_totalInventory;
    const TAG_UI_POSITION tag = pUIButtonWithItem->m_tagUIPosition;
    if (button == MouseButton::IDLE)
    {
        if (event == Event::ENTER &&
            !inven.m_stateClicked)
        {
            Item* pItem = pUIButtonWithItem->pItem;
            if (UIPosition::IsDropped(tag) ||
                UIPosition::IsInven(tag))
            {
                POINT mouse = Mouse::GetPosition();
                //inven.m_pDescriptionBorder->SetIsRender(true);
                inven.m_pDescriptionBorder->SetPosition(
                    D3DXVECTOR3(
                        static_cast<float>(mouse.x),
                        static_cast<float>(mouse.y), 0.0f));
                inven.m_pDescriptionText->SetText(ItemInfo::GetDescription(pItem->GetTagResStatic()));
                inven.m_pDescriptionName->SetText(ItemInfo::GetName(pItem->GetTagResStatic()));
                inven.m_pDescriptionNum->SetText("용량 : " + to_string(pItem->GetCount()));
                inven.m_pDescriptionImage->SetTexture(pItem->GetUIImage()->GetTexture());
            }
        }

        if (event == Event::EXIT &&
            !inven.m_stateClicked)
        {
            //inven.m_pDescriptionBorder->SetIsRender(true);

            const float max = std::numeric_limits<float>::max();
            inven.m_pDescriptionBorder->SetPosition(Vector3::ONE * max);
        }

        if (event == Event::DRAG &&
            !inven.m_stateClicked)
        {
            POINT mouse = Mouse::GetPosition();
            inven.m_pDescriptionBorder->SetPosition(
                D3DXVECTOR3(
                    static_cast<float>(mouse.x),
                    static_cast<float>(mouse.y), 0.0f));
        }
    }
    if (button == MouseButton::LEFT)
    {
        if (event == Event::DOWN && 
            !inven.m_stateClicked)
        {
            if (UIPosition::IsDropped(tag) || 
                UIPosition::IsInven(tag))
            {
                // 좌표
                POINT mouse = Mouse::GetPosition();

                inven.pUIPicked->SetPosition(
                    D3DXVECTOR3(
                        static_cast<float>(mouse.x - 21),
                        static_cast<float>(mouse.y - 21), 0.0f));
                inven.pUIPicked->pItem = pUIButtonWithItem->pItem;

                UIImage* pItemImage = pUIButtonWithItem->pItem->GetUIImage();
                UIImage* pPickedImage = static_cast<UIImage*>(inven.pUIPicked->GetChild(0));

                pPickedImage->SetTexture(pItemImage->GetTexture());
                pPickedImage->SetSize(pItemImage->GetSize());

                if (ItemInfo::GetItemCategory(inven.pUIPicked->pItem->GetTagResStatic()) == TAG_ITEM_CATEGORY::Rifle)
                {
                    inven.m_pWeapon1->SetIsActive(true);
                    inven.m_pWeapon2->SetIsActive(true);
                }
            }

            if (tag == TAG_UI_POSITION::Weapon1)
            {
                POINT mouse = Mouse::GetPosition();

                inven.pUIPicked->SetPosition(
                    D3DXVECTOR3(
                        static_cast<float>(mouse.x - 21),
                        static_cast<float>(mouse.y - 21), 0.0f));
                inven.pUIPicked->pItem = pUIButtonWithItem->pItem;

                UIImage* pItemImage = pUIButtonWithItem->pItem->GetUIImage();
                UIImage* pPickedImage = static_cast<UIImage*>(inven.pUIPicked->GetChild(0));

                pPickedImage->SetTexture(pItemImage->GetTexture());
                pPickedImage->SetSize(pItemImage->GetSize());
            }
            else if (tag == TAG_UI_POSITION::Weapon2 ||
                tag == TAG_UI_POSITION::Armor ||
                tag == TAG_UI_POSITION::Bag ||
                tag == TAG_UI_POSITION::Helmat)
            {
                POINT mouse = Mouse::GetPosition();

                inven.pUIPicked->SetPosition(
                    D3DXVECTOR3(
                        static_cast<float>(mouse.x - 21),
                        static_cast<float>(mouse.y - 21), 0.0f));
                inven.pUIPicked->pItem = pUIButtonWithItem->pItem;

                UIImage* pItemImage = pUIButtonWithItem->pItem->GetUIImage();
                UIImage* pPickedImage = static_cast<UIImage*>(inven.pUIPicked->GetChild(0));

                pPickedImage->SetTexture(pItemImage->GetTexture());
                pPickedImage->SetSize(pItemImage->GetSize());
            }
        }

        if (event == Event::DRAG)
        {
            if (tag == TAG_UI_POSITION::picked)
            {
                POINT mouse = Mouse::GetPosition();

                inven.pUIPicked->SetPosition(
                    D3DXVECTOR3(
                        static_cast<float>(mouse.x - 21),
                        static_cast<float>(mouse.y - 21), 0.0f));
            }
        }

        if (event == Event::UP)
        {
            if (tag == TAG_UI_POSITION::picked && 
                !pUIButtonWithItem->pItem->IsInInventory()) // dropped인 것
            {
                const D3DXVECTOR3& pos = pUIButtonWithItem->GetPosition();
                const auto cat =
                    ItemInfo::GetItemCategory(
                        pUIButtonWithItem->pItem->GetTagResStatic());

                if (pos.x < TotalInventory::FIRST_LINE)
                {
                }
                else if (pos.x < TotalInventory::SECOND_LINE)
                {
                    switch (cat)
                    {
                    case TAG_ITEM_CATEGORY::Ammo:
                    case TAG_ITEM_CATEGORY::Rifle:
                    case TAG_ITEM_CATEGORY::Head:
                    case TAG_ITEM_CATEGORY::Armor:
                    case TAG_ITEM_CATEGORY::Back:
                        {
                            if (PutItemInTotalInventory(pUIButtonWithItem->pItem))
                            {
                            }
                        }
                        break;
                    }
                }
                else    //3번째 장비 착용창.
                {
                    // OOTZ_FLAG 해당 칸 버튼 눌르면 거기로 껴짐

                    switch (cat)
                    {
                    case TAG_ITEM_CATEGORY::Ammo:
                    case TAG_ITEM_CATEGORY::Rifle:
                    case TAG_ITEM_CATEGORY::Head:
                    case TAG_ITEM_CATEGORY::Armor:
                    case TAG_ITEM_CATEGORY::Back:
                        {
                            if (PutItemInTotalInventory(pUIButtonWithItem->pItem))
                            {
                            }
                        }
                        break;
                    }
                }
                const float max = std::numeric_limits<float>::max();
                pUIButtonWithItem->SetPosition(Vector3::ONE * max);
            }

            /////////////////////인벤토리 안에서
            else if (
                tag == TAG_UI_POSITION::picked && 
                pUIButtonWithItem->pItem->IsInInventory())
            {
                const D3DXVECTOR3& pos = pUIButtonWithItem->GetPosition();
                const auto tag = pUIButtonWithItem->pItem->GetTagResStatic();
                const auto cat = ItemInfo::GetItemCategory(tag);

                if (pos.x < TotalInventory::FIRST_LINE) // 필드에 놓는다
                {
                    Item* pItem = pUIButtonWithItem->pItem;

                    const std::string itemName = pItem->GetName();

                    switch (cat)
                    {
                    case TAG_ITEM_CATEGORY::Ammo:
                        {
                            std::vector<Item*>& items = inven.m_mapInventory[tag];
                            for (auto it = items.begin(); it != items.end();)
                            {
                                if ((*it)->GetName() == itemName)
                                {
                                    auto index = std::distance(items.begin(), it);
                                    Item* pItem = items[index];
                                    pItem->SetPosition(GetTransform()->GetPosition());
                                    inven.m_capacity += ItemInfo::GetCapacity(tag) * pItem->GetCount();
                                    inven.DropItem(&items[index]);
                                    it = items.erase(it);
                                    cout << inven.m_capacity << endl;
                                }
                                else
                                {
                                    ++it;
                                }
                            }

                            Communication()()->SendEventMoveItemBulletsToField(m_index, itemName, pItem->GetCount());
                        }
                        break;

                    case TAG_ITEM_CATEGORY::Armor:
                        {
                            //inven.DropItem(&pUIButtonWithItem->pItem);
                            inven.DropArmor();
                            inven.m_pUIArmor->pUIImage = nullptr;
                            inven.m_pUIArmor->pItem = nullptr;
                            inven.m_pUIArmor->SetIsActive(false);
                            
                            Communication()()->SendEventMoveItemArmorToField(m_index, itemName);
                            
                        }
                        break;

                    case TAG_ITEM_CATEGORY::Back:
                        {
                            inven.DropBack();
                            Communication()()->SendEventMoveItemBackToField(m_index, itemName);
                            inven.m_pUIBack->pUIImage = nullptr;
                            inven.m_pUIBack->pItem = nullptr;
                            inven.m_pUIBack->SetIsActive(false);
                        }
                        break;

                    case TAG_ITEM_CATEGORY::Head:
                        {
                            inven.DropHead();
                            Communication()()->SendEventMoveItemHeadToField(m_index, itemName);
                            inven.m_pUIHead->pUIImage = nullptr;
                            inven.m_pUIHead->pItem = nullptr;
                            inven.m_pUIHead->SetIsActive(false);
                        }
                        break;

                    case TAG_ITEM_CATEGORY::Rifle:
                        {
                            bool isHand = false;
                            if (inven.m_pWeapon1->pItem &&
                                itemName == inven.m_pWeapon1->pItem->GetName())
                            {
                                if (inven.m_pHand && 
                                    itemName == inven.m_pHand->GetName())
                                {
                                    MoveItemHandToPrimary();
                                    Communication()()->SendEventMoveItemHandToPrimary(m_index);

                                    isHand = true;
                                }

                                inven.DropPrimary();
                            }
                            else if (inven.m_pWeapon2->pItem &&
                                itemName == inven.m_pWeapon2->pItem->GetName())
                            {
                                if (inven.m_pHand &&
                                    itemName == inven.m_pHand->GetName())
                                {
                                    MoveItemHandToSecondary();
                                    Communication()()->SendEventMoveItemHandToSecondary(m_index);

                                    isHand = true;
                                }

                                inven.DropSecondary();
                            }
                            else
                            {
                                cout << "Character::onMouse(), rifle name is unknown\n";
                                //assert(false && "Character::onMouse(), rifle name is unknown");
                            }

                            if (inven.pTempSaveWeaponForX &&
                                itemName == inven.pTempSaveWeaponForX->GetName())
                            {
                                inven.pTempSaveWeaponForX = nullptr;
                            }

                            if (isHand)
                            {
                                inven.m_handState = TAG_RIFLE::None;
                                m_attacking = Attacking::Unarmed;

                                TAG_ANIM_CHARACTER tagAnim = TAG_ANIM_CHARACTER::COUNT;
                                if (m_stance == Stance::Stand)
                                    tagAnim = TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1;
                                else if (m_stance == Stance::Crouch)
                                    tagAnim = TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Idling_1;
                                else if (m_stance == Stance::Prone)
                                    tagAnim = TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Idling_1;
                                assert((tagAnim != TAG_ANIM_CHARACTER::COUNT) && " Character::onMouse(), COUNT");

                                setAnimation(
                                    CharacterAnimation::BodyPart::BOTH,
                                    tagAnim,
                                    true);
                            }
                        }
                        break;
                    }
                }
                else if (pos.x < TotalInventory::SECOND_LINE)
                {
                }
                else
                {
                    switch (cat)
                    {
                    case TAG_ITEM_CATEGORY::Ammo:
                        {

                        }
                        break;
                    }
                }

                const float max = std::numeric_limits<float>::max();
                pUIButtonWithItem->SetPosition(Vector3::ONE * max);
            }
            Sound()()->Play(TAG_SOUND::UI_InputItem, Vector3::ZERO, 0.3f, FMOD_2D);

            //if (inven.m_pWeaponPrimary == nullptr && !(inven.m_handState == TAG_RIFLE::Primary))
            //{
            //    inven.m_pWeapon1->SetIsActive(false);
            //}
            //if (inven.m_pWeaponSecondary == nullptr && !(inven.m_handState == TAG_RIFLE::Secondary)
            //    /*&& inven.m_pHand == nullptr*/)
            //{
            //    inven.m_pWeapon2->SetIsActive(false);
            //}
            inven.m_stateClicked = false;
        }
    }

}
