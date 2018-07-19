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

    if (button == MouseButton::LEFT)
    {
        if (event == Event::DOWN && !inven.m_stateClicked)
        {
            if (UIPosition::IsDropped(tag) || UIPosition::IsInven(tag))
            {
                // ��ǥ
                POINT mouse;
                GetCursorPos(&mouse);
                ScreenToClient(g_hWnd, &mouse);

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
            //if(UIPosition::IsInven(tag))

            if (tag == TAG_UI_POSITION::Weapon1)
            {
                POINT mouse;
                GetCursorPos(&mouse);
                ScreenToClient(g_hWnd, &mouse);

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
            else if (tag == TAG_UI_POSITION::Weapon2)
            {
                POINT mouse;
                GetCursorPos(&mouse);
                ScreenToClient(g_hWnd, &mouse);

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
            inven.m_stateClicked = true;

        }

        if (event == Event::DRAG)
        {
            if (tag == TAG_UI_POSITION::picked)
            {
                POINT mouse;
                GetCursorPos(&mouse);
                ScreenToClient(g_hWnd, &mouse);

                inven.pUIPicked->SetPosition(
                    D3DXVECTOR3(
                        static_cast<float>(mouse.x - 21),
                        static_cast<float>(mouse.y - 21), 0.0f));
            }
        }

        if (event == Event::UP)
        {
            if (tag == TAG_UI_POSITION::picked && 
                !pUIButtonWithItem->pItem->GetState())
            {
                const D3DXVECTOR3& pos = pUIButtonWithItem->GetPosition();
                const auto cat =
                    ItemInfo::GetItemCategory(
                        pUIButtonWithItem->pItem->GetTagResStatic());

                // ��ӵ� �κ� ���м� ��ġ
                const float firstLine = 233.0f;
                // TODO : �κ� ���� ���м� ��ġ �����ؾ� ��
                const float secondLine = 500.0f;
                if (pos.x < firstLine)
                {
                }
                else if (pos.x < secondLine)
                {
                    switch (cat)
                    {
                    case TAG_ITEM_CATEGORY::Ammo:
                        {
                            Item* pItem = pUIButtonWithItem->pItem;
                            if (PutItemInTotalInventory(pItem))
                            {
                                pItem->SetState(true);
                                CurrentScene()()->ItemIntoInventory(
                                    CurrentScene()()->GetCellIndex(
                                        pItem->GetTransform()->GetPosition()),
                                    pItem);
                                
                                if (pItem->IsInDeathDropBox())
                                    pItem->DeleteThisInDeathDropBox();
                            }
                        }
                        break;

                    case TAG_ITEM_CATEGORY::Rifle:
                        {
                            Item* pItem = pUIButtonWithItem->pItem;
                            if (PutItemInTotalInventory(pItem))
                            {
                                pItem->SetState(true);
                                CurrentScene()()->ItemIntoInventory(
                                    CurrentScene()()->GetCellIndex(
                                        pItem->GetTransform()->GetPosition()),
                                    pItem);

                                if (pItem->IsInDeathDropBox())
                                    pItem->DeleteThisInDeathDropBox();

                                /*UIImage* pItemImage = pUIButtonWithItem->pItem->GetUIImage2();*/

                                //���� ��ư.
                                if (inven.m_pWeaponPrimary)
                                {
                                    inven.m_pWeapon1->pUIImage = inven.m_pWeaponPrimary->GetUIImage2();
                                    inven.m_pWeapon1->pItem = inven.m_pWeaponPrimary;
                                }

                                if (inven.m_pWeaponSecondary)
                                {
                                    inven.m_pWeapon2->pUIImage = inven.m_pWeaponSecondary->GetUIImage2();
                                    inven.m_pWeapon2->pItem = inven.m_pWeaponSecondary;
                                }
                            }

                        }
                        break;
                    }
                }
                else    //3��° ��� ����â.
                {
                    switch (cat)
                    {
                    case TAG_ITEM_CATEGORY::Ammo:
                        {
                            //Item* pItem = pUIButtonWithItem->pItem;
                            //PutItemInTotalInventory(pItem);
                            //pItem->SetState(true);
                            //CurrentScene()()->ItemIntoInventory(
                            //    CurrentScene()()->GetCellIndex(
                            //        pItem->GetTransform()->GetPosition()),
                            //    pItem);
                        }
                        break;
                        /*case TAG_ITEM_CATEGORY::Rifle:
                        {
                        Item* pItem = pUIButtonWithItem->pItem;
                        PutItemInTotalInventory(pItem);
                        pItem->SetState(true);
                        CurrentScene()()->ItemIntoInventory(
                        CurrentScene()()->GetCellIndex(
                        pItem->GetTransform()->GetPosition()),
                        pItem);
                        UIImage* pItemImage = pUIButtonWithItem->pItem->GetUIImage2();
                        inven.m_pWeapon1->pUIImage = pItemImage;
                        inven.m_pWeapon1->pItem = pItem;

                        }*/
                        break;
                    }
                }
                const float max = std::numeric_limits<float>::max();
                pUIButtonWithItem->SetPosition(Vector3::ONE * max);
            }
            /////////////////////�κ��丮 �ȿ���
            else if (tag == TAG_UI_POSITION::picked
                && pUIButtonWithItem->pItem->GetState())
            {
                const D3DXVECTOR3& pos = pUIButtonWithItem->GetPosition();
                const auto cat =
                    ItemInfo::GetItemCategory(
                        pUIButtonWithItem->pItem->GetTagResStatic());

                // ��ӵ� �κ� ���м� ��ġ
                const float firstLine = 233.0f;
                // TODO : �κ� ���� ���м� ��ġ �����ؾ� ��
                const float secondLine = 500.0f;
                if (pos.x < firstLine)
                {
                    Item* pItem = pUIButtonWithItem->pItem;

                    pItem->SetState(false);
                    D3DXVECTOR3 p = m_totalInventory.pCharacter->GetTransform()->GetPosition();
                    CurrentScene()()->AddObject(pItem);
                    CurrentScene()()->InsertObjIntoTotalCellSpace(
                        TAG_OBJECT::Item,
                        CurrentScene()()->GetCellIndex(p),
                        pItem);

                    pItem->SetPosition(D3DXVECTOR3(p.x, p.y + 20, p.z));

                    TAG_RES_STATIC tag = pItem->GetTagResStatic();
                    std::vector<Item*>& items = inven.m_mapInventory[tag];
                    for (auto it = items.begin(); it != items.end(); ++it)
                    {
                        if (*it == pItem)
                        {
                            items.erase(it);
                            break;
                        }
                    }

                    pItem->GetTransform()->Update();

                    switch (cat)
                    {
                    case TAG_ITEM_CATEGORY::Rifle:
                        {
                            //if()
                            //�ٴڿ� ������ Effect
                            pItem->SetIsRenderSkinnedMesh(false);
                            pItem->SetIsRenderEffectMesh(true);
                            pItem->GetTransform()->SetRotation(Vector3::ZERO);
                            pItem->GetTransform()->Update();

                            if (pItem == inven.m_pWeapon1->pItem)
                            {
                                inven.m_pWeapon1->pUIImage = nullptr;
                                inven.m_pWeapon1->pItem = nullptr;
                                inven.m_pWeaponPrimary = nullptr;
                                if (inven.m_handState == TAG_RIFLE::Primary)
                                {
                                    inven.m_handState = TAG_RIFLE::None;
                                    inven.pTempSaveWeaponForX = nullptr;
                                }

                                // OOTZ_FLAG : ��Ʈ��ũ �����̸Ӹ� -> �ʵ�
                            }
                            else if (pItem == inven.m_pWeapon2->pItem)
                            {
                                inven.m_pWeapon2->pUIImage = nullptr;
                                inven.m_pWeapon2->pItem = nullptr;
                                inven.m_pWeaponSecondary = nullptr;
                                if (inven.m_handState == TAG_RIFLE::Secondary)
                                {
                                    inven.m_handState = TAG_RIFLE::None;
                                    inven.pTempSaveWeaponForX = nullptr;
                                }

                                // OOTZ_FLAG : ��Ʈ��ũ �������� -> �ʵ�
                            }

                            //�տ� ���� �Ǿ��ִ� �������� Ŭ���� �������̶� ���ٸ� 
                            if (pItem == inven.m_pHand)
                            {
                                inven.m_pHand = nullptr;
                                inven.m_handState = TAG_RIFLE::None;
                                inven.pTempSaveWeaponForX = nullptr;
                                m_attacking = Attacking::Unarmed;

                                //ĳ���� �ִϸ��̼�
                                {

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

                                // OOTZ_FLAG : ��Ʈ��ũ �ڵ� -> �ʵ�
                            }



                        }
                        break;

                    default:
                        break;
                    }
                    //pItem->
                }
                else if (pos.x < secondLine)
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
            if (inven.m_pWeaponPrimary == nullptr && !(inven.m_handState == TAG_RIFLE::Primary))
            {
                inven.m_pWeapon1->SetIsActive(false);
            }
            if (inven.m_pWeaponSecondary == nullptr && !(inven.m_handState == TAG_RIFLE::Secondary)
                /*&& inven.m_pHand == nullptr*/)
            {
                inven.m_pWeapon2->SetIsActive(false);
            }
            inven.m_stateClicked = false;
        }
    }

}
