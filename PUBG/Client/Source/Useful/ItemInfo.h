#pragma once

struct ItemInfo
{
    static TAG_ITEM_CATEGORY GetItemCategory(const TAG_RES_STATIC tag); //�������� ��з�
    
    static string GetName(const TAG_RES_STATIC tag);             //�����۸�
    static string GetDescription(const TAG_RES_STATIC tag);      //������ ����
    
    static float  GetCapacity(const TAG_RES_STATIC tag);         //�뷮
    static float  GetFasterADS(const TAG_RES_STATIC tag);        //������ �ӵ�
    static float  GetMagnification(const TAG_RES_STATIC tag);    //����
    static float  GetDamageReduction(const TAG_RES_STATIC tag);  //���ذ��ҷ�
    static float  GetDefaultDurability(const TAG_RES_STATIC tag);//������
    static float  GetCapacityExtension(const TAG_RES_STATIC tag);//�����뷮
    static float  GetCastTime(const TAG_RES_STATIC tag);         //���ð�
    static float  GetRecovery(const TAG_RES_STATIC tag);         //ȸ������

    ////�� ���õ� ����� ���� ���ض� ����;;
};