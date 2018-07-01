#pragma once

struct ItemInfo
{
    static TAG_ITEM_CATEGORY GetItemCategory(const TAG_RES_STATIC tag); //아이템의 대분류
    
    static string GetName(const TAG_RES_STATIC tag);             //아이템명
    static string GetDescription(const TAG_RES_STATIC tag);      //아이템 설명
    
    static float  GetCapacity(const TAG_RES_STATIC tag);         //용량
    static float  GetFasterADS(const TAG_RES_STATIC tag);        //정조준 속도
    static float  GetMagnification(const TAG_RES_STATIC tag);    //배율
    static float  GetDamageReduction(const TAG_RES_STATIC tag);  //피해감소량
    static float  GetDefaultDurability(const TAG_RES_STATIC tag);//내구도
    static float  GetCapacityExtension(const TAG_RES_STATIC tag);//소지용량
    static float  GetCastTime(const TAG_RES_STATIC tag);         //사용시간
    static float  GetRecovery(const TAG_RES_STATIC tag);         //회복정도

    ////총 관련된 상수는 아직 안해뜸 ㅎㅎ;;
};