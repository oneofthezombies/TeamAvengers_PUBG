#pragma once

enum class TAG_REQUEST
{
    TEST_CONNECTION,
    RECEIVE_MY_ID,

    SEND_ID,
    SEND_NICKNAME,
    SEND_POSITION_AND_ROTATION,
    SEND_HEAD_ANGLE,
    SEND_UPPER_ANIMATION_INDEX,
    SEND_LOWER_ANIMATION_INDEX,
    SEND_IS_DEAD,

    SEND_EVENT_FIRE_BULLET,
    SEND_EVENT_SOUND,
    SEND_EVENT_MINUS_DAMAGE,

    SEND_EVENT_DESTROY_ITEM,

    SEND_EVENT_MOVE_ITEM_FIELD_TO_PRIMARY,
    SEND_EVENT_MOVE_ITEM_FIELD_TO_SECONDARY,
    SEND_EVENT_MOVE_ITEM_FIELD_TO_HEAD,
    SEND_EVENT_MOVE_ITEM_FIELD_TO_ARMOR,
    SEND_EVENT_MOVE_ITEM_FIELD_TO_BACK,
    SEND_EVENT_MOVE_ITEM_FIELD_TO_INVENTORY,

    SEND_EVENT_MOVE_ITEM_PRIMARY_TO_FIELD,
    SEND_EVENT_MOVE_ITEM_SECONDARY_TO_FIELD,
    SEND_EVENT_MOVE_ITEM_HEAD_TO_FIELD,
    SEND_EVENT_MOVE_ITEM_ARMOR_TO_FIELD,
    SEND_EVENT_MOVE_ITEM_BACK_TO_FIELD,

    SEND_EVENT_MOVE_ITEM_PRIMARY_TO_HAND,
    SEND_EVENT_MOVE_ITEM_SECONDARY_TO_HAND,
    SEND_EVENT_MOVE_ITEM_HAND_TO_PRIMARY,
    SEND_EVENT_MOVE_ITEM_HAND_TO_SECONDARY,

    SEND_EVENT_CREATE_DEATH_DROP_BOX,
};
